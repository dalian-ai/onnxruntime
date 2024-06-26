#if 0
;
; Note: shader requires additional functionality:
;       Use native low precision
;
;
; Input signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
;
; Output signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
; shader hash: 6a1d88feb14177832f5ee49ca330c549
;
; Pipeline Runtime Information: 
;
;
;
; Buffer Definitions:
;
; cbuffer 
; {
;
;   [128 x i8] (type annotation not present)
;
; }
;
; Resource bind info for 
; {
;
;   [2 x i8] (type annotation not present)
;
; }
;
; Resource bind info for 
; {
;
;   [2 x i8] (type annotation not present)
;
; }
;
; Resource bind info for 
; {
;
;   [2 x i8] (type annotation not present)
;
; }
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
;                                   cbuffer      NA          NA     CB0            cb0     1
;                                       UAV  struct         r/w      U0             u0     1
;                                       UAV  struct         r/w      U1             u1     1
;                                       UAV  struct         r/w      U2             u2     1
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:8-i16:16-i32:32-i64:64-f16:16-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%dx.types.CBufRet.i32 = type { i32, i32, i32, i32 }
%dx.types.ResRet.f16 = type { half, half, half, half, i32 }
%dx.types.CBufRet.f32 = type { float, float, float, float }
%"class.RWStructuredBuffer<half>" = type { half }
%Constants = type { i32, i32, i32, i32, <4 x i32>, <4 x i32>, <4 x i32>, <4 x i32>, <4 x i32>, <4 x i32>, i32, float, float, i32 }

define void @DFT() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 2, i32 2, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 1, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %3 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %4 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 2, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %5 = call i32 @dx.op.threadId.i32(i32 93, i32 0)  ; ThreadId(component)
  %6 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 0)  ; CBufferLoadLegacy(handle,regIndex)
  %7 = extractvalue %dx.types.CBufRet.i32 %6, 0
  %8 = add i32 %7, %5
  %9 = extractvalue %dx.types.CBufRet.i32 %6, 1
  %10 = icmp ult i32 %8, %9
  br i1 %10, label %11, label %190

; <label>:11                                      ; preds = %0
  %12 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 7)  ; CBufferLoadLegacy(handle,regIndex)
  %13 = extractvalue %dx.types.CBufRet.i32 %12, 3
  %14 = lshr i32 %13, 1
  %15 = extractvalue %dx.types.CBufRet.i32 %6, 2
  %16 = and i32 %15, 31
  %17 = shl i32 1, %16
  %18 = add i32 %15, 31
  %19 = and i32 %18, 31
  %20 = shl nuw i32 1, %19
  %21 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 3)  ; CBufferLoadLegacy(handle,regIndex)
  %22 = extractvalue %dx.types.CBufRet.i32 %21, 1
  %23 = extractvalue %dx.types.CBufRet.i32 %21, 2
  %24 = mul i32 %23, %22
  %25 = urem i32 %8, %24
  %26 = udiv i32 %8, %24
  %27 = udiv i32 %25, %23
  %28 = urem i32 %25, %23
  %29 = lshr i32 %27, %16
  %30 = shl i32 %29, %19
  %31 = add i32 %20, -1
  %32 = and i32 %27, %31
  %33 = add i32 %30, %32
  %34 = add i32 %33, %14
  %35 = extractvalue %dx.types.CBufRet.i32 %12, 0
  %36 = icmp eq i32 %35, 1
  %37 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 5)  ; CBufferLoadLegacy(handle,regIndex)
  %38 = extractvalue %dx.types.CBufRet.i32 %37, 2
  %39 = icmp ult i32 %33, %38
  %40 = and i1 %36, %39
  br i1 %40, label %41, label %56, !dx.controlflow.hints !14

; <label>:41                                      ; preds = %11
  %42 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 6)  ; CBufferLoadLegacy(handle,regIndex)
  %43 = extractvalue %dx.types.CBufRet.i32 %42, 2
  %44 = mul i32 %43, %33
  %45 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %1, i32 %44, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %46 = extractvalue %dx.types.ResRet.f16 %45, 0
  %47 = fpext half %46 to float
  %48 = extractvalue %dx.types.CBufRet.i32 %37, 3
  %49 = icmp eq i32 %48, 2
  br i1 %49, label %50, label %56, !dx.controlflow.hints !15

; <label>:50                                      ; preds = %41
  %51 = extractvalue %dx.types.CBufRet.i32 %42, 3
  %52 = add i32 %51, %44
  %53 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %1, i32 %52, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %54 = extractvalue %dx.types.ResRet.f16 %53, 0
  %55 = fpext half %54 to float
  br label %56

; <label>:56                                      ; preds = %50, %41, %11
  %57 = phi float [ %47, %50 ], [ %47, %41 ], [ 1.000000e+00, %11 ]
  %58 = phi float [ %55, %50 ], [ 0.000000e+00, %41 ], [ 0.000000e+00, %11 ]
  %59 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 1)  ; CBufferLoadLegacy(handle,regIndex)
  %60 = extractvalue %dx.types.CBufRet.i32 %59, 1
  %61 = icmp ult i32 %33, %60
  br i1 %61, label %62, label %83, !dx.controlflow.hints !16

; <label>:62                                      ; preds = %56
  %63 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 2)  ; CBufferLoadLegacy(handle,regIndex)
  %64 = extractvalue %dx.types.CBufRet.i32 %63, 0
  %65 = mul i32 %64, %26
  %66 = extractvalue %dx.types.CBufRet.i32 %63, 1
  %67 = mul i32 %66, %33
  %68 = add i32 %67, %65
  %69 = extractvalue %dx.types.CBufRet.i32 %63, 2
  %70 = mul i32 %69, %28
  %71 = add i32 %68, %70
  %72 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %3, i32 %71, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %73 = extractvalue %dx.types.ResRet.f16 %72, 0
  %74 = fpext half %73 to float
  %75 = extractvalue %dx.types.CBufRet.i32 %59, 3
  %76 = icmp eq i32 %75, 2
  br i1 %76, label %77, label %83, !dx.controlflow.hints !17

; <label>:77                                      ; preds = %62
  %78 = extractvalue %dx.types.CBufRet.i32 %63, 3
  %79 = add i32 %78, %71
  %80 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %3, i32 %79, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %81 = extractvalue %dx.types.ResRet.f16 %80, 0
  %82 = fpext half %81 to float
  br label %83

