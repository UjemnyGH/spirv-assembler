/**
 * @file spirva.h
 * @author Piotr UjemnyGH Plombon
 * @brief Single file SPIR-V compiler
 * @version 0.1
 * @date 2025-09-16
 * 
 * 
 * MIT License
 * 
 * Copyright (c) 2025 Piotr Plombon
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * 
 * Library based on: https://registry.khronos.org/SPIR-V/specs/unified1/SPIRV.pdf
 * 
 */

#ifndef _SPIRV_ASSEMBLY_
#define _SPIRV_ASSEMBLY_

#include <stdlib.h>
#include <stdarg.h>

#define sa_malloc malloc
#define sa_calloc calloc
#define sa_realloc realloc
#define sa_free free

#define SA_LITTLE_ENDIAN16(u16) (((u16 << 8) & 0xFF00) | ((u16 >> 8) & 0x00FF))
#define SA_LITTLE_ENDIAN32(u32) (((u32 << 24) & 0xFF000000UL) | ((u32 << 8) & 0x00FF0000UL) | ((u32 >> 8) & 0x0000FF00UL) | ((u32 >> 24) & 0x000000FFUL))

#define SA_SPIRV_MAGIC_NUMBER   0x07230203UL
#define SA_SPIRV_VERSION        0x00010000UL
#define SA_SPIRV_GENERATOR_ID   0x5A5A0001UL

#define SA_NULL 0

#define SA_UINT8_MAX 0xFF
#define SA_UINT16_MAX 0xFFFF
#define SA_UINT32_MAX 0xFFFFFFFFU
#define SA_UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

typedef char   sa_int8_t;
typedef short  sa_int16_t;
typedef int    sa_int32_t;
typedef long   sa_int64_t;

typedef unsigned char  sa_uint8_t;
typedef unsigned short sa_uint16_t;
typedef unsigned int   sa_uint32_t;
typedef unsigned long  sa_uint64_t;

#define SA_TRUE   1
#define SA_FALSE  0

typedef sa_int8_t     sa_bool;

typedef sa_uint32_t*  sa_binary;

// OpSource = Unknown, We dont have source such as GLSL, etc.
#define SA_SHADER_SOURCE 0

// Used by OpEntryPoint
enum sa__OpEntryPoint {
  saOpEntryPoint_Vertex = 0,
  saOpEntryPoint_TessellationControl = 1,
  saOpEntryPoint_TessellationEvaluation = 2,
  saOpEntryPoint_Geometry = 3,
  saOpEntryPoint_Fragment = 4,
  saOpEntryPoint_GLCompute = 5,
  saOpEntryPoint_Kernel = 6,
  saOpEntryPoint_TaskNV = 5267,
  saOpEntryPoint_MeshNV = 5268,
  saOpEntryPoint_RayGenerationKHR = 5313,
  saOpEntryPoint_IntersectionKHR = 5314,
  saOpEntryPoint_AnyHitKHR = 5315,
  saOpEntryPoint_ClosestHitKHR = 5316,
  saOpEntryPoint_MissKHR = 5317,
  saOpEntryPoint_CallableKHR = 5318,
  saOpEntryPoint_TaskEXT = 5364,
  saOpEntryPoint_MeshEXT = 5365,

  // Nvidia raytracing, same as Khronos
  saOpEntryPoint_RayGenerationNV = saOpEntryPoint_RayGenerationKHR,
  saOpEntryPoint_IntersectionNV = saOpEntryPoint_IntersectionKHR,
  saOpEntryPoint_AnyHitNV = saOpEntryPoint_AnyHitKHR,
  saOpEntryPoint_ClosestHitNV = saOpEntryPoint_ClosestHitKHR,
  saOpEntryPoint_MissNV = saOpEntryPoint_MissKHR,
  saOpEntryPoint_CallableNV = saOpEntryPoint_CallableKHR
};

// Used by OpMemoryModel
enum sa__OpAddressingModel {
  saOpAddresingModel_Logical = 0,
  saOpAddresingModel_Physical32 = 1,
  saOpAddresingModel_Physical64 = 2,
  saOpAddresingModel_PhysicalStorageBuffer64 = 5348,
  saOpAddresingModel_PhysicalStorageBuffer64EXT = saOpAddresingModel_PhysicalStorageBuffer64
};

// Used by OpMemoryModel
enum sa__OpMemoryModel {
  saOpMemoryModel_Simple = 0,
  saOpMemoryModel_GLSL450 = 1,
  saOpMemoryModel_OpenCL = 2,
  saOpMemoryModel_Vulkan = 3,
  saOpMemoryModel_VulkanKHR = saOpMemoryModel_Vulkan
};

// Used by OpExecutionMode and OpExecutionModeId
enum sa__OpExecutionMode {
  saOpExecutionMode_Invocations = 0,
  saOpExecutionMode_SpacingEqual = 1,
  saOpExecutionMode_SpacingFractionalEven = 2,
  saOpExecutionMode_SpacingFractionalOdd = 3,
  saOpExecutionMode_VertexOrderCw = 4,
  saOpExecutionMode_VertexOrderCcw = 5,
  saOpExecutionMode_PixelCenterInteger = 6,
  saOpExecutionMode_OriginUpperLeft = 7,
  saOpExecutionMode_OriginLowerLeft = 8,
  saOpExecutionMode_EarlyFragmentTests = 9,
  saOpExecutionMode_PointMode = 10,
  saOpExecutionMode_Xfb = 11,
  saOpExecutionMode_DepthReplacing = 12,
  saOpExecutionMode_DepthGreater = 14,
  saOpExecutionMode_DepthLess = 15,
  saOpExecutionMode_DepthUnchanged = 16,
  saOpExecutionMode_LocalSize = 17,
  saOpExecutionMode_LocalSizeHint = 18,
  saOpExecutionMode_InputPoints = 19,
  saOpExecutionMode_InputLines = 20,
  saOpExecutionMode_InputLinesAdjacency = 21,
  saOpExecutionMode_InputTriangles = 22,
  saOpExecutionMode_InputTrianglesAdjacency = 23,
  saOpExecutionMode_Quads = 24,
  saOpExecutionMode_Isolines = 25,
  saOpExecutionMode_OutputVertices = 26,
  saOpExecutionMode_OutputPoints = 27,
  saOpExecutionMode_OutputLineStrip = 28,
  saOpExecutionMode_OutputTriangleStrip = 29,
  saOpExecutionMode_VecTypeHint = 30,
  saOpExecutionMode_ContractionOff = 31,
  saOpExecutionMode_Initializer = 33,
  saOpExecutionMode_Finalizer = 34,
  saOpExecutionMode_SubgroupSize = 35,
  saOpExecutionMode_SubgroupsPerWorkgroup = 36,
  saOpExecutionMode_SubgroupsPerWorkgroupId = 37,
  saOpExecutionMode_LocalSizeId = 38,
  saOpExecutionMode_LocalSizeHintId = 39,
  saOpExecutionMode_NonCoherentColorAttachmentReadEXT = 4169,
  saOpExecutionMode_NonCoherentDepthAttachmentReadEXT = 4170,
  saOpExecutionMode_NonCoherentStencilAttachmentReadEXT = 4171,
  saOpExecutionMode_SubgroupUniformControlFlowKHR = 4421,
  saOpExecutionMode_PostDepthCoverage = 4446,
  saOpExecutionMode_DenormPreserve = 4459,
  saOpExecutionMode_DenormFlushToZero = 4460,
  saOpExecutionMode_SignedZeroInfNanPreserve = 4461,
  saOpExecutionMode_RoundingModeRTE = 4462,
  saOpExecutionMode_RoundingModeRTZ = 4463,
  saOpExecutionMode_EarlyAndLateFragmentTestsAMD = 5017,
  saOpExecutionMode_StencilRefReplacingEXT = 5027,
  saOpExecutionMode_CoalesaingAMDX = 5069,
  saOpExecutionMode_IsApiEntryAMDX = 5070,
  saOpExecutionMode_MaxNodeRecursionAMDX = 5071,
  saOpExecutionMode_StaticNumWorkgroupsAMDX = 5072,
  saOpExecutionMode_ShaderIndexAMDX = 5073,
  saOpExecutionMode_MaxNumWorkgroupsAMDX = 5077,
  saOpExecutionMode_StencilRefUnchangedFrontAMD = 5079,
  saOpExecutionMode_StencilRefGreaterFrontAMD = 5080,
  saOpExecutionMode_StencilRefLessFrontAMD = 5081,
  saOpExecutionMode_StencilRefUnchangedBackAMD = 5082,
  saOpExecutionMode_StencilRefGreaterBackAMD = 5083,
  saOpExecutionMode_StencilRefLessBackAMD = 5084,
  saOpExecutionMode_QuadDerivativesKHR = 5088,
  saOpExecutionMode_RequireFullQuadsKHR = 5089,
  saOpExecutionMode_ShareInputWithAMDX = 5102,
  saOpExecutionMode_OutputLinesEXT = 5269,
  saOpExecutionMode_OutputPrimitivesEXT = 5270,
  saOpExecutionMode_DerivativeGroupQuadsKHR = 5289,
  saOpExecutionMode_DerivativeGroupLinearKHR = 5290,
  saOpExecutionMode_OutputTrianglesEXT = 5298,
  saOpExecutionMode_PixelInterlockOrderedEXT = 5366,
  saOpExecutionMode_PixelInterlockUnorderedEXT = 5367,
  saOpExecutionMode_SamplerInterlockOrderedEXT = 5368,
  saOpExecutionMode_SamplerInterlockUnorderedEXT = 5369,
  saOpExecutionMode_ShadingRateInterlockOrderedEXT = 5370,
  saOpExecutionMode_ShadingRateInterlockUnorderedEXT = 5371,
  saOpExecutionMode_SharedLocalMemorySizeINTEL = 5618,
  saOpExecutionMode_RoundingModeRTPINTEL = 5620,
  saOpExecutionMode_RoundingModeRTNINTEL = 5621,
  saOpExecutionMode_FloatingPointModeALTINTEL = 5622,
  saOpExecutionMode_FloatingPointModeIEEEINTEL = 5623,
  saOpExecutionMode_MaxWorkgroupSizeINTEL = 5893,
  saOpExecutionMode_MaxWorkDimIMTEL = 5894,
  saOpExecutionMode_NoGlobalOffsetINTEL = 5895,
  saOpExecutionMode_NumSIMDWorkitemsINTEL = 5896,
  saOpExecutionMode_schedulerTargetFmaxMhzINTEL = 5903,
  saOpExecutionMode_MaximallyReconvergesKHR = 6023,
  saOpExecutionMode_FPFastMathDefault = 6028,
  saOpExecutionMode_StreamingInterfaceINTEL = 6154,
  saOpExecutionMode_RegisterMapInterfaceINTEL = 6160,
  saOpExecutionMode_NamedBarrierCountINTEL = 6417,
  saOpExecutionMode_MaximumRegistersINTEL = 6461,
  saOpExecutionMode_MaximumRegistersIdINTEL = 6462,
  saOpExecutionMode_NamedMaximumRegistersINTEL = 6463,

