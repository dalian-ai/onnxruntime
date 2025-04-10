// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "contrib_ops/cpu/bert/attention_helper.h"
#include "contrib_ops/cpu/bert/attention_common.h"
#include "contrib_ops/cpu/bert/attention_parameters.h"
#include "contrib_ops/cpu/utils/dump_tensor.h"

#include "core/common/common.h"
#include "core/common/safeint.h"
#include "core/framework/op_kernel.h"

namespace onnxruntime {
namespace contrib {

class SparseAttentionBase {
 protected:
  SparseAttentionBase(const OpKernelInfo& info) {
    int64_t num_heads = 0;
    ORT_ENFORCE(info.GetAttr("num_heads", &num_heads).IsOK() && num_heads > 0);
    num_heads_ = static_cast<int>(num_heads);

    int64_t kv_num_heads = 0;
    ORT_ENFORCE(info.GetAttr("kv_num_heads", &kv_num_heads).IsOK() && kv_num_heads > 0);
    kv_num_heads_ = static_cast<int>(kv_num_heads);

    scale_ = info.GetAttrOrDefault<float>("scale", 0.0f);

    do_rotary_ = info.GetAttrOrDefault<int64_t>("do_rotary", 0) == 1;
    rotary_interleaved_ = info.GetAttrOrDefault<int64_t>("rotary_interleaved", 0) == 1;

    int64_t sparse_block_size = 0;
    ORT_ENFORCE(info.GetAttr("sparse_block_size", &sparse_block_size).IsOK());
    sparse_block_size_ = static_cast<int>(sparse_block_size);
  }

  int num_heads_;     // number of attention heads of Q
  int kv_num_heads_;  // number of attention heads of K or V
  float scale_;       // the scaling factor applied before softmax
  bool do_rotary_;    // whether or not to use rotary embeddings
  bool rotary_interleaved_;
  int sparse_block_size_;

  template <typename T>
  Status ApplyAttention(const T* Q,                             // Q data with shape BxNxSxH
                        const T* K,                             // K data with shape BxN_kvxSxH
                        const T* V,                             // V data with shape BxN_kvxSxH
                        const Tensor* past_key,                 // past K input tensor
                        const Tensor* past_value,               // past V input tensor
                        Tensor* output,                         // output tensor
                        Tensor* present_key,                    // present K output tensor
                        Tensor* present_value,                  // present V output tensor
                        const Tensor* total_key_lengths,        // total key lengths tensor
                        const Tensor* block_row_indices,        // block row indices
                        const Tensor* block_col_indices,        // block column indices
                        SparseAttentionParameters& parameters,  // attention parameters
                        AllocatorPtr allocator,                 // allocator for temporary tensors
                        OpKernelContext* context) const {
    const int batch_size = parameters.batch_size;
    const int sequence_length = parameters.sequence_length;
    const int head_size = parameters.head_size;
    const bool packed_qkv = parameters.is_packed_qkv;

    int past_buffer_sequence_length = static_cast<int>(past_key->Shape().GetDims()[2]);
    int present_buffer_sequence_length = static_cast<int>(present_key->Shape().GetDims()[2]);

    // Allocate a buffer to store Softmax(QK)
    bool attention_mlas_supported = MlasGQASupported<T>(CblasNoTrans, CblasTrans) &&
                                    MlasGQASupported<T>(CblasNoTrans, CblasNoTrans);
    size_t bytes = SafeInt<size_t>(batch_size) * num_heads_ * sequence_length * parameters.total_sequence_length *
                   (attention_mlas_supported ? sizeof(T) : sizeof(float));
    auto attention_probs = allocator->Alloc(bytes);
    BufferUniquePtr scratch_buffer(attention_probs, BufferDeleter(allocator));

    bool past_present_share_buffer = parameters.past_present_share_buffer;
    assert(past_present_share_buffer);

    auto* tp = context->GetOperatorThreadPool();

    const T* k = packed_qkv ? Q + num_heads_ * sequence_length * head_size : K;
    const T* v = packed_qkv ? Q + (num_heads_ + kv_num_heads_) * sequence_length * head_size : V;

    if (attention_mlas_supported) {
      ComputeAttentionProbs(
          static_cast<T*>(attention_probs), Q, k, total_key_lengths->Data<int32_t>(),
          batch_size, sequence_length, parameters.total_sequence_length,
          past_buffer_sequence_length, present_buffer_sequence_length, head_size,
          past_key->Data<T>(), present_key->MutableData<T>(), past_present_share_buffer, packed_qkv,
          block_row_indices->Data<int32_t>(), block_col_indices->Data<int32_t>(), parameters, tp, allocator);

      ComputeVxAttentionScore(
          output->MutableData<T>(), static_cast<T*>(attention_probs), v,
          total_key_lengths->Data<int32_t>(),
          batch_size, sequence_length, parameters.total_sequence_length,
          past_buffer_sequence_length, present_buffer_sequence_length, head_size, parameters.hidden_size,
          past_value->Data<T>(), present_value->MutableData<T>(), past_present_share_buffer, packed_qkv, tp, allocator);
    } else {
      ComputeAttentionProbs(
          static_cast<float*>(attention_probs), Q, k, total_key_lengths->Data<int32_t>(),
          batch_size, sequence_length, parameters.total_sequence_length,
          past_buffer_sequence_length, present_buffer_sequence_length, head_size,
          past_key->Data<T>(), present_key->MutableData<T>(), past_present_share_buffer, packed_qkv,
          block_row_indices->Data<int32_t>(), block_col_indices->Data<int32_t>(), parameters, tp, allocator);

      ComputeVxAttentionScore(
          output->MutableData<T>(), static_cast<float*>(attention_probs), v,
          total_key_lengths->Data<int32_t>(),
          batch_size, sequence_length, parameters.total_sequence_length,
          past_buffer_sequence_length, present_buffer_sequence_length, head_size, parameters.hidden_size,
          past_value->Data<T>(), present_value->MutableData<T>(), past_present_share_buffer, packed_qkv, tp, allocator);
    }

    return Status::OK();
  }