; <label>:83                                      ; preds = %77, %62, %56
  %84 = phi float [ %74, %77 ], [ %74, %62 ], [ 0.000000e+00, %56 ]
  %85 = phi float [ %82, %77 ], [ 0.000000e+00, %62 ], [ 0.000000e+00, %56 ]
  %86 = fmul fast float %84, %57
  %87 = fmul fast float %85, %58
  %88 = fmul fast float %84, %58
  %89 = fmul fast float %85, %57
  %90 = icmp ult i32 %34, %38
  %91 = and i1 %36, %90
  br i1 %91, label %92, label %107, !dx.controlflow.hints !14

; <label>:92                                      ; preds = %83
  %93 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 6)  ; CBufferLoadLegacy(handle,regIndex)
  %94 = extractvalue %dx.types.CBufRet.i32 %93, 2
  %95 = mul i32 %94, %34
  %96 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %1, i32 %95, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %97 = extractvalue %dx.types.ResRet.f16 %96, 0
  %98 = fpext half %97 to float
  %99 = extractvalue %dx.types.CBufRet.i32 %37, 3
  %100 = icmp eq i32 %99, 2
  br i1 %100, label %101, label %107, !dx.controlflow.hints !15

; <label>:101                                     ; preds = %92
  %102 = extractvalue %dx.types.CBufRet.i32 %93, 3
  %103 = add i32 %102, %95
  %104 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %1, i32 %103, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %105 = extractvalue %dx.types.ResRet.f16 %104, 0
  %106 = fpext half %105 to float
  br label %107

; <label>:107                                     ; preds = %101, %92, %83
  %108 = phi float [ %98, %101 ], [ %98, %92 ], [ 1.000000e+00, %83 ]
  %109 = phi float [ %106, %101 ], [ 0.000000e+00, %92 ], [ 0.000000e+00, %83 ]
  %110 = icmp ult i32 %34, %60
  br i1 %110, label %111, label %132, !dx.controlflow.hints !16

; <label>:111                                     ; preds = %107
  %112 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 2)  ; CBufferLoadLegacy(handle,regIndex)
  %113 = extractvalue %dx.types.CBufRet.i32 %112, 0
  %114 = mul i32 %113, %26
  %115 = extractvalue %dx.types.CBufRet.i32 %112, 1
  %116 = mul i32 %115, %34
  %117 = add i32 %116, %114
  %118 = extractvalue %dx.types.CBufRet.i32 %112, 2
  %119 = mul i32 %118, %28
  %120 = add i32 %117, %119
  %121 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %3, i32 %120, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %122 = extractvalue %dx.types.ResRet.f16 %121, 0
  %123 = fpext half %122 to float
  %124 = extractvalue %dx.types.CBufRet.i32 %59, 3
  %125 = icmp eq i32 %124, 2
  br i1 %125, label %126, label %132, !dx.controlflow.hints !17

; <label>:126                                     ; preds = %111
  %127 = extractvalue %dx.types.CBufRet.i32 %112, 3
  %128 = add i32 %127, %120
  %129 = call %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32 139, %dx.types.Handle %3, i32 %128, i32 0, i8 1, i32 2)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %130 = extractvalue %dx.types.ResRet.f16 %129, 0
  %131 = fpext half %130 to float
  br label %132

; <label>:132                                     ; preds = %126, %111, %107
  %133 = phi float [ %123, %126 ], [ %123, %111 ], [ 0.000000e+00, %107 ]
  %134 = phi float [ %131, %126 ], [ 0.000000e+00, %111 ], [ 0.000000e+00, %107 ]
  %135 = fmul fast float %133, %108
  %136 = fmul fast float %134, %109
  %137 = fsub fast float %135, %136
  %138 = fmul fast float %133, %109
  %139 = fmul fast float %134, %108
  %140 = fadd fast float %139, %138
  %141 = add i32 %17, -1
  %142 = and i32 %27, %141
  %143 = extractvalue %dx.types.CBufRet.i32 %6, 3
  %144 = icmp eq i32 %143, 1
  %145 = select i1 %144, float 0x401921FB60000000, float 0xC01921FB60000000
  %146 = uitofp i32 %142 to float
  %147 = fmul fast float %145, %146
  %148 = uitofp i32 %17 to float
  %149 = fdiv fast float %147, %148
  %150 = call float @dx.op.unary.f32(i32 12, float %149)  ; Cos(value)
  %151 = call float @dx.op.unary.f32(i32 13, float %149)  ; Sin(value)
  %152 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %4, i32 4)  ; CBufferLoadLegacy(handle,regIndex)
  %153 = extractvalue %dx.types.CBufRet.i32 %152, 2
  %154 = mul i32 %153, %8
  %155 = extractvalue %dx.types.CBufRet.i32 %152, 3
  %156 = add i32 %154, %155
  %157 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %4, i32 7)  ; CBufferLoadLegacy(handle,regIndex)
  %158 = extractvalue %dx.types.CBufRet.f32 %157, 2
  %159 = fmul fast float %150, %137
  %160 = fmul fast float %151, %140
  %161 = fsub fast float %86, %87
  %162 = fadd fast float %161, %159
  %163 = fsub fast float %162, %160
  %164 = fmul fast float %158, %163
  %165 = fmul fast float %150, %140
  %166 = fmul fast float %151, %137
  %167 = fadd fast float %89, %88
  %168 = fadd fast float %167, %165
  %169 = fadd fast float %168, %166
  %170 = fmul fast float %158, %169
  %171 = extractvalue %dx.types.CBufRet.f32 %157, 1
  %172 = fcmp fast oeq float %171, 0.000000e+00
  br i1 %172, label %173, label %176, !dx.controlflow.hints !18

; <label>:173                                     ; preds = %132
  %174 = fptrunc float %164 to half
  call void @dx.op.rawBufferStore.f16(i32 140, %dx.types.Handle %2, i32 %154, i32 0, half %174, half undef, half undef, half undef, i8 1, i32 2)  ; RawBufferStore(uav,index,elementOffset,value0,value1,value2,value3,mask,alignment)
  %175 = fptrunc float %170 to half
  call void @dx.op.rawBufferStore.f16(i32 140, %dx.types.Handle %2, i32 %156, i32 0, half %175, half undef, half undef, half undef, i8 1, i32 2)  ; RawBufferStore(uav,index,elementOffset,value0,value1,value2,value3,mask,alignment)
  br label %190