  saOpExecutionMode_OutputLinesNV = saOpExecutionMode_OutputLinesEXT,
  saOpExecutionMode_OutputPrimitivesNV = saOpExecutionMode_OutputPrimitivesEXT,
  saOpExecutionMode_DerivativeGroupQuadsNV = saOpExecutionMode_DerivativeGroupQuadsKHR,
  saOpExecutionMode_DerivativeGroupLinearNV = saOpExecutionMode_DerivativeGroupLinearKHR,
  saOpExecutionMode_OutputTrianglesNV = saOpExecutionMode_OutputTrianglesEXT
};

// Used by OpTypePointer, OpTypeForwardPointer, OpVariable, OpGenericCastToPtrExplicit, OpTypeUndefinedPointerKHR, OpUntypedVariableKHR
enum sa__OpStorageClass {
  saOpStorageClass_UniformConstant = 0,
  saOpStorageClass_Input = 1,
  saOpStorageClass_Uniform = 2,
  saOpStorageClass_Output = 3,
  saOpStorageClass_Workgroup = 4,
  saOpStorageClass_CrossWorkgroup = 5,
  saOpStorageClass_Private = 6,
  saOpStorageClass_Function = 7,
  saOpStorageClass_Generic = 8,
  saOpStorageClass_PushConstant = 9,
  saOpStorageClass_AtomicCounter = 10,
  saOpStorageClass_Image = 11,
  saOpStorageClass_StorageBuffer = 12,
  saOpStorageClass_TileImageEXT = 4172,
  saOpStorageClass_NodePayloadAMDX = 5068,
  saOpStorageClass_CallableDataKHR = 5328,
  saOpStorageClass_IncomingCallableDataKHR = 5329,
  saOpStorageClass_RayPayloadKHR = 5338,
  saOpStorageClass_HitAttributeKHR = 5339,
  saOpStorageClass_IncomingRayPayloadKHR = 5342,
  saOpStorageClass_ShaderRecordBufferKHR = 5343,
  saOpStorageClass_PhysicalStorageBuffer = 5349,
  saOpStorageClass_HitObjectAttributeNV = 5385,
  saOpStorageClass_TaskPayloadWorkgroupEXT = 5402,
  saOpStorageClass_CodeSelectionINTEL = 5605,
  saOpStorageClass_DeviceOnlyINTEL = 5936,
  saOpStorageClass_HostOnlyINTEL = 5937,

  saOpStorageClass_CallableDataNV = saOpStorageClass_CallableDataKHR,
  saOpStorageClass_IncomingCallableDataNV = saOpStorageClass_IncomingCallableDataKHR,
  saOpStorageClass_RayPayloadNV = saOpStorageClass_RayPayloadKHR,
  saOpStorageClass_HitAttributeNV = saOpStorageClass_HitAttributeKHR,
  saOpStorageClass_IncomingRayPayloadNV = saOpStorageClass_IncomingRayPayloadKHR,
  saOpStorageClass_ShaderRecordBufferNV = saOpStorageClass_ShaderRecordBufferKHR,
  saOpStorageClass_PhysicalStorageBufferEXT = saOpStorageClass_PhysicalStorageBuffer
};

// Used by OpTypeImage
enum sa__OpImageDimmension {
  saOpImageDimmension_1D = 0,
  saOpImageDimmension_2D = 1,
  saOpImageDimmension_3D = 2,
  saOpImageDimmension_Cube = 3,
  saOpImageDimmension_Rect = 4,
  saOpImageDimmension_Buffer = 5,
  saOpImageDimmension_SubpassData = 6,
  saOpImageDimmension_TileImageDataEXT = 4173
};

// Used by OpConstantSampler
enum sa__OpSamplerAddressingMode {
  saOpSamplerAddressingMode_None = 0,
  saOpSamplerAddressingMode_ClampToEdge = 1,
  saOpSamplerAddressingMode_Clamp = 2,
  saOpSamplerAddressingMode_Repeat = 3,
  saOpSamplerAddressingMode_RepeatMirrored = 4
};

// Used by OpConstantSampler
enum sa__OpSamplerFilterMode {
  saOpSamplerFilterMode_Nearest = 0,
  saOpSamplerFilterMode_Linear = 1
};

// Used by OpTypeImage
enum sa__OpImageFormat {
  saOpImageFormat_Unknown = 0,
  saOpImageFormat_Rgba32f = 1,
  saOpImageFormat_Rgba16f = 2,
  saOpImageFormat_R32f = 3,
  saOpImageFormat_Rgba8 = 4,
  saOpImageFormat_Rgba8Snorm = 5,
  saOpImageFormat_Rg32f = 6,
  saOpImageFormat_Rg16f = 7,
  saOpImageFormat_R11fG11fB10f = 8,
  saOpImageFormat_R16f = 9,
  saOpImageFormat_Rgba16 = 10,
  saOpImageFormat_Rgb10A2 = 11,
  saOpImageFormat_Rg16 = 12,
  saOpImageFormat_Rg8 = 13,
  saOpImageFormat_R16 = 14,
  saOpImageFormat_R8 = 15,
  saOpImageFormat_Rgba16Snorm = 16,
  saOpImageFormat_Rg16Snorm = 17,
  saOpImageFormat_Rg8Snorm = 18,
  saOpImageFormat_R16Snorm = 19,
  saOpImageFormat_R8Snorm = 20,
  saOpImageFormat_Rgba32i = 21,
  saOpImageFormat_Rgba16i = 22,
  saOpImageFormat_Rgba8i = 23,
  saOpImageFormat_R32i = 24,
  saOpImageFormat_Rg32i = 25,
  saOpImageFormat_Rg16i = 26,
  saOpImageFormat_Rg8i = 27,
  saOpImageFormat_R16i = 28,
  saOpImageFormat_R8i = 29,
  saOpImageFormat_Rgba32ui = 30,
  saOpImageFormat_Rgba16ui = 31,
  saOpImageFormat_Rgba8ui = 32,
  saOpImageFormat_R32ui = 33,
  saOpImageFormat_Rgb10a2ui = 34,
  saOpImageFormat_Rg32ui = 35,
  saOpImageFormat_Rg16ui = 36,
  saOpImageFormat_Rg8ui = 37,
  saOpImageFormat_R16ui = 38,
  saOpImageFormat_R8ui = 39,
  saOpImageFormat_R64ui = 40,
  saOpImageFormat_R64i = 41
};

// Used bu OpImageQueryOrder
enum sa__OpImageChannelOrder {
  saOpImageChannelOrder_R = 0,
  saOpImageChannelOrder_A = 1,
  saOpImageChannelOrder_RG = 2,
  saOpImageChannelOrder_RA = 3,
  saOpImageChannelOrder_RGB = 4,
  saOpImageChannelOrder_RGBA = 5,
  saOpImageChannelOrder_BGRA = 6,
  saOpImageChannelOrder_ARGB = 7,
  saOpImageChannelOrder_Intensity = 8,
  saOpImageChannelOrder_Luminance = 9,
  saOpImageChannelOrder_Rx = 10,
  saOpImageChannelOrder_RGx = 11,
  saOpImageChannelOrder_RGBx = 12,
  saOpImageChannelOrder_Depth = 13,
  saOpImageChannelOrder_DepthStencil = 14,
  saOpImageChannelOrder_sRGB = 15,
  saOpImageChannelOrder_sRGBx = 16,
  saOpImageChannelOrder_sRGBA = 17,
  saOpImageChannelOrder_sBGRA = 18,
  saOpImageChannelOrder_ABGR = 19
};

// Used by OpImageQueryFormat
enum sa__OpImageChannelDataType {
  saOpImageChannelDataFormat_SnormInt8 = 0,
  saOpImageChannelDataFormat_SnormInt16 = 1,
  saOpImageChannelDataFormat_UnormInt8 = 2,
  saOpImageChannelDataFormat_UnormInt16 = 3,
  saOpImageChannelDataFormat_UnormShort565 = 4,
  saOpImageChannelDataFormat_UnormShort555 = 5,
  saOpImageChannelDataFormat_UnormInt101010 = 6,
  saOpImageChannelDataFormat_SignedInt8 = 7,
  saOpImageChannelDataFormat_SignedInt16 = 8,
  saOpImageChannelDataFormat_SignedInt32 = 9,
  saOpImageChannelDataFormat_UnsignedInt8 = 10,
  saOpImageChannelDataFormat_UnsignedInt16 = 11,
  saOpImageChannelDataFormat_UnsignedInt32 = 12,
  saOpImageChannelDataFormat_HalfFloat = 13,
  saOpImageChannelDataFormat_Float = 14,
  saOpImageChannelDataFormat_UnormInt24 = 15,
  saOpImageChannelDataFormat_UnormInt101010_2 = 16,
  saOpImageChannelDataFormat_UnsignedIntRaw10EXT = 19,
  saOpImageChannelDataFormat_UnsignedIntRaw12EXT = 20,
  saOpImageChannelDataFormat_UnormInt2_101010EXT = 21,
  saOpImageChannelDataFormat_UnsignedInt10X6EXT = 22,
  saOpImageChannelDataFormat_UnsignedInt12X4EXT = 23,
  saOpImageChannelDataFormat_UnsignedInt14X2EXT = 24,
  saOpImageChannelDataFormat_UnormInt12X4EXT = 25,
  saOpImageChannelDataFormat_UnormInt14X2EXT = 26
};