 private:
  // Helper function to compute the attention probs. It does 2 things:
  //  attention_probs(B, N, S, T) = 1/sqrt(H) x Q(B, N, S, H) x K'(B, N, T, H -> B, N, H, T)
  //  attention_probs(B, N, S, T) = Softmax(attention_probs)
  template <typename T, typename U>
  void ComputeAttentionProbs(
      U* attention_probs,                     // output buffer with size BxNxSxT
      const T* Q,                             // query start pointer
      const T* K,                             // key start pointer
      const int32_t* total_key_lengths,       // total key sequence lengths (past + new)
      int batch_size,                         // batch size
      int sequence_length,                    // sequence length of query or new key
      int total_sequence_length,              // maximum past_sequence_length + sequence_length
      int past_buffer_sequence_length,        // sequence length of past_key or past_value
      int present_buffer_sequence_length,     // sequence length of present_key or present_value
      int head_size,                          // head size of query
      const T* past_key,                      // past key
      T* present_key,                         // present key
      bool past_present_share_buffer,         // whether past_key and present_key share the buffer
      bool packed_qkv,                        // whether Q, K, V are packed
      const int32_t* block_row_indices,       // block row indices
      const int32_t* block_col_indices,       // block column indices
      SparseAttentionParameters& parameters,  // parameters
      ThreadPool* tp,                         // thread pool
      AllocatorPtr allocator) const {
    const bool is_prompt = (total_sequence_length == sequence_length);
    const ptrdiff_t packed_batch_stride =
        packed_qkv ? SafeInt<ptrdiff_t>(num_heads_ + 2 * kv_num_heads_) * sequence_length * head_size
                   : SafeInt<ptrdiff_t>(0);
    const int kv_num_heads_factor = num_heads_ / kv_num_heads_;
    const size_t q_input_chunk_length = static_cast<size_t>(sequence_length) * head_size;  // S x H
    const size_t kv_input_chunk_length = q_input_chunk_length;
    const size_t past_buff_chunk_length = static_cast<size_t>(past_buffer_sequence_length) * head_size;
    const size_t present_buff_chunk_length = static_cast<size_t>(present_buffer_sequence_length) * head_size;

    const int loop_len = batch_size * num_heads_;
    const float alpha = scale_ == 0.0f ? 1.0f / sqrt(static_cast<float>(head_size)) : scale_;

    TensorOpCost unit_cost;
    const ptrdiff_t probs_matrix_bytes =
        SafeInt<ptrdiff_t>(sequence_length) * total_sequence_length * sizeof(T);
    unit_cost.compute_cycles =
        static_cast<double>(SafeInt<ptrdiff_t>(2) * sequence_length * head_size * total_sequence_length);
    unit_cost.bytes_loaded =
        static_cast<double>((sequence_length + total_sequence_length) * head_size * sizeof(T));
    unit_cost.bytes_stored = static_cast<double>(probs_matrix_bytes);

    unit_cost.bytes_loaded += static_cast<double>(probs_matrix_bytes);
    unit_cost.bytes_stored += static_cast<double>(probs_matrix_bytes);

    // Cost to concatenate current key to cache (assume past and present share buffer).
    double bytes_to_copy_key = static_cast<double>(sizeof(T) * sequence_length * head_size);
    unit_cost.bytes_loaded += bytes_to_copy_key;
    unit_cost.bytes_stored += bytes_to_copy_key;

    DUMP_CPU_TENSOR_INIT();
    DUMP_CPU_TENSOR("block_row_indices", block_row_indices, parameters.num_sparse_layout, parameters.stride_row_indices);
    DUMP_CPU_TENSOR("block_col_indices", block_col_indices, parameters.num_sparse_layout, parameters.stride_col_indices);

    // Check whether each layout has sparse (has zero in lower triangular)
    std::vector<bool> layout_has_sparse(parameters.num_sparse_layout);
    for (int layout_index = 0; layout_index < parameters.num_sparse_layout; layout_index++) {
      int nonzero_elements = block_row_indices[(layout_index + 1) * parameters.stride_row_indices - 1];
      int dense_nonzero = (parameters.stride_row_indices * (parameters.stride_row_indices - 1)) / 2;
      layout_has_sparse[layout_index] = nonzero_elements < dense_nonzero;
      DUMP_CPU_STRING("layout_has_sparse[", layout_index, "]=", layout_has_sparse[layout_index]);
    }

    ThreadPool::TryParallelFor(tp, loop_len, unit_cost, [&](std::ptrdiff_t begin, std::ptrdiff_t end) {
      DUMP_CPU_STRING("batch_size=", batch_size, ",num_heads=", num_heads_, ",loop_len=", loop_len, ",begin=", begin, ",end=", end);
      for (std::ptrdiff_t i = begin; i != end; ++i) {
        const int batch_index = static_cast<int>(i) / num_heads_;
        const int head_index = static_cast<int>(i) % num_heads_;
        const int past_seq_len = is_prompt ? 0 : (static_cast<int>(total_key_lengths[batch_index]) - sequence_length);
        const size_t past_chunk_length = static_cast<size_t>(past_seq_len) * head_size;
        const int total_seq_len = total_key_lengths[batch_index];

        const ptrdiff_t output_offset = SafeInt<ptrdiff_t>(i) * sequence_length * total_sequence_length;
        U* output = attention_probs + output_offset;

        const T* k;
        if (packed_qkv) {
          k = K + packed_batch_stride * batch_index + kv_input_chunk_length * (head_index / kv_num_heads_factor);
        } else {
          k = K + kv_input_chunk_length * (i / kv_num_heads_factor);
        }

        // Concatenate past_k + k -> present_k
        // TODO: avoid copying mutiple times for a group.
        k = ConcatStateChunkGQA(past_key, k, present_key, present_buff_chunk_length, past_buff_chunk_length,
                                is_prompt ? 0 : past_chunk_length, kv_input_chunk_length, past_present_share_buffer,
                                i / kv_num_heads_factor);

        // Compute Q*K' + AttentionMask
        //                     original                 transposed             each iteration
        // A: Q                (B x N x) S x H          (B x N x) S x H        S x H
        // B: K'               (B x N x) T x H          (B x N x) H x T        H x T
        // C: attention_probs  (B x N x) S x T          (B x N x) S x T        S x T
        const T* q;
        if (packed_qkv) {
          q = Q + packed_batch_stride * batch_index + q_input_chunk_length * head_index;
        } else {
          q = Q + q_input_chunk_length * i;
        }

        DUMP_CPU_STRING("i=", i, ",batch_index=", batch_index, ",head_index=", head_index,
                        ",past_seq_len=", past_seq_len, ",total_seq_len=", total_seq_len, ",packed_qkv=", packed_qkv);
        DUMP_CPU_TENSOR("Q", q, sequence_length, head_size);
        DUMP_CPU_TENSOR("K", k, total_seq_len, head_size);

        if constexpr (std::is_same<T, float>::value) {
          math::GemmEx<T, ThreadPool>(CblasNoTrans, CblasTrans, sequence_length, total_seq_len, head_size, alpha, q,
                                      head_size, k, head_size, 0.0f /*bata*/, output, total_seq_len,
                                      nullptr);
        } else if constexpr (std::is_same<U, MLFloat16>::value) {
          MlasGemm(CblasNoTrans, CblasTrans, sequence_length, total_seq_len, head_size,
                   q, head_size, k, head_size, output, total_seq_len,
                   MLFloat16(alpha).val, static_cast<uint16_t>(0) /*beta*/, nullptr);
        } else {
          size_t bytes = static_cast<size_t>(head_size) * (sequence_length + total_seq_len) * sizeof(float);
          auto q_k_fp32 = allocator->Alloc(bytes);
          BufferUniquePtr scratch_buffer(q_k_fp32, BufferDeleter(allocator));

          float* q_fp32 = static_cast<float*>(q_k_fp32);
          MlasConvertHalfToFloatBuffer(q, q_fp32, static_cast<size_t>(head_size) * sequence_length);

          float* k_fp32 = q_fp32 + head_size * sequence_length;
          MlasConvertHalfToFloatBuffer(k, k_fp32, static_cast<size_t>(head_size) * total_seq_len);

          math::GemmEx<float, ThreadPool>(CblasNoTrans, CblasTrans, sequence_length, total_seq_len, head_size,
                                          alpha, q_fp32, head_size, k_fp32, head_size, 0.0f /*bata*/,
                                          output, total_seq_len, nullptr);
        }

        DUMP_CPU_TENSOR("QK", output, sequence_length, total_seq_len);

        // Compute Softmax for causal and output result in place.
        U* output_softmax = output;

        int layout_id = head_index % parameters.num_sparse_layout;
        bool is_sparse_layout = layout_has_sparse[layout_id];

        DUMP_CPU_STRING("layout_id=", layout_id, ",is_sparse_layout=", is_sparse_layout);

        if (!is_sparse_layout) {  // dense
          for (int q_id = 0; q_id < sequence_length; q_id++) {
            int causal_length = past_seq_len + q_id + 1;
            ComputeAttentionSoftmaxInplace(output_softmax, 1, causal_length, nullptr);
            for (int remain_seq_id = causal_length; remain_seq_id < total_seq_len; remain_seq_id++) {
              if constexpr (std::is_same<U, float>::value) {
                output_softmax[remain_seq_id] = 0.f;
              } else {
                output_softmax[remain_seq_id] = MLFloat16::FromBits(static_cast<uint16_t>(0));
              }
            }
            output_softmax += total_seq_len;
          }
        } else {  // sparse
          int q_id = 0;
          bool has_sparse = false;
          std::vector<int32_t> mask(parameters.max_sequence_length);

          const int32_t* layout_row_indices = block_row_indices + layout_id * parameters.stride_row_indices;
          const int32_t* layout_col_indices = block_col_indices + layout_id * parameters.stride_col_indices;
          do {
            int q_abs_position = past_seq_len + q_id;
            int causal_length = q_abs_position + 1;

            // Update mask when query token is the first or at the boundary of sparse block.
            if (q_id == 0 || q_abs_position % parameters.sparse_block_size == 0) {
              int row_in_sparse_layout = q_abs_position / parameters.sparse_block_size;
              int start_in_col_indices = layout_row_indices[row_in_sparse_layout];
              int end_in_col_indices = layout_row_indices[row_in_sparse_layout + 1];
              int nonzero_blocks = end_in_col_indices - start_in_col_indices;
              has_sparse = (nonzero_blocks != row_in_sparse_layout + 1);

              DUMP_CPU_STRING("q_id=", q_id,
                              ",q_abs_position=", q_abs_position,
                              ",sparse_block_size=", parameters.sparse_block_size,
                              ",row_in_sparse_layout=", row_in_sparse_layout,
                              ",start_in_col_indices=", start_in_col_indices,
                              ",end_in_col_indices=", end_in_col_indices,
                              ",nonzero_blocks=", nonzero_blocks,
                              ",has_sparse=", has_sparse);

              // Expand attention mask for current row of q_id
              if (has_sparse) {
                int block_aligned_length = q_abs_position / parameters.sparse_block_size * parameters.sparse_block_size + parameters.sparse_block_size;
                DUMP_CPU_STRING("block_aligned_length=", block_aligned_length);

                std::fill_n(mask.begin(), block_aligned_length, 0);
                for (int j = start_in_col_indices; j < end_in_col_indices; j++) {
                  int col_in_sparse_layout = layout_col_indices[j];

                  int offset = col_in_sparse_layout * parameters.sparse_block_size;
                  for (int s = 0; s < parameters.sparse_block_size; s++, offset++) {
                    mask[offset] = 1;
                  }
                }

                DUMP_CPU_TENSOR("mask", mask, block_aligned_length);
              }
            }

            // Update inline according to attention mask.
            if (has_sparse) {
              for (int s = 0; s < causal_length; s++) {
                if (mask[s] == 0) {
                  if constexpr (std::is_same<U, float>::value) {
                    output_softmax[s] = std::numeric_limits<T>::lowest();
                  } else {
                    output_softmax[s] = MLFloat16::FromBits(static_cast<uint16_t>(0xFBFF));
                  }
                }
              }
            }
            ComputeAttentionSoftmaxInplace(output_softmax, 1, causal_length, nullptr);

            for (int remain_seq_id = causal_length; remain_seq_id < total_seq_len; remain_seq_id++) {
              if constexpr (std::is_same<U, float>::value) {
                output_softmax[remain_seq_id] = 0.f;
              } else {
                output_softmax[remain_seq_id] = MLFloat16::FromBits(static_cast<uint16_t>(0));
              }
            }

            output_softmax += total_seq_len;
            q_id++;

          } while (q_id < sequence_length);
        }

        DUMP_CPU_TENSOR("softmax", output, sequence_length, total_seq_len);
      }
    });
  }