; <label>:176                                     ; preds = %132
  %177 = fmul fast float %146, %146
  %178 = fmul fast float %177, 0x400921FB60000000
  %179 = fdiv fast float %178, %171
  %180 = call float @dx.op.unary.f32(i32 12, float %179)  ; Cos(value)
  %181 = call float @dx.op.unary.f32(i32 13, float %179)  ; Sin(value)
  %182 = fmul fast float %180, %164
  %183 = fmul fast float %181, %170
  %184 = fsub fast float %182, %183
  %185 = fptrunc float %184 to half
  call void @dx.op.rawBufferStore.f16(i32 140, %dx.types.Handle %2, i32 %154, i32 0, half %185, half undef, half undef, half undef, i8 1, i32 2)  ; RawBufferStore(uav,index,elementOffset,value0,value1,value2,value3,mask,alignment)
  %186 = fmul fast float %181, %164
  %187 = fmul fast float %180, %170
  %188 = fadd fast float %186, %187
  %189 = fptrunc float %188 to half
  call void @dx.op.rawBufferStore.f16(i32 140, %dx.types.Handle %2, i32 %156, i32 0, half %189, half undef, half undef, half undef, i8 1, i32 2)  ; RawBufferStore(uav,index,elementOffset,value0,value1,value2,value3,mask,alignment)
  br label %190

; <label>:190                                     ; preds = %176, %173, %0
  ret void
}

; Function Attrs: nounwind readnone
declare i32 @dx.op.threadId.i32(i32, i32) #0

; Function Attrs: nounwind readnone
declare float @dx.op.unary.f32(i32, float) #0

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.f16 @dx.op.rawBufferLoad.f16(i32, %dx.types.Handle, i32, i32, i8, i32) #1

; Function Attrs: nounwind
declare void @dx.op.rawBufferStore.f16(i32, %dx.types.Handle, i32, i32, half, half, half, half, i8, i32) #2

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #1

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!2}
!dx.shaderModel = !{!3}
!dx.resources = !{!4}
!dx.entryPoints = !{!11}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 2}
!2 = !{i32 1, i32 6}
!3 = !{!"cs", i32 6, i32 2}
!4 = !{null, !5, !9, null}
!5 = !{!6, !7, !8}
!6 = !{i32 0, %"class.RWStructuredBuffer<half>"* undef, !"", i32 0, i32 0, i32 1, i32 12, i1 false, i1 false, i1 false, !1}
!7 = !{i32 1, %"class.RWStructuredBuffer<half>"* undef, !"", i32 0, i32 1, i32 1, i32 12, i1 false, i1 false, i1 false, !1}
!8 = !{i32 2, %"class.RWStructuredBuffer<half>"* undef, !"", i32 0, i32 2, i32 1, i32 12, i1 false, i1 false, i1 false, !1}
!9 = !{!10}
!10 = !{i32 0, %Constants* undef, !"", i32 0, i32 0, i32 1, i32 128, null}
!11 = !{void ()* @DFT, !"DFT", null, !4, !12}
!12 = !{i32 0, i64 8388656, i32 4, !13}
!13 = !{i32 64, i32 1, i32 1}
!14 = distinct !{!14, !"dx.controlflow.hints", i32 2}
!15 = distinct !{!15, !"dx.controlflow.hints", i32 1}
!16 = distinct !{!16, !"dx.controlflow.hints", i32 2}
!17 = distinct !{!17, !"dx.controlflow.hints", i32 1}
!18 = distinct !{!18, !"dx.controlflow.hints", i32 1}

#endif