// Used by: OpImageSampleImplicitLod, OpImageSampleExplicitLod, OpImageSampleDrefImplicitLod, OpImageSampleDrefExplicitLod, OpImageSampleProjImplicitLod, OpImageSampleProjExplicitLod, 
// OpImageSampleProjDrefImplicitLod, OpImageSampleProjDrefExplicitLod, OpImageFetch, OpImageGather, OpImageDrefGather, OpImageRead, OpImageWrite, OpImageSparseSampleImplicitLod, 
// OpImageSparseSampleExplicitLod, OpImageSparseSampleDrefImplicitLod, OpImageSparseSampleDrefExplicitLod, OpImageSparseFetch, OpImageSparseGather, OpImageSparseDrefGather, OpImageSparseRead, OpImageSampleFootprintNV
enum sa__OpImageOperands {
  saOpImageOperands_None = 0x0,
  saOpImageOperands_Bias = 0x1,
  saOpImageOperands_Lod = 0x2,
  saOpImageOperands_Grad = 0x4,
  saOpImageOperands_ConstOffset = 0x8,
  saOpImageOperands_Offset = 0x10,
  saOpImageOperands_ConstOffsets = 0x20,
  saOpImageOperands_Sample = 0x40,
  saOpImageOperands_MinLod = 0x80,
  saOpImageOperands_MakeTexelAvailable = 0x100,
  saOpImageOperands_MakeTexelVisible = 0x200,
  saOpImageOperands_NonPrivateTexel = 0x400,
  saOpImageOperands_VolatileTexel = 0x800,
  saOpImageOperands_SignExtent = 0x1000,
  saOpImageOperands_ZeroExtent = 0x2000,
  saOpImageOperands_Nontemporal = 0x4000,
  saOpImageOperands_Offsets = 0x10000,
  
  saOpImageOperands_MakeTexelAvailableKHR = saOpImageOperands_MakeTexelAvailable,
  saOpImageOperands_MakeTexelVisibleKHR = saOpImageOperands_MakeTexelVisible,
  saOpImageOperands_NonPrivateTexelKHR = saOpImageOperands_NonPrivateTexel,
  saOpImageOperands_VolatileTexelKHR = saOpImageOperands_VolatileTexel
};

// Used by: OpFAdd, OpFSub, OpFMul, OpFDiv, OpFRem, OpFMod
// SPIR-V 1.6:
// OpFNegate, OpOrdered, OpUnordered, OpFOrdEqual, OpFUnordEqual, OpFOrdNotEqual,
// OpFUnordNotEqual, OpFOrdLessThan, OpFUnordLessThan, OpFOrdGreaterThan,
// OpFUnordGreaterThan, OpFOrdLessThanEqual, OpFUnordLessThanEqual,
// OpFOrdGreaterThanEqual, OpFUnordGreaterThanEqual, OpExtInst
enum sa__OpFPFastMathMode {
  saOpFPFastMathMode_None = 0x0,
  saOpFPFastMathMode_NotNan = 0x1,
  saOpFPFastMathMode_NotInf = 0x2,
  saOpFPFastMathMode_NSZ = 0x4,
  saOpFPFastMathMode_AllowReciprocal = 0x8,
  saOpFPFastMathMode_Fast = 0x10,
  saOpFPFastMathMode_AllowContract = 0x10000,
  saOpFPFastMathMode_AllowReassoc = 0x20000,
  saOpFPFastMathMode_AllowTransform = 0x40000,

  saOpFPFastMathMode_AllowContractINTEL = saOpFPFastMathMode_AllowContract,
  saOpFPFastMathMode_AllowReassocINTEL = saOpFPFastMathMode_AllowReassoc,
};