  template <typename T, typename U>
  void ComputeVxAttentionScore(T* output,                           // buffer for the result with size BxSxNxH
                               const U* attention_probs,            // Softmax of Q*K' with size BxNxSxT
                               const T* V,                          // v value with size BxN_kvxSxH
                               const int32_t* total_key_lengths,    // total sequence lengths
                               int batch_size,                      // batch size
                               int sequence_length,                 // sequence length
                               int total_sequence_length,           // maximum past_sequence_length + sequence_length
                               int past_buffer_sequence_length,     // sequence length in past state
                               int present_buffer_sequence_length,  // sequence length in past state
                               int head_size,                       // head size of Q, K, V
                               int hidden_size,                     // hidden size of Output
                               const T* past_value,                 // past value only
                               T* present_value,                    // present value only
                               bool past_present_share_buffer,      // whether past_key and present_key share the buffer
                               bool packed_qkv,                     // whether Q, K, V are packed
                               ThreadPool* tp,
                               AllocatorPtr allocator) const {
    const bool is_prompt = sequence_length == total_sequence_length;
    const ptrdiff_t packed_batch_stride =
        packed_qkv ? SafeInt<ptrdiff_t>(num_heads_ + 2 * kv_num_heads_) * sequence_length * head_size
                   : SafeInt<ptrdiff_t>(0);
    const int kv_num_heads_factor = num_heads_ / kv_num_heads_;

    const int kv_input_chunk_length = sequence_length * head_size;  // S x H
    const size_t past_buff_chunk_length = static_cast<size_t>(past_buffer_sequence_length) * head_size;
    const size_t present_buff_chunk_length = static_cast<size_t>(present_buffer_sequence_length) * head_size;

    // The cost of Gemm.
    TensorOpCost unit_cost;
    // Here we use total_sequence_length to estimate total_key_lengths[batch_index] used in GEMM.
    unit_cost.compute_cycles =
        static_cast<double>(SafeInt<ptrdiff_t>(2) * sequence_length * head_size * total_sequence_length);
    unit_cost.bytes_loaded = static_cast<double>(SafeInt<ptrdiff_t>(sequence_length + head_size) *
                                                 total_sequence_length * sizeof(T));
    unit_cost.bytes_stored = static_cast<double>(sequence_length * head_size * sizeof(T));

    if (present_value) {
      double bytes_to_copy_value = static_cast<double>(sizeof(T) * sequence_length * head_size);
      unit_cost.bytes_loaded += bytes_to_copy_value;
      unit_cost.bytes_stored += bytes_to_copy_value;
    }

    size_t output_fp32_bytes = 0;
    if constexpr (std::is_same<T, MLFloat16>::value && std::is_same<U, float>::value) {
      output_fp32_bytes = SafeInt<size_t>(sequence_length) * batch_size * num_heads_ * head_size * sizeof(float);
    }
    auto output_fp32 = allocator->Alloc(output_fp32_bytes);
    BufferUniquePtr scratch_buffer(output_fp32, BufferDeleter(allocator));

    DUMP_CPU_TENSOR_INIT();

    ThreadPool::TryParallelFor(
        tp, SafeInt<ptrdiff_t>(batch_size) * num_heads_, unit_cost, [&](std::ptrdiff_t begin, std::ptrdiff_t end) {
          DUMP_CPU_STRING("batch_size=", batch_size, ",num_heads=", num_heads_, ",begin=", begin, ",end=", end);

          for (std::ptrdiff_t i = begin; i != end; ++i) {
            const int batch_index = static_cast<int>(i / num_heads_);
            const int head_index = static_cast<int>(i % num_heads_);
            const int past_seq_len = is_prompt ? 0 : (static_cast<int>(total_key_lengths[batch_index]) - sequence_length);
            const size_t past_chunk_length = static_cast<size_t>(past_seq_len) * head_size;
            const int total_seq_len = total_key_lengths[batch_index];

            DUMP_CPU_STRING("i=", i, ",batch_index=", batch_index, ",head_index=", head_index,
                            ",past_seq_len=", past_seq_len, ",total_seq_len=", total_seq_len, ",packed_qkv=", packed_qkv);

            const T* v;
            if (packed_qkv) {
              v = V + packed_batch_stride * batch_index + kv_input_chunk_length * (head_index / kv_num_heads_factor);
            } else {
              v = V + kv_input_chunk_length * (i / kv_num_heads_factor);
            }

            // Concatenate past_v + v -> present_v
            v = ConcatStateChunkGQA(past_value, v, present_value, present_buff_chunk_length, past_buff_chunk_length,
                                    is_prompt ? 0 : past_chunk_length, kv_input_chunk_length, past_present_share_buffer,
                                    i / kv_num_heads_factor);

            DUMP_CPU_TENSOR("present_value", v, total_seq_len, head_size);

            T* output_current = output + (batch_index * sequence_length * num_heads_ + head_index) * head_size;
            ptrdiff_t attention_probs_offset = SafeInt<ptrdiff_t>(sequence_length) * total_seq_len * i;

            DUMP_CPU_TENSOR("attention_probs", attention_probs + attention_probs_offset, sequence_length, total_seq_len);

            if constexpr (std::is_same<T, float>::value) {
              math::GemmEx<T, ThreadPool>(CblasNoTrans, CblasNoTrans, sequence_length, head_size, total_seq_len,
                                          1.f, /*alpha*/
                                          attention_probs + attention_probs_offset, total_seq_len, v,
                                          head_size, 0.0f /*beta*/, output_current, hidden_size, nullptr);
            } else if constexpr (std::is_same<U, MLFloat16>::value) {
              MlasGemm(CblasNoTrans, CblasNoTrans, sequence_length, head_size, total_seq_len,
                       attention_probs + attention_probs_offset, total_seq_len,
                       v, head_size, output_current, hidden_size,
                       MLFloat16(1.0f).val, static_cast<uint16_t>(0) /*beta*/, nullptr);
            } else {
              size_t bytes = static_cast<size_t>(head_size) * total_seq_len * sizeof(float);
              auto v_fp32 = allocator->Alloc(bytes);
              BufferUniquePtr scratch_buffer(v_fp32, BufferDeleter(allocator));

              float* v_fp32_ptr = static_cast<float*>(v_fp32);
              MlasConvertHalfToFloatBuffer(v, v_fp32_ptr, static_cast<size_t>(head_size) * total_seq_len);

              float* output_fp32_current = static_cast<float*>(output_fp32) +
                                           (batch_index * sequence_length * num_heads_ + head_index) * head_size;
              math::GemmEx<float, ThreadPool>(CblasNoTrans, CblasNoTrans, sequence_length, head_size, total_seq_len,
                                              1.f, /*alpha*/ attention_probs + attention_probs_offset,
                                              total_seq_len, v_fp32_ptr,
                                              head_size, 0.0f /*beta*/, output_fp32_current,
                                              hidden_size, nullptr);
            }

            DUMP_CPU_TENSOR("out", attention_probs + attention_probs_offset, sequence_length, head_size);
          }
        });

    if constexpr (std::is_same<T, MLFloat16>::value && std::is_same<U, float>::value) {
      MlasConvertFloatToHalfBuffer(static_cast<float*>(output_fp32),
                                   output,
                                   SafeInt<size_t>(sequence_length) * batch_size * num_heads_ * head_size);
    }
  }
};

}  // namespace contrib
}  // namespace onnxruntime