const unsigned char g_DFT[] = {
  0x44, 0x58, 0x42, 0x43, 0x12, 0x40, 0x8a, 0x15, 0xf2, 0x7d, 0x33, 0xd8,
  0x35, 0x6a, 0x11, 0xd5, 0x43, 0xa1, 0x29, 0x3b, 0x01, 0x00, 0x00, 0x00,
  0x3c, 0x0c, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x48, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
  0x18, 0x01, 0x00, 0x00, 0x34, 0x01, 0x00, 0x00, 0x53, 0x46, 0x49, 0x30,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x49, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x4f, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x53, 0x56, 0x30,
  0xa8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x48, 0x41, 0x53, 0x48, 0x14, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x6a, 0x1d, 0x88, 0xfe, 0xb1, 0x41, 0x77, 0x83,
  0x2f, 0x5e, 0xe4, 0x9c, 0xa3, 0x30, 0xc5, 0x49, 0x44, 0x58, 0x49, 0x4c,
  0x00, 0x0b, 0x00, 0x00, 0x62, 0x00, 0x05, 0x00, 0xc0, 0x02, 0x00, 0x00,
  0x44, 0x58, 0x49, 0x4c, 0x02, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0xe8, 0x0a, 0x00, 0x00, 0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00,
  0xb7, 0x02, 0x00, 0x00, 0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x13, 0x00, 0x00, 0x00, 0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49,
  0x06, 0x10, 0x32, 0x39, 0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19,
  0x1e, 0x04, 0x8b, 0x62, 0x80, 0x18, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42,
  0xc4, 0x10, 0x32, 0x14, 0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x62, 0x88,
  0x48, 0x90, 0x14, 0x20, 0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42,
  0xe4, 0x48, 0x0e, 0x90, 0x11, 0x23, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c,
  0xe1, 0x83, 0xe5, 0x8a, 0x04, 0x31, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07,
  0x40, 0x02, 0xa8, 0x0d, 0x86, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20,
  0x01, 0xd5, 0x06, 0x62, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0x90, 0x00,
  0x49, 0x18, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42,
  0x20, 0x4c, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00,
  0x41, 0x00, 0x00, 0x00, 0x32, 0x22, 0x88, 0x09, 0x20, 0x64, 0x85, 0x04,
  0x13, 0x23, 0xa4, 0x84, 0x04, 0x13, 0x23, 0xe3, 0x84, 0xa1, 0x90, 0x14,
  0x12, 0x4c, 0x8c, 0x8c, 0x0b, 0x84, 0xc4, 0x4c, 0x10, 0x8c, 0xc1, 0x08,
  0x40, 0x09, 0x00, 0x0a, 0xe6, 0x08, 0xc0, 0xa0, 0x0c, 0xc3, 0x30, 0x10,
  0x31, 0x03, 0x50, 0x06, 0x63, 0x30, 0xe8, 0x18, 0x05, 0xb8, 0x69, 0xb8,
  0xfc, 0x09, 0x7b, 0x08, 0xc9, 0x5f, 0x09, 0x69, 0x25, 0x26, 0xbf, 0xa8,
  0x75, 0x54, 0x24, 0x49, 0x92, 0x0c, 0x73, 0x04, 0x08, 0x2d, 0xf7, 0x0c,
  0x97, 0x3f, 0x61, 0x0f, 0x21, 0xf9, 0x21, 0xd0, 0x0c, 0x0b, 0x81, 0x02,
  0xa6, 0x1c, 0xca, 0xd0, 0x0c, 0xc3, 0x32, 0x90, 0x53, 0x16, 0x60, 0x68,
  0x86, 0x21, 0x49, 0x92, 0x64, 0x19, 0x08, 0x3a, 0x6a, 0xb8, 0xfc, 0x09,
  0x7b, 0x08, 0xc9, 0xe7, 0x36, 0xaa, 0x58, 0x89, 0xc9, 0x47, 0x6e, 0x1b,
  0x11, 0xc3, 0x30, 0x0c, 0x85, 0x90, 0x86, 0x66, 0xa0, 0xe9, 0xa8, 0xe1,
  0xf2, 0x27, 0xec, 0x21, 0x24, 0x9f, 0xdb, 0xa8, 0x62, 0x25, 0x26, 0xbf,
  0xb8, 0x6d, 0x44, 0x18, 0x86, 0x61, 0x14, 0xa2, 0x1a, 0x9a, 0x81, 0xac,
  0x39, 0x82, 0xa0, 0x18, 0xcd, 0xb0, 0x0c, 0x03, 0x46, 0xd9, 0x40, 0xc0,
  0x4c, 0xde, 0x38, 0xb0, 0x43, 0x38, 0xcc, 0xc3, 0x3c, 0xb8, 0x81, 0x2c,
  0xdc, 0xc2, 0x2c, 0xd0, 0x83, 0x3c, 0xd4, 0xc3, 0x38, 0xd0, 0x43, 0x3d,
  0xc8, 0x43, 0x39, 0x90, 0x83, 0x28, 0xd4, 0x83, 0x39, 0x98, 0x43, 0x39,
  0xc8, 0x03, 0x1f, 0xa0, 0x43, 0x38, 0xb0, 0x83, 0x39, 0xf8, 0x01, 0x0a,
  0x12, 0xe2, 0x86, 0x11, 0x88, 0xe1, 0x12, 0xce, 0x69, 0xa4, 0x09, 0x68,
  0x26, 0x09, 0x39, 0xc3, 0x30, 0x0c, 0x9e, 0xe7, 0x79, 0x9e, 0x37, 0x18,
  0xc6, 0x40, 0xdf, 0x1c, 0x01, 0x28, 0x4c, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x13, 0x14, 0x72, 0xc0, 0x87, 0x74, 0x60, 0x87, 0x36, 0x68, 0x87, 0x79,
  0x68, 0x03, 0x72, 0xc0, 0x87, 0x0d, 0xae, 0x50, 0x0e, 0x6d, 0xd0, 0x0e,
  0x7a, 0x50, 0x0e, 0x6d, 0x00, 0x0f, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x71, 0xa0, 0x07, 0x73, 0x20, 0x07,
  0x6d, 0x90, 0x0e, 0x78, 0xa0, 0x07, 0x78, 0xd0, 0x06, 0xe9, 0x10, 0x07,
  0x76, 0xa0, 0x07, 0x71, 0x60, 0x07, 0x6d, 0x90, 0x0e, 0x73, 0x20, 0x07,
  0x7a, 0x30, 0x07, 0x72, 0xd0, 0x06, 0xe9, 0x60, 0x07, 0x74, 0xa0, 0x07,
  0x76, 0x40, 0x07, 0x6d, 0x60, 0x0e, 0x71, 0x60, 0x07, 0x7a, 0x10, 0x07,
  0x76, 0xd0, 0x06, 0xe6, 0x30, 0x07, 0x72, 0xa0, 0x07, 0x73, 0x20, 0x07,
  0x6d, 0x60, 0x0e, 0x76, 0x40, 0x07, 0x7a, 0x60, 0x07, 0x74, 0xd0, 0x06,
  0xee, 0x80, 0x07, 0x7a, 0x10, 0x07, 0x76, 0xa0, 0x07, 0x73, 0x20, 0x07,
  0x7a, 0x60, 0x07, 0x74, 0x30, 0xe4, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xc8, 0x43, 0x00, 0x01, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x90, 0xe7, 0x00, 0x02, 0x20,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x21, 0x8f, 0x03, 0x04,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x1e, 0x08,
  0x08, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x3c,
  0x13, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x79, 0x2c, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0xf2, 0x64, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x90, 0x05, 0x02, 0x0c, 0x00, 0x00, 0x00, 0x32, 0x1e, 0x98, 0x14,
  0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47, 0xc6, 0x04, 0x43, 0x1a,
  0x4a, 0xa0, 0x08, 0x8a, 0x61, 0x04, 0xa0, 0x30, 0x0a, 0x50, 0xa0, 0x10,
  0x0a, 0x30, 0x80, 0xb0, 0x11, 0x00, 0x0a, 0x0b, 0x1c, 0x10, 0x10, 0x81,
  0xc0, 0x19, 0x00, 0xea, 0x66, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x52, 0x00, 0x00, 0x00, 0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0x44,
  0x35, 0x18, 0x63, 0x0b, 0x73, 0x3b, 0x03, 0xb1, 0x2b, 0x93, 0x9b, 0x4b,
  0x7b, 0x73, 0x03, 0x99, 0x71, 0xb9, 0x01, 0x41, 0xa1, 0x0b, 0x3b, 0x9b,
  0x7b, 0x91, 0x2a, 0x62, 0x2a, 0x0a, 0x9a, 0x2a, 0xfa, 0x9a, 0xb9, 0x81,
  0x79, 0x31, 0x4b, 0x73, 0x0b, 0x63, 0x4b, 0xd9, 0x10, 0x04, 0x13, 0x84,
  0x01, 0x99, 0x20, 0x0c, 0xc9, 0x06, 0x61, 0x20, 0x26, 0x08, 0x83, 0xb2,
  0x41, 0x18, 0x0c, 0x0a, 0x63, 0x73, 0x1b, 0x06, 0xc4, 0x20, 0x26, 0x08,
  0xc3, 0x32, 0x41, 0xe8, 0x26, 0x02, 0x13, 0x84, 0x81, 0x99, 0x20, 0x60,
  0xd0, 0x86, 0x45, 0x59, 0x18, 0x45, 0x19, 0x1a, 0xc7, 0x71, 0x8a, 0x0d,
  0xcb, 0xb0, 0x30, 0xca, 0x30, 0x34, 0x8e, 0xe3, 0x14, 0x1b, 0x16, 0x62,
  0x61, 0x14, 0x62, 0x68, 0x1c, 0xc7, 0x29, 0x36, 0x0c, 0x0f, 0x14, 0x4d,
  0x10, 0xc0, 0x40, 0x9a, 0x20, 0x0c, 0xcd, 0x06, 0x44, 0x99, 0x18, 0x45,
  0x19, 0x28, 0x60, 0x43, 0x50, 0x6d, 0x20, 0x00, 0xc9, 0x02, 0x26, 0x08,
  0x02, 0xc0, 0x81, 0xc8, 0x88, 0x6a, 0x82, 0x10, 0x06, 0xd1, 0x04, 0x61,
  0x70, 0x26, 0x08, 0xc3, 0xb3, 0x61, 0xe0, 0x86, 0x61, 0x03, 0xa1, 0x68,
  0x5b, 0xb7, 0xa1, 0xc0, 0x32, 0xe0, 0xf2, 0x48, 0x91, 0xe1, 0xb9, 0x8c,
  0xbd, 0xb9, 0xd1, 0xc9, 0xbd, 0xb1, 0x99, 0xb1, 0xbd, 0xdd, 0xb9, 0xa0,
  0xa5, 0xb9, 0xd1, 0xcd, 0xad, 0x18, 0xc2, 0x00, 0x0c, 0x88, 0x15, 0x83,
  0x18, 0x80, 0xc1, 0xb0, 0x62, 0x18, 0x03, 0x30, 0x20, 0x56, 0x0c, 0x64,
  0x00, 0x06, 0xc3, 0x8a, 0xa1, 0x0c, 0xc0, 0x60, 0xa8, 0xc2, 0xc6, 0x66,
  0xd7, 0xe6, 0x92, 0x46, 0x56, 0xe6, 0x46, 0x37, 0x25, 0x08, 0xaa, 0x90,
  0xe1, 0xb9, 0xd8, 0x95, 0xc9, 0xcd, 0xa5, 0xbd, 0xb9, 0x4d, 0x09, 0x88,
  0x26, 0x64, 0x78, 0x2e, 0x76, 0x61, 0x6c, 0x76, 0x65, 0x72, 0x53, 0x02,
  0xa3, 0x0e, 0x19, 0x9e, 0xcb, 0x1c, 0x5a, 0x18, 0x59, 0x99, 0x5c, 0xd3,
  0x1b, 0x59, 0x19, 0xdb, 0x94, 0x00, 0x29, 0x43, 0x86, 0xe7, 0x22, 0x57,
  0x36, 0xf7, 0x56, 0x27, 0x37, 0x56, 0x36, 0x37, 0x25, 0xb0, 0xea, 0x90,
  0xe1, 0xb9, 0x94, 0xb9, 0xd1, 0xc9, 0xe5, 0x41, 0xbd, 0xa5, 0xb9, 0xd1,
  0xcd, 0x4d, 0x09, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x59, 0x00, 0x00, 0x00, 0x33, 0x08, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0x66,
  0x14, 0x01, 0x3d, 0x88, 0x43, 0x38, 0x84, 0xc3, 0x8c, 0x42, 0x80, 0x07,
  0x79, 0x78, 0x07, 0x73, 0x98, 0x71, 0x0c, 0xe6, 0x00, 0x0f, 0xed, 0x10,
  0x0e, 0xf4, 0x80, 0x0e, 0x33, 0x0c, 0x42, 0x1e, 0xc2, 0xc1, 0x1d, 0xce,
  0xa1, 0x1c, 0x66, 0x30, 0x05, 0x3d, 0x88, 0x43, 0x38, 0x84, 0x83, 0x1b,
  0xcc, 0x03, 0x3d, 0xc8, 0x43, 0x3d, 0x8c, 0x03, 0x3d, 0xcc, 0x78, 0x8c,
  0x74, 0x70, 0x07, 0x7b, 0x08, 0x07, 0x79, 0x48, 0x87, 0x70, 0x70, 0x07,
  0x7a, 0x70, 0x03, 0x76, 0x78, 0x87, 0x70, 0x20, 0x87, 0x19, 0xcc, 0x11,
  0x0e, 0xec, 0x90, 0x0e, 0xe1, 0x30, 0x0f, 0x6e, 0x30, 0x0f, 0xe3, 0xf0,
  0x0e, 0xf0, 0x50, 0x0e, 0x33, 0x10, 0xc4, 0x1d, 0xde, 0x21, 0x1c, 0xd8,
  0x21, 0x1d, 0xc2, 0x61, 0x1e, 0x66, 0x30, 0x89, 0x3b, 0xbc, 0x83, 0x3b,
  0xd0, 0x43, 0x39, 0xb4, 0x03, 0x3c, 0xbc, 0x83, 0x3c, 0x84, 0x03, 0x3b,
  0xcc, 0xf0, 0x14, 0x76, 0x60, 0x07, 0x7b, 0x68, 0x07, 0x37, 0x68, 0x87,
  0x72, 0x68, 0x07, 0x37, 0x80, 0x87, 0x70, 0x90, 0x87, 0x70, 0x60, 0x07,
  0x76, 0x28, 0x07, 0x76, 0xf8, 0x05, 0x76, 0x78, 0x87, 0x77, 0x80, 0x87,
  0x5f, 0x08, 0x87, 0x71, 0x18, 0x87, 0x72, 0x98, 0x87, 0x79, 0x98, 0x81,
  0x2c, 0xee, 0xf0, 0x0e, 0xee, 0xe0, 0x0e, 0xf5, 0xc0, 0x0e, 0xec, 0x30,
  0x03, 0x62, 0xc8, 0xa1, 0x1c, 0xe4, 0xa1, 0x1c, 0xcc, 0xa1, 0x1c, 0xe4,
  0xa1, 0x1c, 0xdc, 0x61, 0x1c, 0xca, 0x21, 0x1c, 0xc4, 0x81, 0x1d, 0xca,
  0x61, 0x06, 0xd6, 0x90, 0x43, 0x39, 0xc8, 0x43, 0x39, 0x98, 0x43, 0x39,
  0xc8, 0x43, 0x39, 0xb8, 0xc3, 0x38, 0x94, 0x43, 0x38, 0x88, 0x03, 0x3b,
  0x94, 0xc3, 0x2f, 0xbc, 0x83, 0x3c, 0xfc, 0x82, 0x3b, 0xd4, 0x03, 0x3b,
  0xb0, 0xc3, 0x0c, 0xc4, 0x21, 0x07, 0x7c, 0x70, 0x03, 0x7a, 0x28, 0x87,
  0x76, 0x80, 0x87, 0x19, 0xd5, 0x43, 0x0e, 0xf8, 0xe0, 0x06, 0xe3, 0xf0,
  0x0e, 0xee, 0x40, 0x0f, 0xf2, 0xf0, 0x0e, 0xec, 0x60, 0x0e, 0xec, 0xf0,
  0x0e, 0xf7, 0xe0, 0x06, 0xe8, 0x90, 0x0e, 0xee, 0x40, 0x0f, 0xf3, 0x30,
  0x23, 0x82, 0xc8, 0x01, 0x1f, 0xdc, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0xdc,
  0xc0, 0x1e, 0xc2, 0x41, 0x1e, 0xd8, 0x21, 0x1c, 0xf2, 0xe1, 0x1d, 0xea,
  0x81, 0x1e, 0x66, 0x30, 0x11, 0x39, 0xe0, 0x83, 0x1b, 0x88, 0x83, 0x3c,
  0x94, 0x43, 0x38, 0xac, 0x83, 0x1b, 0x88, 0x83, 0x3c, 0x00, 0x00, 0x00,
  0x71, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x06, 0x30, 0x74, 0x5f,
  0x6b, 0x06, 0xdb, 0x70, 0xf9, 0xce, 0xe3, 0x0b, 0x01, 0x55, 0x14, 0x44,
  0x54, 0x3a, 0xc0, 0x50, 0x12, 0x06, 0x20, 0x60, 0x7e, 0x71, 0xdb, 0x56,
  0xb0, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x50, 0x45, 0x41, 0x44, 0xa5,
  0x03, 0x0c, 0x25, 0x61, 0x00, 0x02, 0xe6, 0x23, 0xb7, 0x6d, 0x07, 0xd2,
  0x70, 0xf9, 0xce, 0xe3, 0x0b, 0x11, 0x01, 0x4c, 0x44, 0x08, 0x34, 0xc3,
  0x42, 0xd8, 0xc0, 0x35, 0x5c, 0xbe, 0xf3, 0xf8, 0x11, 0x60, 0x6d, 0x54,
  0x51, 0x10, 0x51, 0xe9, 0x00, 0x83, 0x5f, 0xd4, 0xba, 0x11, 0x60, 0xc3,
  0xe5, 0x3b, 0x8f, 0x1f, 0x01, 0xd6, 0x46, 0x15, 0x05, 0x11, 0xb1, 0x93,
  0x13, 0x11, 0x7e, 0x51, 0xeb, 0x16, 0x20, 0x0d, 0x97, 0xef, 0x3c, 0xfe,
  0x74, 0x44, 0x04, 0x30, 0x88, 0x83, 0x8f, 0xdc, 0xb6, 0x09, 0x3c, 0xc3,
  0xe5, 0x3b, 0x8f, 0x4f, 0x35, 0x40, 0x84, 0xf9, 0xc5, 0x6d, 0x03, 0x00,
  0x61, 0x20, 0x00, 0x00, 0x2f, 0x01, 0x00, 0x00, 0x13, 0x04, 0x51, 0x2c,
  0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x34, 0x94, 0x5d, 0x59,
  0x0a, 0x94, 0x5c, 0x61, 0x0a, 0x94, 0x4f, 0x39, 0xd4, 0x40, 0x69, 0x94,
  0x6e, 0x40, 0x19, 0x94, 0x02, 0x2d, 0x45, 0x50, 0x02, 0x64, 0x8c, 0x11,
  0xec, 0xfe, 0x28, 0xb3, 0x60, 0x30, 0x46, 0xb0, 0xfb, 0xa3, 0xcc, 0x82,
  0xc3, 0x18, 0xc1, 0xee, 0x8f, 0x32, 0x09, 0x06, 0x94, 0xcc, 0x00, 0x90,
  0x31, 0x46, 0x00, 0x82, 0x20, 0x88, 0x7f, 0x23, 0x00, 0x00, 0x00, 0x00,
  0x23, 0x06, 0x09, 0x00, 0x82, 0x60, 0x90, 0x81, 0x41, 0x74, 0x78, 0xde,
  0x35, 0x62, 0x90, 0x00, 0x20, 0x08, 0x06, 0x59, 0x18, 0x48, 0x08, 0x18,
  0x80, 0x01, 0x36, 0x62, 0x90, 0x00, 0x20, 0x08, 0x06, 0x99, 0x18, 0x4c,
  0x89, 0xe7, 0x65, 0x23, 0x06, 0x09, 0x00, 0x82, 0x60, 0x90, 0x8d, 0x01,
  0xb5, 0x7c, 0x9f, 0x36, 0x62, 0x60, 0x00, 0x20, 0x08, 0x06, 0x84, 0x1a,
  0x3c, 0x60, 0x30, 0x62, 0x70, 0x00, 0x20, 0x08, 0x06, 0xd3, 0x19, 0x60,
  0x42, 0x18, 0x8c, 0x26, 0x04, 0x40, 0x05, 0x03, 0x8c, 0x26, 0x0c, 0xc1,
  0x70, 0x83, 0x10, 0x90, 0xc1, 0x2c, 0x43, 0x00, 0x05, 0x23, 0x06, 0x07,
  0x00, 0x82, 0x60, 0x30, 0xb1, 0x41, 0x77, 0x60, 0xa3, 0x09, 0xc1, 0x50,
  0xc1, 0x1a, 0xe0, 0x68, 0x02, 0x22, 0x54, 0xd0, 0x69, 0xb9, 0x41, 0x70,
  0x35, 0x7c, 0x50, 0x01, 0x18, 0xa8, 0xc5, 0x41, 0x70, 0x81, 0x11, 0x83,
  0x03, 0x00, 0x41, 0x30, 0x98, 0xea, 0xc0, 0x0c, 0xa0, 0x6e, 0x34, 0x21,
  0x08, 0x46, 0x13, 0x04, 0xa1, 0x02, 0x41, 0x0a, 0x0a, 0xaa, 0x22, 0x61,
  0x4a, 0x20, 0xa6, 0x86, 0xa2, 0x4a, 0x68, 0xb0, 0x82, 0xe5, 0x6a, 0x51,
  0x03, 0xa8, 0x22, 0xd0, 0x1a, 0x02, 0xa8, 0x80, 0x82, 0xd1, 0x84, 0x0b,
  0x18, 0x6e, 0x08, 0x42, 0x01, 0x0c, 0x46, 0x0c, 0x0e, 0x00, 0x04, 0xc1,
  0x60, 0x2a, 0x05, 0x3b, 0x00, 0x03, 0x36, 0x18, 0x4d, 0x08, 0x84, 0xe1,
  0x06, 0x23, 0x20, 0x83, 0x22, 0x02, 0x9d, 0x65, 0x10, 0x88, 0x60, 0xc4,
  0xe0, 0x00, 0x40, 0x10, 0x0c, 0xa6, 0x54, 0xd0, 0x03, 0x32, 0x20, 0x85,
  0xd1, 0x84, 0x40, 0xa8, 0x40, 0x91, 0x11, 0x03, 0x05, 0x00, 0x41, 0x30,
  0x70, 0x5c, 0x81, 0x0f, 0xd4, 0x20, 0x30, 0x85, 0x38, 0x40, 0x85, 0xd1,
  0x84, 0x00, 0xb8, 0xc0, 0xc0, 0xd1, 0x04, 0x65, 0x18, 0x6e, 0x08, 0x58,
  0x01, 0x0c, 0x66, 0x19, 0x06, 0x22, 0x18, 0x4d, 0x40, 0x86, 0x0a, 0x0e,
  0x18, 0x31, 0x50, 0x00, 0x10, 0x04, 0x03, 0xa7, 0x16, 0x46, 0x21, 0x0e,
  0x82, 0x56, 0xc0, 0x83, 0x57, 0x18, 0x4d, 0x08, 0x80, 0x0b, 0x0c, 0x9c,
  0x25, 0x20, 0x06, 0x3a, 0x0c, 0x68, 0x80, 0x04, 0x76, 0x08, 0x06, 0x3a,
  0x0c, 0x62, 0x60, 0x07, 0x81, 0x1d, 0x82, 0x11, 0x83, 0x03, 0x00, 0x41,
  0x30, 0x98, 0x70, 0x21, 0x15, 0xe6, 0xa0, 0x16, 0x46, 0x13, 0x82, 0x60,
  0xb8, 0x21, 0x0b, 0xc8, 0x60, 0x96, 0xa1, 0x38, 0x82, 0x11, 0x83, 0x03,
  0x00, 0x41, 0x30, 0x98, 0x76, 0x81, 0x15, 0xec, 0xe0, 0x16, 0x46, 0x13,
  0x02, 0xa0, 0x82, 0x31, 0x90, 0xd1, 0x84, 0x21, 0xa8, 0xc0, 0x93, 0x0a,
  0x06, 0x18, 0x4d, 0x30, 0x84, 0x0a, 0xcc, 0x40, 0x6a, 0x08, 0x60, 0xc4,
  0x40, 0x01, 0x40, 0x10, 0x0c, 0x1c, 0x73, 0xa0, 0x05, 0x50, 0x08, 0x7c,
  0x21, 0x15, 0xc0, 0x61, 0x34, 0x21, 0x00, 0x2e, 0x30, 0x70, 0x34, 0xe1,
  0x19, 0x86, 0x1b, 0x02, 0x72, 0x00, 0x83, 0x59, 0x06, 0xe3, 0x08, 0x46,
  0x13, 0x9c, 0xa1, 0x82, 0x03, 0x46, 0x0c, 0x14, 0x00, 0x04, 0xc1, 0xc0,
  0x69, 0x87, 0x5d, 0x38, 0x85, 0xa0, 0x1c, 0x60, 0xe1, 0x1c, 0x46, 0x13,
  0x02, 0xe0, 0x02, 0x03, 0x67, 0x09, 0x8e, 0x81, 0x0e, 0x03, 0x32, 0xa0,
  0x42, 0x27, 0x88, 0x81, 0x0e, 0x83, 0x30, 0x78, 0xa2, 0xe0, 0x09, 0xc2,
  0x04, 0x4d, 0x3e, 0x26, 0x68, 0xf2, 0x31, 0x62, 0x93, 0x8f, 0x11, 0x9d,
  0x7c, 0x86, 0x1b, 0xe4, 0xc0, 0x0d, 0xc8, 0xa0, 0xe2, 0x20, 0xd0, 0x59,
  0x06, 0x44, 0x09, 0x46, 0x0c, 0x0e, 0x00, 0x04, 0xc1, 0x60, 0xb2, 0x87,
  0x73, 0x88, 0x85, 0x78, 0x18, 0x4d, 0x08, 0x84, 0x0a, 0xec, 0x40, 0x46,
  0x0c, 0x14, 0x00, 0x04, 0xc1, 0xc0, 0xd9, 0x87, 0x74, 0xb8, 0x85, 0x60,
  0x1e, 0x7c, 0xa1, 0x1e, 0x46, 0x13, 0x02, 0xe0, 0x02, 0x03, 0x47, 0x13,
  0xee, 0x60, 0x18, 0x6e, 0x08, 0xf2, 0x01, 0x0c, 0x66, 0x19, 0x12, 0x25,
  0x18, 0x4d, 0x40, 0x86, 0x0a, 0x0e, 0x18, 0x31, 0x50, 0x00, 0x10, 0x04,
  0x03, 0x47, 0x24, 0xe0, 0xc1, 0x17, 0x02, 0x7d, 0x28, 0x07, 0x7e, 0x18,
  0x4d, 0x08, 0x80, 0x0b, 0x0c, 0x9c, 0x25, 0x50, 0x06, 0x3a, 0x0c, 0x28,
  0x81, 0x10, 0xd3, 0x38, 0x06, 0x3a, 0x0c, 0x22, 0x31, 0x0d, 0xc4, 0x34,
  0x8e, 0xe1, 0x86, 0x51, 0x60, 0x03, 0x32, 0x98, 0x65, 0x58, 0x9a, 0x60,
  0xc4, 0xe0, 0x00, 0x40, 0x10, 0x0c, 0x26, 0x93, 0xb8, 0x87, 0x70, 0x10,
  0x89, 0xd1, 0x84, 0x00, 0xa8, 0xc0, 0x15, 0x64, 0x34, 0x61, 0x08, 0x2a,
  0x40, 0x05, 0xa9, 0x60, 0x80, 0xd1, 0x04, 0x43, 0xa8, 0x20, 0x16, 0xa4,
  0x86, 0x00, 0x46, 0x0c, 0x14, 0x00, 0x04, 0xc1, 0xc0, 0x89, 0x89, 0x7f,
  0x58, 0x87, 0x20, 0x25, 0xe8, 0x61, 0x25, 0x46, 0x13, 0x02, 0xe0, 0x02,
  0x03, 0x47, 0x13, 0xf4, 0x60, 0x18, 0x6e, 0x08, 0x5e, 0x02, 0x0c, 0x66,
  0x19, 0x98, 0x26, 0x18, 0x4d, 0x70, 0x86, 0x0a, 0x0e, 0x18, 0x31, 0x50,
  0x00, 0x10, 0x04, 0x03, 0x07, 0x27, 0x4c, 0x42, 0x1e, 0x02, 0x98, 0xd8,
  0x07, 0x99, 0x18, 0x4d, 0x08, 0x80, 0x0b, 0x0c, 0x9c, 0x25, 0x68, 0x06,
  0x3a, 0x0c, 0x88, 0x81, 0x16, 0xf8, 0x50, 0x06, 0x3a, 0x0c, 0x82, 0x91,
  0x8f, 0x45, 0x3e, 0x14, 0x13, 0x30, 0xf9, 0x98, 0x80, 0xc9, 0xc7, 0x84,
  0x20, 0x3e, 0x56, 0x68, 0xf2, 0xb1, 0x82, 0x93, 0x8f, 0x05, 0x02, 0x7c,
  0x0a, 0x1e, 0x58, 0x02, 0xea, 0x1c, 0x02, 0x1d, 0x4d, 0xe0, 0x87, 0x61,
  0xb8, 0x21, 0x08, 0x0b, 0x30, 0x98, 0x6e, 0x48, 0x09, 0x94, 0x08, 0x8e,
  0x30, 0xca, 0x84, 0x40, 0x3e, 0x77, 0x0f, 0x46, 0x99, 0x10, 0xd0, 0x67,
  0xc4, 0xc0, 0x00, 0x40, 0x10, 0x0c, 0x8e, 0xb6, 0x18, 0x8b, 0x60, 0xc4,
  0xc0, 0x00, 0x40, 0x10, 0x0c, 0x0e, 0xb7, 0xa8, 0x09, 0x61, 0xc4, 0xe0,
  0x00, 0x40, 0x10, 0x0c, 0x26, 0xb6, 0xe8, 0x89, 0x93, 0x18, 0x8b, 0xd1,
  0x84, 0x40, 0xa8, 0xa0, 0x24, 0x64, 0x34, 0x61, 0x18, 0x4a, 0x08, 0x60,
  0xc4, 0xe0, 0x00, 0x40, 0x10, 0x0c, 0x2c, 0xb8, 0x10, 0x0b, 0x96, 0xe8,
  0x89, 0xd1, 0x84, 0x40, 0xb0, 0xc4, 0x92, 0x8f, 0x25, 0x94, 0x7c, 0xac,
  0x14, 0x48, 0x21, 0x3e, 0x16, 0x0c, 0xf0, 0xb1, 0x60, 0x88, 0x8f, 0x19,
  0x81, 0x7c, 0xec, 0xc9, 0xe4, 0x63, 0x4f, 0x27, 0x1f, 0x43, 0x85, 0x54,
  0x80, 0x8f, 0x05, 0x03, 0x7c, 0x2c, 0x18, 0xe0, 0x63, 0x4c, 0x20, 0x9f,
  0xd1, 0x04, 0x27, 0x18, 0x8e, 0x08, 0x7e, 0x22, 0xf8, 0x66, 0x19, 0x9c,
  0x27, 0xb8, 0x24, 0xb9, 0x11, 0x03, 0x07, 0x00, 0x41, 0x30, 0x80, 0x46,
  0x03, 0x2e, 0x7e, 0x82, 0xe2, 0x8b, 0x60, 0x2c, 0xc6, 0x62, 0x2c, 0xce,
  0xc2, 0x2f, 0x8e, 0x48, 0x6e, 0xc4, 0xc0, 0x01, 0x40, 0x10, 0x0c, 0x20,
  0xd2, 0x88, 0x0b, 0xb0, 0x98, 0xfa, 0x22, 0x20, 0x0b, 0xb2, 0x20, 0x0b,
  0xb4, 0xf8, 0x8b, 0x59, 0x02, 0xc8, 0xba, 0x4e, 0x3e, 0x16, 0x98, 0x85,
  0x7c, 0x2c, 0x30, 0xe8, 0x33, 0x62, 0x60, 0x00, 0x20, 0x08, 0x06, 0x47,
  0x6a, 0xfc, 0x45, 0x30, 0x62, 0x60, 0x00, 0x20, 0x08, 0x06, 0x87, 0x6a,
  0xc4, 0x85, 0x60, 0x02, 0x24, 0x1f, 0x13, 0x16, 0xf9, 0x98, 0x10, 0xc4,
  0xe7, 0x82, 0xe4, 0x46, 0x0c, 0x1c, 0x00, 0x04, 0xc1, 0x00, 0x6a, 0x0d,
  0xbd, 0x48, 0x0b, 0xcf, 0x34, 0x82, 0xb6, 0x68, 0x8b, 0xb6, 0x88, 0x0b,
  0xd4, 0xb0, 0x82, 0x92, 0x8f, 0x1d, 0x8f, 0x7c, 0x4c, 0x08, 0xe0, 0x73,
  0x41, 0x72, 0x23, 0x06, 0x0e, 0x00, 0x82, 0x60, 0x00, 0xc5, 0x86, 0x5f,
  0xb4, 0x05, 0x18, 0xa8, 0x46, 0x10, 0x17, 0x71, 0x11, 0x17, 0x75, 0xc1,
  0x1a, 0xb3, 0x04, 0x10, 0x06, 0xc4, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
  0x5b, 0x06, 0x34, 0x78, 0xc0, 0x60, 0xcb, 0x10, 0x07, 0x4f, 0x18, 0x6c,
  0x19, 0xfa, 0xe0, 0x11, 0x83, 0x2d, 0x03, 0x2b, 0x3c, 0x63, 0xb0, 0x65,
  0xd8, 0x85, 0x07, 0x0c, 0xb6, 0x0c, 0xe4, 0xf0, 0x84, 0xc1, 0x96, 0xc1,
  0x1d, 0x1e, 0x31, 0xd8, 0x32, 0xf4, 0xc3, 0x33, 0x06, 0x5b, 0x06, 0xb6,
  0x78, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