typedef enum sa__OpCodesCore_e {
  saOp_Nop = 0,
  saOp_Undef = 1,
  saOp_SourceContinued = 2,
  saOp_Source = 3,
  saOp_SourceExtension = 4,
  saOp_Name = 5,
  saOp_MemberName = 6,
  saOp_String = 7,
  saOp_Line = 8,
  saOp_Extension = 10,
  saOp_ExtInstImport = 11,
  saOp_ExtInst = 12,
  saOp_MemoryModel = 14,
  saOp_EntryPoint = 15,
  saOp_ExecutionMode = 16,
  saOp_Capability = 17,
  saOp_TypeVoid = 19,
  saOp_TypeBool = 20,
  saOp_TypeInt = 21,
  saOp_TypeFloat = 22,
  saOp_TypeVector = 23,
  saOp_TypeMatrix = 24,
  saOp_TypeImage = 25,
  saOp_TypeSampler = 26,
  saOp_TypeSampledImage = 27,
  saOp_TypeArray = 28,
  saOp_TypeRuntimeArray = 29,
  saOp_TypeStruct = 30,
  saOp_TypeOpaque = 31,
  saOp_TypePointer = 32,
  saOp_TypeFunction = 33,
  saOp_TypeEvent = 34,
  saOp_TypeDeviceEvent = 35,
  saOp_TypeReserveId = 36,
  saOp_TypeQueue = 37,
  saOp_TypePipe = 38,
  saOp_TypeForwardPointer = 39,
  saOp_ConstantTrue = 41,
  saOp_ConstantFalse = 42,
  saOp_Constant = 43,
  saOp_ConstantComposite = 44,
  saOp_ConstantSampler = 45,
  saOp_ConstantNull = 46,
  saOp_SpecConstantTrue = 48,
  saOp_SpecConstantFalse = 49,
  saOp_SpecConstant = 50,
  saOp_SpecConstantComposite = 51,
  saOp_SpecConstantOp = 52,
  saOp_Function = 54,
  saOp_FunctionParameter = 55,
  saOp_FunctionEnd = 56,
  saOp_FunctionCall = 57,
  saOp_Variable = 59,
  saOp_ImageTexelPointer = 60,
  saOp_Load = 61,
  saOp_Store = 62,
  saOp_CopyMemory = 63,
  saOp_CopyMemorySized = 64,
  saOp_AccessChain = 65,
  saOp_InBoundsAccessChain = 66,
  saOp_PtrAccessChain = 67,
  saOp_ArrayLength = 68,
  saOp_GenericPtrMemSemantics = 69,
  saOp_InBoundsPtrAccessChain = 70,
  saOp_Decorate = 71,
  saOp_MemberDecorate = 72,
  saOp_DecorationGroup = 73,
  saOp_DecorateGroup = 74,
  saOp_GroupMemberDecorate = 75,
  saOp_VectorExtractDynamic = 77,
  saOp_VectorInsertDynamic = 78,
  saOp_VectorShuffle = 79,
  saOp_CompositeConstruct = 80,
  saOp_CompositeExtract = 81,
  saOp_CompositeInsert = 82,
  saOp_CopyObject = 83,
  saOp_Transpose = 84,
  saOp_SampledImage = 86,
  saOp_ImageSampleImplicitLod = 87,
  saOp_ImageSampleExplicitLod = 88,
  saOp_ImageSampleDrefImplicitLod = 89,
  saOp_ImageSampleDrefExplicitLod = 90,
  saOp_ImageSampleProjImplicitLod = 91,
  saOp_ImageSampleProjExplicitLod = 92,
  saOp_ImageSampleProjDrefImplicitLod = 93,
  saOp_ImageSampleProjDrefExplicitLod = 94,
  saOp_ImageFetch = 95,
  saOp_ImageGather = 96,
  saOp_ImageDrefGather = 97,
  saOp_ImageRead = 98,
  saOp_ImageWrite = 99,
  saOp_Image = 100,
  saOp_ImageQueryFormat = 101,
  saOp_ImageQueryOrder = 102,
  saOp_ImageQuerySizeLod = 103,
  saOp_ImageQuerySize = 104,
  saOp_ImageQueryLod = 105,
  saOp_ImageQueryLevels = 106,
  saOp_ImageQuerySamples = 107,
  saOp_ConvertFToU = 109,
  saOp_ConvertFToS = 110,
  saOp_ConvertSToF = 111,
  saOp_ConvertUToF = 112,
  saOp_UConvert = 113,
  saOp_saOnvert = 114,
  saOp_FConvert = 115,
  saOp_QuantizeToF16 = 116,
  saOp_ConvertPtrToU = 117,
  saOp_SatConvertSToU = 118,
  saOp_SatConvertUToS = 119,
  saOp_ConvertUToPtr = 120,
  saOp_PtrCastToGeneric = 121,
  saOp_GenericCastToPtr = 122,
  saOp_GenericCastToPtrExplicit = 123,
  saOp_Bitcast = 124,
  saOp_SNegate = 126,
  saOp_FNegate = 127,
  saOp_IAdd = 128,
  saOp_FAdd = 129,
  saOp_ISub = 130,
  saOp_FSub = 131,
  saOp_IMul = 132,
  saOp_FMul = 133,
  saOp_UDiv = 134,
  saOp_SDiv = 135,
  saOp_FDiv = 136,
  saOp_UMul = 137,
  saOp_SRem = 138,
  saOp_SMod = 139,
  saOp_FRem = 140,
  saOp_FMod = 141,
  saOp_VectorTimesScalar = 142,
  saOp_MatrixTimesScalar = 143,
  saOp_VectorTimesMatrix = 144,
  saOp_MatrixTimesVector = 145,
  saOp_MatrixTimesMatrix = 146,
  saOp_OuterProduct = 147,
  saOp_Dot = 148,
  saOp_IAddCarry = 149,
  saOp_ISubBorrow = 150,
  saOp_UMulExtended = 151,
  saOp_SMulExtended = 152,
  saOp_Any = 154,
  saOp_All = 155,
  saOp_IsNan = 156,
  saOp_IsInf = 157,
  saOp_IsFinite = 158,
  saOp_IsNormal = 159,
  saOp_SignBitSet = 160,
  saOp_LessOrGreater = 161,
  saOp_Ordered = 162,
  saOp_Unordered = 163,
  saOp_LogicalEqual = 164,
  saOp_LogicalNotEqual = 165,
  saOp_LogicalOr = 166,
  saOp_LogicalAnd = 167,
  saOp_LogicalNot = 168,
  saOp_Select = 169,
  saOp_IEqual = 170,
  saOp_INotEqual = 171,
  saOp_UGreaterThan = 172,
  saOp_SGreaterThan = 173,
  saOp_UGreaterThanEqual = 174,
  saOp_SGreaterThanEqual = 175,
  saOp_ULessThan = 176,
  saOp_SLessThan = 177,
  saOp_ULessThanEqual = 178,
  saOp_SLessThanEqual = 179,
  saOp_FOrdEqual = 180,
  saOp_FUnordEqual = 181,
  saOp_FOrdNotEqual = 182,
  saOp_FUnordNotEqual = 183,
  saOp_FOrdLessThan = 184,
  saOp_FUnordLessThan = 185,
  saOp_FOrdGreaterThan = 186,
  saOp_FUnordGreaterThan = 187,
  saOp_FOrdLessThanEqual = 188,
  saOp_FUnordLessThanEqual = 189,
  saOp_FOrdGreaterThanEqual = 190,
  saOp_FUnordGreaterThanEqual = 191,
  saOp_ShiftRightLogical = 194,
  saOp_ShiftRightArithmetic = 195,
  saOp_ShiftLeftLogical = 196,
  saOp_BitwiseOr = 197,
  saOp_BitwiseXor = 198,
  saOp_BitwiseAnd = 199,
  saOp_Not = 200,
  saOp_BitFieldInsert = 201,
  saOp_BitFieldSExtract = 202,
  saOp_BitFieldUExtract = 203,
  saOp_BitReverse = 204,
  saOp_BitCount = 205,
  saOp_DPdx = 207,
  saOp_DPdy = 208,
  saOp_Fwidth = 209,
  saOp_DPdxFine = 210,
  saOp_DPdyFine = 211,
  saOp_FwidthFine = 212,
  saOp_DPdxCoarse = 213,
  saOp_DPdyCoarse = 214,
  saOp_FwidthCoarse = 215,
  saOp_EmitVertex = 218,
  saOp_EndPrimitive = 219,
  saOp_EmitStreamVertex = 220,
  saOp_EndStreamPrimitive = 221,
  saOp_ControlBarrier = 224,
  saOp_MemoryBarrier = 225,
  saOp_AtomicLoad = 227,
  saOp_AtomicStore = 228,
  saOp_AtomicExchange = 229,
  saOp_AtomicCompareExchange = 230,
  saOp_AtomicCompareExchangeWeak = 231,
  saOp_AtomicIIncrement = 232,
  saOp_AtomicIDecrement = 233,
  saOp_AtomicIAdd = 234,
  saOp_AtomicISub = 235,
  saOp_AtomicSMin = 236,
  saOp_AtomicUMin = 237,
  saOp_AtomicSMax = 238,
  saOp_AtomicUMax = 239,
  saOp_AtomicAnd = 240,
  saOp_AtomicOr = 241,
  saOp_AtomicXor = 242,
  saOp_Phi = 245,
  saOp_LoopMerge = 246,
  saOp_SelectionMerge = 247,
  saOp_Label = 248,
  saOp_Branch = 249,
  saOp_BranchConditional = 250,
  saOp_Switch = 251,
  saOp_Kill = 252,
  saOp_Return = 253,
  saOp_ReturnValue = 254,
  saOp_Unreachable = 255,
  saOp_LifetimeStart = 256,
  saOp_LifetimeStop = 257,
  saOp_GroupAsyncCopy = 259,
  saOp_GroupWaitEvents = 260,
  saOp_GroupAll = 261,
  saOp_GroupAny = 262,
  saOp_GroupBroadcast = 263,
  saOp_GroupIAdd = 264,
  saOp_GroupFAdd = 265,
  saOp_GroupFMin = 266,
  saOp_GroupUMin = 267,
  saOp_GroupSMin = 268,
  saOp_GroupFMax = 269,
  saOp_GroupUMax = 270,
  saOp_GroupSMax = 271,
  saOp_ReadPipe = 274,
  saOp_WritePipe = 275,
  saOp_ReservedReadPipe = 276,
  saOp_ReservedWritePipe = 277,
  saOp_ReservedReadPipePackets = 278,
  saOp_ReservedWritePipePackets = 279,
  saOp_CommitReadPipe = 280,
  saOp_CommitWritePipe = 281,
  saOp_IsValidReservedId = 282,
  saOp_GetNumPipePackets = 283,
  saOp_GetMaxPipePackets = 284,
  saOp_GroupReserveReadPipePackets = 285,
  saOp_GroupReserveWritePipePackets = 286,
  saOp_GroupCommitReadPipe = 287,
  saOp_GroupCommitWritePipe = 288,
  saOp_EnqueueMarker = 291,
  saOp_EnqueueKernel = 292,
  saOp_GetKernelINDrangeSubGroupCount = 293,
  saOp_GetKernelINDrangeMaxSubGroupSize = 294,
  saOp_GetKernelIWorkGroupSize = 295,
  saOp_GetKernelIPreferredWorkGroupSizeMultiple = 296,
  saOp_RetainEvent = 297,
  saOp_ReleaseEvent = 298,
  saOp_CreateUserEvent = 299,
  saOp_IsValidEvent = 300,
  saOp_SetUserEventStatus = 301,
  saOp_CaptureEventProfilingInfo = 302,
  saOp_GetDefaultQueue = 303,
  saOp_BuildNDRange = 304,
  saOp_ImageSparseSampleImplicitLod = 305,
  saOp_ImageSparseSampleExplicitLod = 306,
  saOp_ImageSparseSampleDrefImplicitLod = 307,
  saOp_ImageSparseSampleDrefExplicitLod = 308,
  saOp_ImageSparseFetch = 313,
  saOp_ImageSparseGather = 314,
  saOp_ImageSparseDrefGather = 315,
  saOp_ImageSparseTexelResident = 316,
  saOp_AtomicFlagTestAndSet = 318,
  saOp_AtomicFlagClear = 319,
  saOp_ImageSparseRead = 320,
  saOp_SizeOf = 321,
  saOp_TypePipeStorage = 322,
  saOp_ConstantPipeStorage = 323,
  saOp_CreatePipeFromPipeStorage = 324,
  saOp_GetKernelLocalSizeForSubgroupCount = 325,
  saOp_GetKernelMaxNumSubgroups = 326,
  saOp_TypeNamedBarrier = 327,
  saOp_NamedBarrierInitialize = 328,
  saOp_MemoryNamedBarrier = 329,
  saOp_ModuleProcessed = 330,
  saOp_ExecutionModeId = 331,
  saOp_DecorateId = 332,
  saOp_GroupNonUniformElect = 333,
  saOp_GroupNonUniformAll = 334,
  saOp_GroupNonUniformAny = 335,
  saOp_GroupNonUniformAllEqual = 336,
  saOp_GroupNonUniformBroadcast = 337,
  saOp_GroupNonUniformBroadcastFirst = 338,
  saOp_GroupNonUniformBallot = 339,
  saOp_GroupNonUniformInverseBallot = 340,
  saOp_GroupNonUniformBallotBitExtract = 341,
  saOp_GroupNonUniformBallotBitCount = 342,
  saOp_GroupNonUniformBallotFindLSB = 343,
  saOp_GroupNonUniformBallotFindMSB = 344,
  saOp_GroupNonUniformShuffle = 345,
  saOp_GroupNonUniformShuffleXor = 346,
  saOp_GroupNonUniformShuffleUp = 347,
  saOp_GroupNonUniformShuffleDown = 348,
  saOp_GroupNonUniformIAdd = 349,
  saOp_GroupNonUniformFAdd = 350,
  saOp_GroupNonUniformIMul = 351,
  saOp_GroupNonUniformFMul = 352,
  saOp_GroupNonUniformSMin = 353,
  saOp_GroupNonUniformUMin = 354,
  saOp_GroupNonUniformFMin = 355,
  saOp_GroupNonUniformSMax = 356,
  saOp_GroupNonUniformUMax = 357,
  saOp_GroupNonUniformFMax = 358,
  saOp_GroupNonUniformBitwiseAnd = 359,
  saOp_GroupNonUniformBitwiseOr = 360,
  saOp_GroupNonUniformBitwiseXor = 361,
  saOp_GroupNonUniformLogicalAnd = 362,
  saOp_GroupNonUniformLogicalOr = 363,
  saOp_GroupNonUniformLogicalXor = 364,
  saOp_GroupNonUniformQuadBroadcast = 365,
  saOp_GroupNonUniformQuadSwap = 366,
  saOp_CopyLogical = 400,
  saOp_PtrEqual = 401,
  saOp_PtrNotEqual = 402,
  saOp_PtrDiff = 403
} sa__OpCodesCore_t;

// Thats one public
typedef enum sa_ShaderType_e {
  saShaderType_Vertex =                          saOpEntryPoint_Vertex,
  saShaderType_TessellationControl =             saOpEntryPoint_TessellationControl,
  saShaderType_TessellationEvaluation =          saOpEntryPoint_TessellationEvaluation,
  saShaderType_Geometry =                        saOpEntryPoint_Geometry,
  saShaderType_Fragment =                        saOpEntryPoint_Fragment,
  saShaderType_GLCompute =                       saOpEntryPoint_GLCompute,
  saShaderType_Kernel =                          saOpEntryPoint_Kernel,
  saShaderType_TaskNV =                          saOpEntryPoint_TaskNV,
  saShaderType_MeshNV =                          saOpEntryPoint_MeshNV,
  saShaderType_RayGenerationKHR =                saOpEntryPoint_RayGenerationKHR,
  saShaderType_IntersectionKHR =                 saOpEntryPoint_IntersectionKHR ,
  saShaderType_AnyHitKHR =                       saOpEntryPoint_AnyHitKHR,
  saShaderType_ClosestHitKHR =                   saOpEntryPoint_ClosestHitKHR,
  saShaderType_MissKHR =                         saOpEntryPoint_MissKHR,
  saShaderType_CallableKHR =                     saOpEntryPoint_CallableKHR,
  saShaderType_TaskEXT =                         saOpEntryPoint_TaskEXT,
  saShaderType_MeshEXT =                         saOpEntryPoint_MeshEXT,
} sa_ShaderType_t;

typedef struct sa__spirvId_s {
  char textId[256];
  sa_uint32_t binaryId;
} sa__spirvId_t;

typedef struct sa__spirvIdTable_s {
  sa__spirvId_t* pIds;
  sa_uint32_t idCount;
} sa__spirvIdTable_t;

typedef struct sa__assemblyHeader_s {
    sa_uint32_t magic;
    sa_uint32_t version;
    sa_uint32_t generator;
    sa_uint32_t bounds;
    sa_uint32_t schema;
} sa__assemblyHeader_t;

typedef struct sa__assemblyInstruction_s {
  sa_uint16_t opCode;
  sa_uint16_t wordSize;
  sa_uint32_t* words;
} sa__assemblyInstruction_t;

typedef struct sa_assembly_s {
  sa__assemblyHeader_t header;
  sa__assemblyInstruction_t* pInst;
  sa_uint32_t instCount;
} sa_assembly_t;

struct sa__assemblerErrorMessages_s {
  char** pMessages;
  sa_uint32_t messagesAmount;
} __gAssemblerErrorMessages;

static sa_uint32_t __gIdGeneratorHoldValue = 0;

// This is the big blob of every basic opcode, could be done better with some lookup table
static const char* sa__opcodeToString(sa_uint16_t opcode) {
  switch(opcode) {
    case saOp_Nop: return "OpNop"; break;
    case saOp_Undef: return "OpUndef"; break;
    case saOp_SourceContinued: return "OpSourceContinued"; break;
    case saOp_Source: return "OpSource"; break;
    case saOp_SourceExtension: return "OpSourceExtension"; break;
    case saOp_Name: return "OpName"; break;
    case saOp_MemberName: return "OpMemberName"; break;
    case saOp_String: return "OpString"; break;
    case saOp_Line: return "OpLine"; break;
    case saOp_Extension: return "OpExtension"; break;
    case saOp_ExtInstImport: return "OpExtInstImport"; break;
    case saOp_ExtInst: return "OpExtInst"; break;
    case saOp_MemoryModel: return "OpMemoryModel"; break;
    case saOp_EntryPoint: return "OpEntryPoint"; break;
    case saOp_ExecutionMode: return "OpExecutionMode"; break;
    case saOp_Capability: return "OpCapability"; break;
    case saOp_TypeVoid: return "OpTypeVoid"; break;
    case saOp_TypeBool: return "OpTypeBool"; break;
    case saOp_TypeInt: return "OpTypeInt"; break;
    case saOp_TypeFloat: return "OpTypeFloat"; break;
    case saOp_TypeVector: return "OpTypeVector"; break;
    case saOp_TypeMatrix: return "OpTypeMatrix"; break;
    case saOp_TypeImage: return "OpTypeImage"; break;
    case saOp_TypeSampler: return "OpTypeSampler"; break;
    case saOp_TypeSampledImage: return "OpTypeSampledImage"; break;
    case saOp_TypeArray: return "OpTypeArray"; break;
    case saOp_TypeRuntimeArray: return "OpTypeRuntimeArray"; break;
    case saOp_TypeStruct: return "OpTypeStruct"; break;
    case saOp_TypeOpaque: return "OpTypeOpaque"; break;
    case saOp_TypePointer: return "OpTypePointer"; break;
    case saOp_TypeFunction: return "OpTypeFunction"; break;
    case saOp_TypeEvent: return "OpTypeEvent"; break;
    case saOp_TypeDeviceEvent: return "OpTypeDeviceEvent"; break;
    case saOp_TypeReserveId: return "OpTypeReserveId"; break;
    case saOp_TypeQueue: return "OpTypeQueue"; break;
    case saOp_TypePipe: return "OpTypePipe"; break;
    case saOp_TypeForwardPointer: return "OpTypeForwardPointer"; break;
    case saOp_ConstantTrue: return "OpConstantTrue"; break;
    case saOp_ConstantFalse: return "OpConstantFalse"; break;
    case saOp_Constant: return "OpConstant"; break;
    case saOp_ConstantComposite: return "OpConstantComposite"; break;
    case saOp_ConstantSampler: return "OpConstantSampler"; break;
    case saOp_ConstantNull: return "OpConstantNull"; break;
    case saOp_SpecConstantTrue: return "OpSpecConstantTrue"; break;
    case saOp_SpecConstantFalse: return "OpSpecConstantFalse"; break;
    case saOp_SpecConstant: return "OpSpecConstant"; break;
    case saOp_SpecConstantComposite: return "OpSpecConstantComposite"; break;
    case saOp_SpecConstantOp: return "OpSpecConstantOp"; break;
    case saOp_Function: return "OpFunction"; break;
    case saOp_FunctionParameter: return "OpFunctionParameter"; break;
    case saOp_FunctionEnd: return "OpFunctionEnd"; break;
    case saOp_FunctionCall: return "OpFunctionCall"; break;
    case saOp_Variable: return "OpVariable"; break;
    case saOp_ImageTexelPointer: return "OpImageTexelPointer"; break;
    case saOp_Load: return "OpLoad"; break;
    case saOp_Store: return "OpStore"; break;
    case saOp_CopyMemory: return "OpCopyMemory"; break;
    case saOp_CopyMemorySized: return "OpCopyMemorySized"; break;
    case saOp_AccessChain: return "OpAccessChain"; break;
    case saOp_InBoundsAccessChain: return "OpInBoundsAccessChain"; break;
    case saOp_PtrAccessChain: return "OpPtrAccessChain"; break;
    case saOp_ArrayLength: return "OpArrayLength"; break;
    case saOp_GenericPtrMemSemantics: return "OpGenericPtrMemSemantics"; break;
    case saOp_InBoundsPtrAccessChain: return "OpInBoundsPtrAccessChain"; break;
    case saOp_Decorate: return "OpDecorate"; break;
    case saOp_MemberDecorate: return "OpMemberDecorate"; break;
    case saOp_DecorationGroup: return "OpDecorationGroup"; break;
    case saOp_DecorateGroup: return "OpDecorateGroup"; break;
    case saOp_GroupMemberDecorate: return "OpGroupMemberDecorate"; break;
    case saOp_VectorExtractDynamic: return "OpVectorExtractDynamic"; break;
    case saOp_VectorInsertDynamic: return "OpVectorInsertDynamic"; break;
    case saOp_VectorShuffle: return "OpVectorShuffle"; break;
    case saOp_CompositeConstruct: return "OpCompositeConstruct"; break;
    case saOp_CompositeExtract: return "OpCompositeExtract"; break;
    case saOp_CompositeInsert: return "OpCompositeInsert"; break;
    case saOp_CopyObject: return "OpCopyObject"; break;
    case saOp_Transpose: return "OpTranspose"; break;
    case saOp_SampledImage: return "OpSampledImage"; break;
    case saOp_ImageSampleImplicitLod: return "OpImageSampleImplicitLod"; break;
    case saOp_ImageSampleExplicitLod: return "OpImageSampleExplicitLod"; break;
    case saOp_ImageSampleDrefImplicitLod: return "OpImageSampleDrefImplicitLod"; break;
    case saOp_ImageSampleDrefExplicitLod: return "OpImageSampleDrefExplicitLod"; break;
    case saOp_ImageSampleProjImplicitLod: return "OpImageSampleProjImplicitLod"; break;
    case saOp_ImageSampleProjExplicitLod: return "OpImageSampleProjExplicitLod"; break;
    case saOp_ImageSampleProjDrefImplicitLod: return "OpImageSampleProjDrefImplicitLod"; break;
    case saOp_ImageSampleProjDrefExplicitLod: return "OpImageSampleProjDrefExplicitLod"; break;
    case saOp_ImageFetch: return "OpImageFetch"; break;
    case saOp_ImageGather: return "OpImageGather"; break;
    case saOp_ImageDrefGather: return "OpImageDrefGather"; break;
    case saOp_ImageRead: return "OpImageRead"; break;
    case saOp_ImageWrite: return "OpImageWrite"; break;
    case saOp_Image: return "OpImage"; break;
    case saOp_ImageQueryFormat: return "OpImageQueryFormat"; break;
    case saOp_ImageQueryOrder: return "OpImageQueryOrder"; break;
    case saOp_ImageQuerySizeLod: return "OpImageQuerySizeLod"; break;
    case saOp_ImageQuerySize: return "OpImageQuerySize"; break;
    case saOp_ImageQueryLod: return "OpImageQueryLod"; break;
    case saOp_ImageQueryLevels: return "OpImageQueryLevels"; break;
    case saOp_ImageQuerySamples: return "OpImageQuerySamples"; break;
    case saOp_ConvertFToU: return "OpConvertFToU"; break;
    case saOp_ConvertFToS: return "OpConvertFToS"; break;
    case saOp_ConvertSToF: return "OpConvertSToF"; break;
    case saOp_ConvertUToF: return "OpConvertUToF"; break;
    case saOp_UConvert: return "OpUConvert"; break;
    case saOp_saOnvert: return "OpsaOnvert"; break;
    case saOp_FConvert: return "OpFConvert"; break;
    case saOp_QuantizeToF16: return "OpQuantizeToF16"; break;
    case saOp_ConvertPtrToU: return "OpConvertPtrToU"; break;
    case saOp_SatConvertSToU: return "OpSatConvertSToU"; break;
    case saOp_SatConvertUToS: return "OpSatConvertUToS"; break;
    case saOp_ConvertUToPtr: return "OpConvertUToPtr"; break;
    case saOp_PtrCastToGeneric: return "OpPtrCastToGeneric"; break;
    case saOp_GenericCastToPtr: return "OpGenericCastToPtr"; break;
    case saOp_GenericCastToPtrExplicit: return "OpGenericCastToPtrExplicit"; break;
    case saOp_Bitcast: return "OpBitcast"; break;
    case saOp_SNegate: return "OpSNegate"; break;
    case saOp_FNegate: return "OpFNegate"; break;
    case saOp_IAdd: return "OpIAdd"; break;
    case saOp_FAdd: return "OpFAdd"; break;
    case saOp_ISub: return "OpISub"; break;
    case saOp_FSub: return "OpFSub"; break;
    case saOp_IMul: return "OpIMul"; break;
    case saOp_FMul: return "OpFMul"; break;
    case saOp_UDiv: return "OpUDiv"; break;
    case saOp_SDiv: return "OpSDiv"; break;
    case saOp_FDiv: return "OpFDiv"; break;
    case saOp_UMul: return "OpUMul"; break;
    case saOp_SRem: return "OpSRem"; break;
    case saOp_SMod: return "OpSMod"; break;
    case saOp_FRem: return "OpFRem"; break;
    case saOp_FMod: return "OpFMod"; break;
    case saOp_VectorTimesScalar: return "OpVectorTimesScalar"; break;
    case saOp_MatrixTimesScalar: return "OpMatrixTimesScalar"; break;
    case saOp_VectorTimesMatrix: return "OpVectorTimesMatrix"; break;
    case saOp_MatrixTimesVector: return "OpMatrixTimesVector"; break;
    case saOp_MatrixTimesMatrix: return "OpMatrixTimesMatrix"; break;
    case saOp_OuterProduct: return "OpOuterProduct"; break;
    case saOp_Dot: return "OpDot"; break;
    case saOp_IAddCarry: return "OpIAddCarry"; break;
    case saOp_ISubBorrow: return "OpISubBorrow"; break;
    case saOp_UMulExtended: return "OpUMulExtended"; break;
    case saOp_SMulExtended: return "OpSMulExtended"; break;
    case saOp_Any: return "OpAny"; break;
    case saOp_All: return "OpAll"; break;
    case saOp_IsNan: return "OpIsNan"; break;
    case saOp_IsInf: return "OpIsInf"; break;
    case saOp_IsFinite: return "OpIsFinite"; break;
    case saOp_IsNormal: return "OpIsNormal"; break;
    case saOp_SignBitSet: return "OpSignBitSet"; break;
    case saOp_LessOrGreater: return "OpLessOrGreater"; break;
    case saOp_Ordered: return "OpOrdered"; break;
    case saOp_Unordered: return "OpUnordered"; break;
    case saOp_LogicalEqual: return "OpLogicalEqual"; break;
    case saOp_LogicalNotEqual: return "OpLogicalNotEqual"; break;
    case saOp_LogicalOr: return "OpLogicalOr"; break;
    case saOp_LogicalAnd: return "OpLogicalAnd"; break;
    case saOp_LogicalNot: return "OpLogicalNot"; break;
    case saOp_Select: return "OpSelect"; break;
    case saOp_IEqual: return "OpIEqual"; break;
    case saOp_INotEqual: return "OpINotEqual"; break;
    case saOp_UGreaterThan: return "OpUGreaterThan"; break;
    case saOp_SGreaterThan: return "OpSGreaterThan"; break;
    case saOp_UGreaterThanEqual: return "OpUGreaterThanEqual"; break;
    case saOp_SGreaterThanEqual: return "OpSGreaterThanEqual"; break;
    case saOp_ULessThan: return "OpULessThan"; break;
    case saOp_SLessThan: return "OpSLessThan"; break;
    case saOp_ULessThanEqual: return "OpULessThanEqual"; break;
    case saOp_SLessThanEqual: return "OpSLessThanEqual"; break;
    case saOp_FOrdEqual: return "OpFOrdEqual"; break;
    case saOp_FUnordEqual: return "OpFUnordEqual"; break;
    case saOp_FOrdNotEqual: return "OpFOrdNotEqual"; break;
    case saOp_FUnordNotEqual: return "OpFUnordNotEqual"; break;
    case saOp_FOrdLessThan: return "OpFOrdLessThan"; break;
    case saOp_FUnordLessThan: return "OpFUnordLessThan"; break;
    case saOp_FOrdGreaterThan: return "OpFOrdGreaterThan"; break;
    case saOp_FUnordGreaterThan: return "OpFUnordGreaterThan"; break;
    case saOp_FOrdLessThanEqual: return "OpFOrdLessThanEqual"; break;
    case saOp_FUnordLessThanEqual: return "OpFUnordLessThanEqual"; break;
    case saOp_FOrdGreaterThanEqual: return "OpFOrdGreaterThanEqual"; break;
    case saOp_FUnordGreaterThanEqual: return "OpFUnordGreaterThanEqual"; break;
    case saOp_ShiftRightLogical: return "OpShiftRightLogical"; break;
    case saOp_ShiftRightArithmetic: return "OpShiftRightArithmetic"; break;
    case saOp_ShiftLeftLogical: return "OpShiftLeftLogical"; break;
    case saOp_BitwiseOr: return "OpBitwiseOr"; break;
    case saOp_BitwiseXor: return "OpBitwiseXor"; break;
    case saOp_BitwiseAnd: return "OpBitwiseAnd"; break;
    case saOp_Not: return "OpNot"; break;
    case saOp_BitFieldInsert: return "OpBitFieldInsert"; break;
    case saOp_BitFieldSExtract: return "OpBitFieldSExtract"; break;
    case saOp_BitFieldUExtract: return "OpBitFieldUExtract"; break;
    case saOp_BitReverse: return "OpBitReverse"; break;
    case saOp_BitCount: return "OpBitCount"; break;
    case saOp_DPdx: return "OpDPdx"; break;
    case saOp_DPdy: return "OpDPdy"; break;
    case saOp_Fwidth: return "OpFwidth"; break;
    case saOp_DPdxFine: return "OpDPdxFine"; break;
    case saOp_DPdyFine: return "OpDPdyFine"; break;
    case saOp_FwidthFine: return "OpFwidthFine"; break;
    case saOp_DPdxCoarse: return "OpDPdxCoarse"; break;
    case saOp_DPdyCoarse: return "OpDPdyCoarse"; break;
    case saOp_FwidthCoarse: return "OpFwidthCoarse"; break;
    case saOp_EmitVertex: return "OpEmitVertex"; break;
    case saOp_EndPrimitive: return "OpEndPrimitive"; break;
    case saOp_EmitStreamVertex: return "OpEmitStreamVertex"; break;
    case saOp_EndStreamPrimitive: return "OpEndStreamPrimitive"; break;
    case saOp_ControlBarrier: return "OpControlBarrier"; break;
    case saOp_MemoryBarrier: return "OpMemoryBarrier"; break;
    case saOp_AtomicLoad: return "OpAtomicLoad"; break;
    case saOp_AtomicStore: return "OpAtomicStore"; break;
    case saOp_AtomicExchange: return "OpAtomicExchange"; break;
    case saOp_AtomicCompareExchange: return "OpAtomicCompareExchange"; break;
    case saOp_AtomicCompareExchangeWeak: return "OpAtomicCompareExchangeWeak"; break;
    case saOp_AtomicIIncrement: return "OpAtomicIIncrement"; break;
    case saOp_AtomicIDecrement: return "OpAtomicIDecrement"; break;
    case saOp_AtomicIAdd: return "OpAtomicIAdd"; break;
    case saOp_AtomicISub: return "OpAtomicISub"; break;
    case saOp_AtomicSMin: return "OpAtomicSMin"; break;
    case saOp_AtomicUMin: return "OpAtomicUMin"; break;
    case saOp_AtomicSMax: return "OpAtomicSMax"; break;
    case saOp_AtomicUMax: return "OpAtomicUMax"; break;
    case saOp_AtomicAnd: return "OpAtomicAnd"; break;
    case saOp_AtomicOr: return "OpAtomicOr"; break;
    case saOp_AtomicXor: return "OpAtomicXor"; break;
    case saOp_Phi: return "OpPhi"; break;
    case saOp_LoopMerge: return "OpLoopMerge"; break;
    case saOp_SelectionMerge: return "OpSelectionMerge"; break;
    case saOp_Label: return "OpLabel"; break;
    case saOp_Branch: return "OpBranch"; break;
    case saOp_BranchConditional: return "OpBranchConditional"; break;
    case saOp_Switch: return "OpSwitch"; break;
    case saOp_Kill: return "OpKill"; break;
    case saOp_Return: return "OpReturn"; break;
    case saOp_ReturnValue: return "OpReturnValue"; break;
    case saOp_Unreachable: return "OpUnreachable"; break;
    case saOp_LifetimeStart: return "OpLifetimeStart"; break;
    case saOp_LifetimeStop: return "OpLifetimeStop"; break;
    case saOp_GroupAsyncCopy: return "OpGroupAsyncCopy"; break;
    case saOp_GroupWaitEvents: return "OpGroupWaitEvents"; break;
    case saOp_GroupAll: return "OpGroupAll"; break;
    case saOp_GroupAny: return "OpGroupAny"; break;
    case saOp_GroupBroadcast: return "OpGroupBroadcast"; break;
    case saOp_GroupIAdd: return "OpGroupIAdd"; break;
    case saOp_GroupFAdd: return "OpGroupFAdd"; break;
    case saOp_GroupFMin: return "OpGroupFMin"; break;
    case saOp_GroupUMin: return "OpGroupUMin"; break;
    case saOp_GroupSMin: return "OpGroupSMin"; break;
    case saOp_GroupFMax: return "OpGroupFMax"; break;
    case saOp_GroupUMax: return "OpGroupUMax"; break;
    case saOp_GroupSMax: return "OpGroupSMax"; break;
    case saOp_ReadPipe: return "OpReadPipe"; break;
    case saOp_WritePipe: return "OpWritePipe"; break;
    case saOp_ReservedReadPipe: return "OpReservedReadPipe"; break;
    case saOp_ReservedWritePipe: return "OpReservedWritePipe"; break;
    case saOp_ReservedReadPipePackets: return "OpReservedReadPipePackets"; break;
    case saOp_ReservedWritePipePackets: return "OpReservedWritePipePackets"; break;
    case saOp_CommitReadPipe: return "OpCommitReadPipe"; break;
    case saOp_CommitWritePipe: return "OpCommitWritePipe"; break;
    case saOp_IsValidReservedId: return "OpIsValidReservedId"; break;
    case saOp_GetNumPipePackets: return "OpGetNumPipePackets"; break;
    case saOp_GetMaxPipePackets: return "OpGetMaxPipePackets"; break;
    case saOp_GroupReserveReadPipePackets: return "OpGroupReserveReadPipePackets"; break;
    case saOp_GroupReserveWritePipePackets: return "OpGroupReserveWritePipePackets"; break;
    case saOp_GroupCommitReadPipe: return "OpGroupCommitReadPipe"; break;
    case saOp_GroupCommitWritePipe: return "OpGroupCommitWritePipe"; break;
    case saOp_EnqueueMarker: return "OpEnqueueMarker"; break;
    case saOp_EnqueueKernel: return "OpEnqueueKernel"; break;
    case saOp_GetKernelINDrangeSubGroupCount: return "OpGetKernelINDrangeSubGroupCount"; break;
    case saOp_GetKernelINDrangeMaxSubGroupSize: return "OpGetKernelINDrangeMaxSubGroupSize"; break;
    case saOp_GetKernelIWorkGroupSize: return "OpGetKernelIWorkGroupSize"; break;
    case saOp_GetKernelIPreferredWorkGroupSizeMultiple: return "OpGetKernelIPreferredWorkGroupSizeMultiple"; break;
    case saOp_RetainEvent: return "OpRetainEvent"; break;
    case saOp_ReleaseEvent: return "OpReleaseEvent"; break;
    case saOp_CreateUserEvent: return "OpCreateUserEvent"; break;
    case saOp_IsValidEvent: return "OpIsValidEvent"; break;
    case saOp_SetUserEventStatus: return "OpSetUserEventStatus"; break;
    case saOp_CaptureEventProfilingInfo: return "OpCaptureEventProfilingInfo"; break;
    case saOp_GetDefaultQueue: return "OpGetDefaultQueue"; break;
    case saOp_BuildNDRange: return "OpBuildNDRange"; break;
    case saOp_ImageSparseSampleImplicitLod: return "OpImageSparseSampleImplicitLod"; break;
    case saOp_ImageSparseSampleExplicitLod: return "OpImageSparseSampleExplicitLod"; break;
    case saOp_ImageSparseSampleDrefImplicitLod: return "OpImageSparseSampleDrefImplicitLod"; break;
    case saOp_ImageSparseSampleDrefExplicitLod: return "OpImageSparseSampleDrefExplicitLod"; break;
    case saOp_ImageSparseFetch: return "OpImageSparseFetch"; break;
    case saOp_ImageSparseGather: return "OpImageSparseGather"; break;
    case saOp_ImageSparseDrefGather: return "OpImageSparseDrefGather"; break;
    case saOp_ImageSparseTexelResident: return "OpImageSparseTexelResident"; break;
    case saOp_AtomicFlagTestAndSet: return "OpAtomicFlagTestAndSet"; break;
    case saOp_AtomicFlagClear: return "OpAtomicFlagClear"; break;
    case saOp_ImageSparseRead: return "OpImageSparseRead"; break;
    case saOp_SizeOf: return "OpSizeOf"; break;
    case saOp_TypePipeStorage: return "OpTypePipeStorage"; break;
    case saOp_ConstantPipeStorage: return "OpConstantPipeStorage"; break;
    case saOp_CreatePipeFromPipeStorage: return "OpCreatePipeFromPipeStorage"; break;
    case saOp_GetKernelLocalSizeForSubgroupCount: return "OpGetKernelLocalSizeForSubgroupCount"; break;
    case saOp_GetKernelMaxNumSubgroups: return "OpGetKernelMaxNumSubgroups"; break;
    case saOp_TypeNamedBarrier: return "OpTypeNamedBarrier"; break;
    case saOp_NamedBarrierInitialize: return "OpNamedBarrierInitialize"; break;
    case saOp_MemoryNamedBarrier: return "OpMemoryNamedBarrier"; break;
    case saOp_ModuleProcessed: return "OpModuleProcessed"; break;
    case saOp_ExecutionModeId: return "OpExecutionModeId"; break;
    case saOp_DecorateId: return "OpDecorateId"; break;
    case saOp_GroupNonUniformElect: return "OpGroupNonUniformElect"; break;
    case saOp_GroupNonUniformAll: return "OpGroupNonUniformAll"; break;
    case saOp_GroupNonUniformAny: return "OpGroupNonUniformAny"; break;
    case saOp_GroupNonUniformAllEqual: return "OpGroupNonUniformAllEqual"; break;
    case saOp_GroupNonUniformBroadcast: return "OpGroupNonUniformBroadcast"; break;
    case saOp_GroupNonUniformBroadcastFirst: return "OpGroupNonUniformBroadcastFirst"; break;
    case saOp_GroupNonUniformBallot: return "OpGroupNonUniformBallot"; break;
    case saOp_GroupNonUniformInverseBallot: return "OpGroupNonUniformInverseBallot"; break;
    case saOp_GroupNonUniformBallotBitExtract: return "OpGroupNonUniformBallotBitExtract"; break;
    case saOp_GroupNonUniformBallotBitCount: return "OpGroupNonUniformBallotBitCount"; break;
    case saOp_GroupNonUniformBallotFindLSB: return "OpGroupNonUniformBallotFindLSB"; break;
    case saOp_GroupNonUniformBallotFindMSB: return "OpGroupNonUniformBallotFindMSB"; break;
    case saOp_GroupNonUniformShuffle: return "OpGroupNonUniformShuffle"; break;
    case saOp_GroupNonUniformShuffleXor: return "OpGroupNonUniformShuffleXor"; break;
    case saOp_GroupNonUniformShuffleUp: return "OpGroupNonUniformShuffleUp"; break;
    case saOp_GroupNonUniformShuffleDown: return "OpGroupNonUniformShuffleDown"; break;
    case saOp_GroupNonUniformIAdd: return "OpGroupNonUniformIAdd"; break;
    case saOp_GroupNonUniformFAdd: return "OpGroupNonUniformFAdd"; break;
    case saOp_GroupNonUniformIMul: return "OpGroupNonUniformIMul"; break;
    case saOp_GroupNonUniformFMul: return "OpGroupNonUniformFMul"; break;
    case saOp_GroupNonUniformSMin: return "OpGroupNonUniformSMin"; break;
    case saOp_GroupNonUniformUMin: return "OpGroupNonUniformUMin"; break;
    case saOp_GroupNonUniformFMin: return "OpGroupNonUniformFMin"; break;
    case saOp_GroupNonUniformSMax: return "OpGroupNonUniformSMax"; break;
    case saOp_GroupNonUniformUMax: return "OpGroupNonUniformUMax"; break;
    case saOp_GroupNonUniformFMax: return "OpGroupNonUniformFMax"; break;
    case saOp_GroupNonUniformBitwiseAnd: return "OpGroupNonUniformBitwiseAnd"; break;
    case saOp_GroupNonUniformBitwiseOr: return "OpGroupNonUniformBitwiseOr"; break;
    case saOp_GroupNonUniformBitwiseXor: return "OpGroupNonUniformBitwiseXor"; break;
    case saOp_GroupNonUniformLogicalAnd: return "OpGroupNonUniformLogicalAnd"; break;
    case saOp_GroupNonUniformLogicalOr: return "OpGroupNonUniformLogicalOr"; break;
    case saOp_GroupNonUniformLogicalXor: return "OpGroupNonUniformLogicalXor"; break;
    case saOp_GroupNonUniformQuadBroadcast: return "OpGroupNonUniformQuadBroadcast"; break;
    case saOp_GroupNonUniformQuadSwap: return "OpGroupNonUniformQuadSwap"; break;
    case saOp_CopyLogical: return "OpCopyLogical"; break;
    case saOp_PtrEqual: return "OpPtrEqual"; break;
    case saOp_PtrNotEqual: return "OpPtrNotEqual"; break;
    case saOp_PtrDiff: return "OpPtrDiff"; break;  
  }

  return "";
}

static void* sa__copyMemory(const void* pSrc, void* pDst, sa_uint32_t size) {
  for(sa_uint32_t i = 0; i < size; i++) {
    ((sa_uint8_t*)pDst)[i] = ((sa_uint8_t*)pSrc)[i];
  }

  return pDst;
}

static void* sa__setMemory(void* pMem, int value, sa_uint32_t size) {
  for(sa_uint32_t i = 0; i < size; i++) {
    ((sa_uint8_t*)pMem)[i] = value;
  }

  return pMem;
}

static sa_uint32_t sa__compareString(const char* a, const char* b) {
  char* _a = (char*)a;
  char* _b = (char*)b;

  while(_a && _b) {
    if(_a - _b != 0)
      return _a - _b;
    _a++;
    _b++;
  }

  return _a - _b;
}

static sa_uint32_t sa__lengthString(const char* str) {
  char* s = (char*)str;
  
  while(s)
    s++;

  return s - str;
}

static char* sa__intToString(sa_int32_t value) {
  static char str[32] = {0};
  sa__setMemory(str, 0, 32 * sizeof(char));

  sa_uint32_t num = value;
  sa_uint32_t numCounter = 0;

  if(num < 0) {
    str[numCounter] = '-';
    numCounter++;
    num = -num;
  }

  while(num) {
    str[numCounter] = '0' + (num % 10);
    numCounter++;
    num /= 10;
  }

  for(sa_uint32_t i = 0; i < numCounter / 2; i++) {
    char tmp = str[i];
    str[i] = str[numCounter - i];
    str[numCounter - i] = tmp;
  }

  return str;
}

static char* sa__hexToString(sa_uint32_t value) {
  static char str[32] = {0};
  sa__setMemory(str, 0, 32 * sizeof(char));

  sa_uint32_t num = value;
  sa_uint32_t numCounter = 0;

  while(num) {
    str[numCounter] = (num & 0xF < 0xA ? '0' + num : 'A' + (num - 0xA));
    numCounter++;
    num >>= 4;
  }

  for(sa_uint32_t i = 0; i < numCounter / 2; i++) {
    char tmp = str[i];
    str[i] = str[numCounter - i];
    str[numCounter - i] = tmp;
  }

  return str;
}

static char* sa__floatToString(float value, int decimals) {
  // buffer large enough for sign + int + dot + frac + null
  static char buf[64];  
  char* p = buf;

  // handle sign
  if (value < 0) {
    *p++ = '-';
    value = -value;
  }

  // integer part
  int int_part = (int)value;
  float frac_part = value - (float)int_part;

  // write integer digits (reverse first)
  char intbuf[32];
  char* ip = intbuf;
  if (int_part == 0)
    *ip++ = '0';
  else {
    while (int_part > 0) {
      *ip++ = '0' + (int_part % 10);
      int_part /= 10;
    }
  }

  // copy integer digits back in correct order
  while (ip != intbuf)
    *p++ = *--ip;

  // decimal point
  if (decimals > 0) {
    *p++ = '.';

    for (int i = 0; i < decimals; i++) {
      frac_part *= 10.0f;
      int digit = (int)frac_part;
      *p++ = '0' + digit;
      frac_part -= digit;
    }
  }

  *p = '\0';
  return buf;
}

static void sa__errMsg(const char* fmt, ...) {
  sa_uint32_t count = 0;

  for(sa_uint32_t i = 0; i < sa__lengthString(fmt); i++) {
    if(fmt[i] == '%')
      count++;
  }
  
  __gAssemblerErrorMessages.pMessages = (char**)sa_realloc(__gAssemblerErrorMessages.pMessages, sizeof(char*) * (++__gAssemblerErrorMessages.messagesAmount));
  // If you look closely at how this is handled, it is so unoptimally done
  char* msg = __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1];

  va_list args;
  va_start(args, count);

  sa_uint32_t msgSize = 0;

  for(sa_uint32_t i = 0; i < sa__lengthString(fmt); i++) {
    if(fmt[i] == '%') {
      switch(fmt[i + 1]) {
      // Char
      case 'c':
        msg = (char*)sa_realloc(msg, sizeof(char) * (++msgSize));
        msg[msgSize - 1] = va_arg(args, char);
        break;

      // Int
      case 'd':
        char* number = sa__intToString(va_arg(args, sa_int32_t));
        sa_uint32_t numLength = sa__lengthString(number);

        msgSize += numLength;
        msg = (char*)sa_realloc(msg, sizeof(char) * msgSize);
        sa__copyMemory(number, &msg[msgSize - numLength - 1], numLength);
        break;

      // Float
      case 'f':
        char* fnumber = sa__floatToString(va_arg(args, float), 6);
        sa_uint32_t fnumLength = sa__lengthString(fnumber);

        msgSize += fnumLength;
        msg = (char*)sa_realloc(msg, sizeof(char) * msgSize);
        sa__copyMemory(fnumber, &msg[msgSize - fnumLength - 1], fnumLength);
        break;

      // String
      case 's':
        char* string = va_arg(args, char*);
        sa_uint32_t length = sa__lengthString(string);

        msgSize += length;
        msg = (char*)sa_realloc(msg, sizeof(char) * msgSize);
        sa__copyMemory(string, &msg[msgSize - length - 1], length);
        break;

      // Hex
      case 'x':
        char* hnumber = sa__hexToString(va_arg(args, sa_uint32_t));
        sa_uint32_t hnumLength = sa__lengthString(hnumber);

        msgSize += hnumLength;
        msg = (char*)sa_realloc(msg, sizeof(char) * msgSize);
        sa__copyMemory(hnumber, &msg[msgSize - hnumLength - 1], hnumLength);
        break;

      // The percent chacarter
      case '%':
        msg = (char*)sa_realloc(msg, sizeof(char) * (++msgSize));
        msg[msgSize - 1] = '%';
        break;

      default:
        break;
      }

      i += 2;
    }

    msg = (char*)sa_realloc(msg, sizeof(char) * (++msgSize));
    msg[msgSize - 1] = fmt[i];
  }

  va_end(args);
}

static sa_uint32_t sa__genId() {
  return __gIdGeneratorHoldValue++;
}

static void sa__resetId() {
  __gIdGeneratorHoldValue = 0;
}

static sa_uint32_t sa__getSpirvId(sa__spirvIdTable_t* pIds, const char* name) {
  for(sa_uint32_t i = 0; i < pIds->idCount; i++) {
    if(sa__compareString(name, pIds->pIds[i].textId) == 0)
      return pIds->pIds[i].binaryId;
  }

  pIds->pIds = (sa__spirvId_t*)sa_realloc(pIds->pIds, sizeof(sa__spirvId_t) * (++pIds->idCount));
  sa__setMemory(&pIds->pIds[pIds->idCount - 1], 0, sizeof(pIds->pIds[pIds->idCount - 1]));
  pIds->pIds[pIds->idCount - 1].binaryId = sa__genId();
  sa__copyMemory(name, pIds->pIds[pIds->idCount - 1].textId, sa__lengthString(name));

  return pIds->pIds[pIds->idCount - 1].binaryId;
}

static void sa__addInstructionFormatted(sa_assembly_t* pAsm, sa_uint16_t wordSize, sa_uint16_t op, ...) {
  pAsm->instCount++;
  pAsm->pInst = (sa__assemblyInstruction_t*)sa_realloc(pAsm->pInst, sizeof(sa__assemblyInstruction_t) * pAsm->instCount);
  pAsm->pInst[pAsm->instCount - 1].opCode = op;
  pAsm->pInst[pAsm->instCount - 1].wordSize = wordSize;
  pAsm->pInst[pAsm->instCount - 1].words = (sa_uint32_t*)sa_realloc(pAsm->pInst[pAsm->instCount - 1].words, sizeof(sa_uint32_t) * wordSize);

  va_list args;
  va_start(args, wordSize - 1);

  for(sa_uint32_t i = 0; i < wordSize - 1; i++) {
    pAsm->pInst[pAsm->instCount - 1].words[i] =  va_arg(args, sa_uint32_t);
  }

  va_end(args);
}

static void sa__addInstruction(sa_assembly_t* pAsm, sa_uint16_t wordSize, sa_uint16_t op, sa_uint32_t* words) {
  pAsm->instCount++;

  if(pAsm->pInst)
    pAsm->pInst = (sa__assemblyInstruction_t*)sa_realloc(pAsm->pInst, sizeof(sa__assemblyInstruction_t) * pAsm->instCount);
  else
    pAsm->pInst = (sa__assemblyInstruction_t*)sa_calloc(pAsm->instCount, sizeof(sa__assemblyInstruction_t));

  sa__setMemory(&pAsm->pInst[pAsm->instCount - 1], 0, sizeof(pAsm->pInst[pAsm->instCount - 1]));

  pAsm->pInst[pAsm->instCount - 1].opCode = op;
  pAsm->pInst[pAsm->instCount - 1].wordSize = wordSize;

  if(pAsm->pInst[pAsm->instCount - 1].words)
    pAsm->pInst[pAsm->instCount - 1].words = (sa_uint32_t*)sa_realloc(pAsm->pInst[pAsm->instCount - 1].words, sizeof(sa_uint32_t) * wordSize);
  else
    pAsm->pInst[pAsm->instCount - 1].words = (sa_uint32_t*)sa_calloc(wordSize, sizeof(sa_uint32_t));

  sa__copyMemory(words, pAsm->pInst[pAsm->instCount - 1].words, (wordSize - 1) * sizeof(sa_uint32_t));
}

static void sa_freeAssembly(sa_assembly_t* pAsm) {
  for(sa_uint32_t i = 0; i < pAsm->instCount; i++) {
    sa_free(pAsm->pInst[i].words);
    pAsm->pInst[i].words = SA_NULL;
    pAsm->pInst[i].opCode = 0;
    pAsm->pInst[i].wordSize = 0;
  }

  sa_free(pAsm->pInst);
  pAsm->pInst = SA_NULL;
  pAsm->instCount = 0;
  pAsm->header.bounds = 0;
  pAsm->header.generator = 0;
  pAsm->header.magic = 0;
  pAsm->header.schema = 0;
  pAsm->header.version = 0;
}

static void sa_freeSPIRV(sa_binary spirvBin) {
  sa_free(spirvBin);
}

/**
 * @brief Return code as a pointer
 * 
 * @param spirvBasicAssembly text to process
 * @param pCodeSizeOut assembled code size
 * @return sa_uint32_t* Assembled code
 */
static sa_binary sa_assembleSPIRV(const char* spirvBasicAssembly, sa_uint32_t* pCodeSizeOut, sa_ShaderType_t shaderType, const char* entryPointFunction) {
  sa__resetId();

  sa_binary sbin = NULL;



  return sbin;
}

/**
 * @brief disassemble SPIR-V shader
 * 
 * @param shaderBin 
 * @param shaderSize size must be as a amount of elements inside SPIR-V in 32bit format (so read file length / sizeof(int))
 */
static void sa_disassembleSPIRV(sa_assembly_t* pAsm, sa_binary shaderBin, sa_uint32_t shaderSize) {
  sa_uint32_t index = 0;

  sa__setMemory(pAsm, 0, sizeof(*pAsm));

  if(shaderBin[0] != SA_SPIRV_MAGIC_NUMBER) {
    sa__errMsg("SPIR-V Magic number does not match file magic number!\n");

    return;
  }

  pAsm->header.magic = shaderBin[0];
  pAsm->header.version = shaderBin[1];
  pAsm->header.generator = shaderBin[2];
  pAsm->header.bounds = shaderBin[3];
  pAsm->header.schema = shaderBin[4];
  // Advance index by 5 to skip header
  index += 5;

  for(; index < shaderSize; index++) {
    // This contain opcode and word count
    sa_uint32_t word = shaderBin[index];

    // Mask off LSB as this is still in LE, then convert
    sa_uint16_t opcode = word & 0x0000FFFF;
    sa_uint16_t wordCount = (word >> 16) & 0x0000FFFF;

    // Add all words
    sa_uint32_t* words = (sa_uint32_t*)sa_calloc(wordCount - 1, sizeof(sa_uint32_t));

    for(sa_uint32_t i = 0; i < wordCount - 1; i++) {
      index++;
      words[i] = shaderBin[index];
    }

    // Make instruction
    sa__addInstruction(pAsm, wordCount, opcode, words);

    // Free temp garbage
    sa_free(words);
    words = SA_NULL;
  }
}

#endif