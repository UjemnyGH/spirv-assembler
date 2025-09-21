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
 * 
 * This assembler uses my own language that closely tie to the SPIR-V binary itself, only cutting off teadious part and allowing some cool features.
 * The language is called SPIR-V Basic Assembly (SBA in short), as it is assembly for SPIR-V but understanding it is quite intuitive (basic).
 * Docs should be in docs/ folder for every feature of SBA, as well as examples.
 * 
 * Have fun
 *  UjemnyGH
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

#define SA_CONVERT(x) (x)

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

#define sa_ptr8(x) ((sa_uint8_t*)x)
#define sa_ptr16(x) ((sa_uint16_t*)x)
#define sa_ptr32(x) ((sa_uint32_t*)x)
#define sa_ptr64(x) ((sa_uint64_t*)x)

#define SA_DEFAULT_MEMORY_ADDRESSING_MODEL saAddresingModel_Logical
#define SA_DEFAULT_MEMORY_MODEL saMemoryModel_Vulkan

#define SA_MAX_ENUMERANT_ENTRIES 128

// Used by OpEntryPoint
enum sa__EntryPoint_e {
  saEntryPoint_Vertex = 0,
  saEntryPoint_TessellationControl = 1,
  saEntryPoint_TessellationEvaluation = 2,
  saEntryPoint_Geometry = 3,
  saEntryPoint_Fragment = 4,
  saEntryPoint_GLCompute = 5,
  // XXX: everything below is currently unused
  saEntryPoint_Kernel = 6,
  saEntryPoint_TaskNV = 5267,
  saEntryPoint_MeshNV = 5268,
  saEntryPoint_RayGenerationKHR = 5313,
  saEntryPoint_IntersectionKHR = 5314,
  saEntryPoint_AnyHitKHR = 5315,
  saEntryPoint_ClosestHitKHR = 5316,
  saEntryPoint_MissKHR = 5317,
  saEntryPoint_CallableKHR = 5318,
  saEntryPoint_TaskEXT = 5364,
  saEntryPoint_MeshEXT = 5365,

  // Nvidia raytracing, same as Khronos
  saEntryPoint_RayGenerationNV = saEntryPoint_RayGenerationKHR,
  saEntryPoint_IntersectionNV = saEntryPoint_IntersectionKHR,
  saEntryPoint_AnyHitNV = saEntryPoint_AnyHitKHR,
  saEntryPoint_ClosestHitNV = saEntryPoint_ClosestHitKHR,
  saEntryPoint_MissNV = saEntryPoint_MissKHR,
  saEntryPoint_CallableNV = saEntryPoint_CallableKHR
};

// Used by OpMemoryModel
enum sa__AddressingModel_e {
  saAddresingModel_Logical = 0,
  saAddresingModel_Physical32 = 1,
  saAddresingModel_Physical64 = 2,
  saAddresingModel_PhysicalStorageBuffer64 = 5348,
  saAddresingModel_PhysicalStorageBuffer64EXT = saAddresingModel_PhysicalStorageBuffer64
};

// Used by OpMemoryModel
enum sa__MemoryModel_e {
  saMemoryModel_Simple = 0,
  saMemoryModel_GLSL450 = 1,
  saMemoryModel_OpenCL = 2,
  saMemoryModel_Vulkan = 3,
  saMemoryModel_VulkanKHR = saMemoryModel_Vulkan
};

// Used by OpExecutionMode and OpExecutionModeId
enum sa__OpExecutionMode_e {
  saExecutionMode_Invocations = 0,
  saExecutionMode_SpacingEqual = 1,
  saExecutionMode_SpacingFractionalEven = 2,
  saExecutionMode_SpacingFractionalOdd = 3,
  saExecutionMode_VertexOrderCw = 4,
  saExecutionMode_VertexOrderCcw = 5,
  saExecutionMode_PixelCenterInteger = 6,
  saExecutionMode_OriginUpperLeft = 7,
  saExecutionMode_OriginLowerLeft = 8,
  saExecutionMode_EarlyFragmentTests = 9,
  saExecutionMode_PointMode = 10,
  saExecutionMode_Xfb = 11,
  saExecutionMode_DepthReplacing = 12,
  saExecutionMode_DepthGreater = 14,
  saExecutionMode_DepthLess = 15,
  saExecutionMode_DepthUnchanged = 16,
  saExecutionMode_LocalSize = 17,
  saExecutionMode_LocalSizeHint = 18,
  saExecutionMode_InputPoints = 19,
  saExecutionMode_InputLines = 20,
  saExecutionMode_InputLinesAdjacency = 21,
  saExecutionMode_InputTriangles = 22,
  saExecutionMode_InputTrianglesAdjacency = 23,
  saExecutionMode_Quads = 24,
  saExecutionMode_Isolines = 25,
  saExecutionMode_OutputVertices = 26,
  saExecutionMode_OutputPoints = 27,
  saExecutionMode_OutputLineStrip = 28,
  saExecutionMode_OutputTriangleStrip = 29,
  saExecutionMode_VecTypeHint = 30,
  saExecutionMode_ContractionOff = 31,
  saExecutionMode_Initializer = 33,
  saExecutionMode_Finalizer = 34,
  saExecutionMode_SubgroupSize = 35,
  saExecutionMode_SubgroupsPerWorkgroup = 36,
  saExecutionMode_SubgroupsPerWorkgroupId = 37,
  saExecutionMode_LocalSizeId = 38,
  saExecutionMode_LocalSizeHintId = 39,
  saExecutionMode_NonCoherentColorAttachmentReadEXT = 4169,
  saExecutionMode_NonCoherentDepthAttachmentReadEXT = 4170,
  saExecutionMode_NonCoherentStencilAttachmentReadEXT = 4171,
  saExecutionMode_SubgroupUniformControlFlowKHR = 4421,
  saExecutionMode_PostDepthCoverage = 4446,
  saExecutionMode_DenormPreserve = 4459,
  saExecutionMode_DenormFlushToZero = 4460,
  saExecutionMode_SignedZeroInfNanPreserve = 4461,
  saExecutionMode_RoundingModeRTE = 4462,
  saExecutionMode_RoundingModeRTZ = 4463,
  saExecutionMode_EarlyAndLateFragmentTestsAMD = 5017,
  saExecutionMode_StencilRefReplacingEXT = 5027,
  saExecutionMode_CoalesaingAMDX = 5069,
  saExecutionMode_IsApiEntryAMDX = 5070,
  saExecutionMode_MaxNodeRecursionAMDX = 5071,
  saExecutionMode_StaticNumWorkgroupsAMDX = 5072,
  saExecutionMode_ShaderIndexAMDX = 5073,
  saExecutionMode_MaxNumWorkgroupsAMDX = 5077,
  saExecutionMode_StencilRefUnchangedFrontAMD = 5079,
  saExecutionMode_StencilRefGreaterFrontAMD = 5080,
  saExecutionMode_StencilRefLessFrontAMD = 5081,
  saExecutionMode_StencilRefUnchangedBackAMD = 5082,
  saExecutionMode_StencilRefGreaterBackAMD = 5083,
  saExecutionMode_StencilRefLessBackAMD = 5084,
  saExecutionMode_QuadDerivativesKHR = 5088,
  saExecutionMode_RequireFullQuadsKHR = 5089,
  saExecutionMode_ShareInputWithAMDX = 5102,
  saExecutionMode_OutputLinesEXT = 5269,
  saExecutionMode_OutputPrimitivesEXT = 5270,
  saExecutionMode_DerivativeGroupQuadsKHR = 5289,
  saExecutionMode_DerivativeGroupLinearKHR = 5290,
  saExecutionMode_OutputTrianglesEXT = 5298,
  saExecutionMode_PixelInterlockOrderedEXT = 5366,
  saExecutionMode_PixelInterlockUnorderedEXT = 5367,
  saExecutionMode_SamplerInterlockOrderedEXT = 5368,
  saExecutionMode_SamplerInterlockUnorderedEXT = 5369,
  saExecutionMode_ShadingRateInterlockOrderedEXT = 5370,
  saExecutionMode_ShadingRateInterlockUnorderedEXT = 5371,
  saExecutionMode_SharedLocalMemorySizeINTEL = 5618,
  saExecutionMode_RoundingModeRTPINTEL = 5620,
  saExecutionMode_RoundingModeRTNINTEL = 5621,
  saExecutionMode_FloatingPointModeALTINTEL = 5622,
  saExecutionMode_FloatingPointModeIEEEINTEL = 5623,
  saExecutionMode_MaxWorkgroupSizeINTEL = 5893,
  saExecutionMode_MaxWorkDimIMTEL = 5894,
  saExecutionMode_NoGlobalOffsetINTEL = 5895,
  saExecutionMode_NumSIMDWorkitemsINTEL = 5896,
  saExecutionMode_schedulerTargetFmaxMhzINTEL = 5903,
  saExecutionMode_MaximallyReconvergesKHR = 6023,
  saExecutionMode_FPFastMathDefault = 6028,
  saExecutionMode_StreamingInterfaceINTEL = 6154,
  saExecutionMode_RegisterMapInterfaceINTEL = 6160,
  saExecutionMode_NamedBarrierCountINTEL = 6417,
  saExecutionMode_MaximumRegistersINTEL = 6461,
  saExecutionMode_MaximumRegistersIdINTEL = 6462,
  saExecutionMode_NamedMaximumRegistersINTEL = 6463,

  saExecutionMode_OutputLinesNV = saExecutionMode_OutputLinesEXT,
  saExecutionMode_OutputPrimitivesNV = saExecutionMode_OutputPrimitivesEXT,
  saExecutionMode_DerivativeGroupQuadsNV = saExecutionMode_DerivativeGroupQuadsKHR,
  saExecutionMode_DerivativeGroupLinearNV = saExecutionMode_DerivativeGroupLinearKHR,
  saExecutionMode_OutputTrianglesNV = saExecutionMode_OutputTrianglesEXT
};

// Used by OpTypePointer, OpTypeForwardPointer, OpVariable, OpGenericCastToPtrExplicit, OpTypeUndefinedPointerKHR, OpUntypedVariableKHR
enum sa__StorageClass_e {
  saStorageClass_UniformConstant = 0,
  saStorageClass_Input = 1,
  saStorageClass_Uniform = 2,
  saStorageClass_Output = 3,
  saStorageClass_Workgroup = 4,
  saStorageClass_CrossWorkgroup = 5,
  saStorageClass_Private = 6,
  saStorageClass_Function = 7,
  saStorageClass_Generic = 8,
  saStorageClass_PushConstant = 9,
  saStorageClass_AtomicCounter = 10,
  saStorageClass_Image = 11,
  saStorageClass_StorageBuffer = 12,
  saStorageClass_TileImageEXT = 4172,
  saStorageClass_NodePayloadAMDX = 5068,
  saStorageClass_CallableDataKHR = 5328,
  saStorageClass_IncomingCallableDataKHR = 5329,
  saStorageClass_RayPayloadKHR = 5338,
  saStorageClass_HitAttributeKHR = 5339,
  saStorageClass_IncomingRayPayloadKHR = 5342,
  saStorageClass_ShaderRecordBufferKHR = 5343,
  saStorageClass_PhysicalStorageBuffer = 5349,
  saStorageClass_HitObjectAttributeNV = 5385,
  saStorageClass_TaskPayloadWorkgroupEXT = 5402,
  saStorageClass_CodeSelectionINTEL = 5605,
  saStorageClass_DeviceOnlyINTEL = 5936,
  saStorageClass_HostOnlyINTEL = 5937,

  saStorageClass_CallableDataNV = saStorageClass_CallableDataKHR,
  saStorageClass_IncomingCallableDataNV = saStorageClass_IncomingCallableDataKHR,
  saStorageClass_RayPayloadNV = saStorageClass_RayPayloadKHR,
  saStorageClass_HitAttributeNV = saStorageClass_HitAttributeKHR,
  saStorageClass_IncomingRayPayloadNV = saStorageClass_IncomingRayPayloadKHR,
  saStorageClass_ShaderRecordBufferNV = saStorageClass_ShaderRecordBufferKHR,
  saStorageClass_PhysicalStorageBufferEXT = saStorageClass_PhysicalStorageBuffer
};

// Used by OpTypeImage
enum sa__ImageDimmension_e {
  saImageDimmension_1D = 0,
  saImageDimmension_2D = 1,
  saImageDimmension_3D = 2,
  saImageDimmension_Cube = 3,
  saImageDimmension_Rect = 4,
  saImageDimmension_Buffer = 5,
  saImageDimmension_SubpassData = 6,
  saImageDimmension_TileImageDataEXT = 4173
};

// Used by OpConstantSampler
enum sa__OpSamplerAddressingMode_e {
  saSamplerAddressingMode_None = 0,
  saSamplerAddressingMode_ClampToEdge = 1,
  saSamplerAddressingMode_Clamp = 2,
  saSamplerAddressingMode_Repeat = 3,
  saSamplerAddressingMode_RepeatMirrored = 4
};

// Used by OpConstantSampler
enum sa__SamplerFilterMode_e {
  saSamplerFilterMode_Nearest = 0,
  saSamplerFilterMode_Linear = 1
};

// Used by OpTypeImage
enum sa__ImageFormat_e {
  saImageFormat_Unknown = 0,
  saImageFormat_Rgba32f = 1,
  saImageFormat_Rgba16f = 2,
  saImageFormat_R32f = 3,
  saImageFormat_Rgba8 = 4,
  saImageFormat_Rgba8Snorm = 5,
  saImageFormat_Rg32f = 6,
  saImageFormat_Rg16f = 7,
  saImageFormat_R11fG11fB10f = 8,
  saImageFormat_R16f = 9,
  saImageFormat_Rgba16 = 10,
  saImageFormat_Rgb10A2 = 11,
  saImageFormat_Rg16 = 12,
  saImageFormat_Rg8 = 13,
  saImageFormat_R16 = 14,
  saImageFormat_R8 = 15,
  saImageFormat_Rgba16Snorm = 16,
  saImageFormat_Rg16Snorm = 17,
  saImageFormat_Rg8Snorm = 18,
  saImageFormat_R16Snorm = 19,
  saImageFormat_R8Snorm = 20,
  saImageFormat_Rgba32i = 21,
  saImageFormat_Rgba16i = 22,
  saImageFormat_Rgba8i = 23,
  saImageFormat_R32i = 24,
  saImageFormat_Rg32i = 25,
  saImageFormat_Rg16i = 26,
  saImageFormat_Rg8i = 27,
  saImageFormat_R16i = 28,
  saImageFormat_R8i = 29,
  saImageFormat_Rgba32ui = 30,
  saImageFormat_Rgba16ui = 31,
  saImageFormat_Rgba8ui = 32,
  saImageFormat_R32ui = 33,
  saImageFormat_Rgb10a2ui = 34,
  saImageFormat_Rg32ui = 35,
  saImageFormat_Rg16ui = 36,
  saImageFormat_Rg8ui = 37,
  saImageFormat_R16ui = 38,
  saImageFormat_R8ui = 39,
  saImageFormat_R64ui = 40,
  saImageFormat_R64i = 41
};

// Used bu OpImageQueryOrder
enum sa__ImageChannelOrder_e {
  saImageChannelOrder_R = 0,
  saImageChannelOrder_A = 1,
  saImageChannelOrder_RG = 2,
  saImageChannelOrder_RA = 3,
  saImageChannelOrder_RGB = 4,
  saImageChannelOrder_RGBA = 5,
  saImageChannelOrder_BGRA = 6,
  saImageChannelOrder_ARGB = 7,
  saImageChannelOrder_Intensity = 8,
  saImageChannelOrder_Luminance = 9,
  saImageChannelOrder_Rx = 10,
  saImageChannelOrder_RGx = 11,
  saImageChannelOrder_RGBx = 12,
  saImageChannelOrder_Depth = 13,
  saImageChannelOrder_DepthStencil = 14,
  saImageChannelOrder_sRGB = 15,
  saImageChannelOrder_sRGBx = 16,
  saImageChannelOrder_sRGBA = 17,
  saImageChannelOrder_sBGRA = 18,
  saImageChannelOrder_ABGR = 19
};

// Used by OpImageQueryFormat
enum sa__ImageChannelDataType_e {
  saImageChannelDataFormat_SnormInt8 = 0,
  saImageChannelDataFormat_SnormInt16 = 1,
  saImageChannelDataFormat_UnormInt8 = 2,
  saImageChannelDataFormat_UnormInt16 = 3,
  saImageChannelDataFormat_UnormShort565 = 4,
  saImageChannelDataFormat_UnormShort555 = 5,
  saImageChannelDataFormat_UnormInt101010 = 6,
  saImageChannelDataFormat_SignedInt8 = 7,
  saImageChannelDataFormat_SignedInt16 = 8,
  saImageChannelDataFormat_SignedInt32 = 9,
  saImageChannelDataFormat_UnsignedInt8 = 10,
  saImageChannelDataFormat_UnsignedInt16 = 11,
  saImageChannelDataFormat_UnsignedInt32 = 12,
  saImageChannelDataFormat_HalfFloat = 13,
  saImageChannelDataFormat_Float = 14,
  saImageChannelDataFormat_UnormInt24 = 15,
  saImageChannelDataFormat_UnormInt101010_2 = 16,
  saImageChannelDataFormat_UnsignedIntRaw10EXT = 19,
  saImageChannelDataFormat_UnsignedIntRaw12EXT = 20,
  saImageChannelDataFormat_UnormInt2_101010EXT = 21,
  saImageChannelDataFormat_UnsignedInt10X6EXT = 22,
  saImageChannelDataFormat_UnsignedInt12X4EXT = 23,
  saImageChannelDataFormat_UnsignedInt14X2EXT = 24,
  saImageChannelDataFormat_UnormInt12X4EXT = 25,
  saImageChannelDataFormat_UnormInt14X2EXT = 26
};

// Used by: OpImageSampleImplicitLod, OpImageSampleExplicitLod, OpImageSampleDrefImplicitLod, OpImageSampleDrefExplicitLod, OpImageSampleProjImplicitLod, OpImageSampleProjExplicitLod, 
// OpImageSampleProjDrefImplicitLod, OpImageSampleProjDrefExplicitLod, OpImageFetch, OpImageGather, OpImageDrefGather, OpImageRead, OpImageWrite, OpImageSparseSampleImplicitLod, 
// OpImageSparseSampleExplicitLod, OpImageSparseSampleDrefImplicitLod, OpImageSparseSampleDrefExplicitLod, OpImageSparseFetch, OpImageSparseGather, OpImageSparseDrefGather, OpImageSparseRead, OpImageSampleFootprintNV
enum sa__ImageOperands_e {
  saImageOperands_None = 0x0,
  saImageOperands_Bias = 0x1,
  saImageOperands_Lod = 0x2,
  saImageOperands_Grad = 0x4,
  saImageOperands_ConstOffset = 0x8,
  saImageOperands_Offset = 0x10,
  saImageOperands_ConstOffsets = 0x20,
  saImageOperands_Sample = 0x40,
  saImageOperands_MinLod = 0x80,
  saImageOperands_MakeTexelAvailable = 0x100,
  saImageOperands_MakeTexelVisible = 0x200,
  saImageOperands_NonPrivateTexel = 0x400,
  saImageOperands_VolatileTexel = 0x800,
  saImageOperands_SignExtent = 0x1000,
  saImageOperands_ZeroExtent = 0x2000,
  saImageOperands_Nontemporal = 0x4000,
  saImageOperands_Offsets = 0x10000,
  
  saImageOperands_MakeTexelAvailableKHR = saImageOperands_MakeTexelAvailable,
  saImageOperands_MakeTexelVisibleKHR = saImageOperands_MakeTexelVisible,
  saImageOperands_NonPrivateTexelKHR = saImageOperands_NonPrivateTexel,
  saImageOperands_VolatileTexelKHR = saImageOperands_VolatileTexel
};

// Used by: OpFAdd, OpFSub, OpFMul, OpFDiv, OpFRem, OpFMod
// SPIR-V 1.6:
// OpFNegate, OpOrdered, OpUnordered, OpFOrdEqual, OpFUnordEqual, OpFOrdNotEqual,
// OpFUnordNotEqual, OpFOrdLessThan, OpFUnordLessThan, OpFOrdGreaterThan,
// OpFUnordGreaterThan, OpFOrdLessThanEqual, OpFUnordLessThanEqual,
// OpFOrdGreaterThanEqual, OpFUnordGreaterThanEqual, OpExtInst
enum sa__FPFastMathMode_e {
  saFPFastMathMode_None = 0x0,
  saFPFastMathMode_NotNan = 0x1,
  saFPFastMathMode_NotInf = 0x2,
  saFPFastMathMode_NSZ = 0x4,
  saFPFastMathMode_AllowReciprocal = 0x8,
  saFPFastMathMode_Fast = 0x10,
  saFPFastMathMode_AllowContract = 0x10000,
  saFPFastMathMode_AllowReassoc = 0x20000,
  saFPFastMathMode_AllowTransform = 0x40000,

  saFPFastMathMode_AllowContractINTEL = saFPFastMathMode_AllowContract,
  saFPFastMathMode_AllowReassocINTEL = saFPFastMathMode_AllowReassoc,
};

enum sa__FPRoundingMode_e {
  saFPRoundingMode_ToNearestEven = 0,
  saFPRoundingMode_ToZero = 1,
  saFPRoundingMode_ToPositiveInfinity = 2,
  saFPRoundingMode_ToNegativeInfinity = 3
};

enum sa__LinkageType_e {
  saLinkageType_Export = 0,
  saLinkageType_Import = 1,
  saLinkageType_LinkOnceODR = 2
};

enum sa__AccessQualifiers_e {
  saAccessQualifiers_ReadOnly = 0,
  saAccessQualifiers_WriteOnly = 1,
  saAccessQualifiers_ReadWrite = 2
};

enum sa__FunctionParameterAttrib_e {
  saFunctionParameterAttrib_ZeroExtend = 0,
  saFunctionParameterAttrib_SignExtend = 1,
  saFunctionParameterAttrib_ByValue = 2,
  saFunctionParameterAttrib_StructReturn = 3,
  saFunctionParameterAttrib_NoAlias = 4,
  saFunctionParameterAttrib_NoCaptute = 5,
  saFunctionParameterAttrib_NoWrite = 6,
  saFunctionParameterAttrib_NoReadWrite = 7,
};

enum sa__Decoration_e {
  saDecoration_RelaxedPrecision = 0,
  saDecoration_SpecId = 1,
  saDecoration_Block = 2,
  saDecoration_BufferBlock = 3,
  saDecoration_RowMajor = 4,
  saDecoration_ColMajor = 5,
  saDecoration_ArrayStride = 6,
  saDecoration_MatrixStride = 7,
  saDecoration_GLSLShared = 8,
  saDecoration_GLSLPacked = 9,
  saDecoration_CPacked = 10,
  saDecoration_BuiltIn = 11,
  saDecoration_NoPerspective = 13,
  saDecoration_Flat = 14,
  saDecoration_Patch = 15,
  saDecoration_Centroid = 16,
  saDecoration_Sample = 17,
  saDecoration_Invariant = 18,
  saDecoration_Restrict = 19,
  saDecoration_Aliased = 20,
  saDecoration_Volatile = 21,
  saDecoration_Constant = 22,
  saDecoration_Coherent = 23,
  saDecoration_NonWritable = 24,
  saDecoration_NonReadable = 25,
  saDecoration_Uniform = 26,
  saDecoration_UniformId = 27,
  saDecoration_SaturatedConversion = 28,
  saDecoration_Stream = 29,
  saDecoration_Location = 30,
  saDecoration_Component = 31,
  saDecoration_Index = 32,
  saDecoration_Binding = 33,
  saDecoration_DescriptorSet = 34,
  saDecoration_Offset = 35,
  saDecoration_XfbBuffer = 36,
  saDecoration_XfbStride = 37,
  saDecoration_FuncParamAttrib = 38,
  saDecoration_FPRoundingMode = 39,
  saDecoration_FPFastMathMode = 40,
  saDecoration_LinkageAttribs = 41,
  saDecoration_NoContraction = 42,
  saDecoration_InputAttachmentIndex = 43,
  saDecoration_Alignment = 44,
  saDecoration_MaxByteOffset = 45,
  saDecoration_AlignmentId = 46,
  saDecoration_MaxByteOffsetId = 47,
};

enum sa_DecorationBuiltIns_e {
  saDecorationBuiltIn_Position = 0,
  saDecorationBuiltIn_PointSize = 1,
  saDecorationBuiltIn_ClipDistance = 3,
  saDecorationBuiltIn_CullDistance = 4,
  saDecorationBuiltIn_VertexId = 5,
  saDecorationBuiltIn_InstanceId = 6,
  saDecorationBuiltIn_PrimitiveId = 7,
  saDecorationBuiltIn_InvocationId = 8,
  saDecorationBuiltIn_Layer = 9,
  saDecorationBuiltIn_ViewportIndex = 10,
  saDecorationBuiltIn_TessLevelOuter = 11,
  saDecorationBuiltIn_TessLevelInner = 12,
  saDecorationBuiltIn_TessCoord = 13,
  saDecorationBuiltIn_PatchVertices = 14,
  saDecorationBuiltIn_FragCoord = 15,
  saDecorationBuiltIn_PointCoord = 16,
  saDecorationBuiltIn_FrontFacing = 17,
  saDecorationBuiltIn_SampleId = 18,
  saDecorationBuiltIn_SamplePosition = 19,
  saDecorationBuiltIn_SampleMask = 20,
  saDecorationBuiltIn_FragDepth = 22,
  saDecorationBuiltIn_HelperInvocation = 23,
  saDecorationBuiltIn_NumWorkgroups = 24,
  saDecorationBuiltIn_WorkgroupSize = 25,
  saDecorationBuiltIn_WorkgroupId = 26,
  saDecorationBuiltIn_LocalInvocationId = 27,
  saDecorationBuiltIn_GlobalInvocationId = 28,
  saDecorationBuiltIn_LocalInvocationIndex = 29,
  saDecorationBuiltIn_WorkDim = 30,
  saDecorationBuiltIn_GlobalSize = 31,
  saDecorationBuiltIn_EnqueuedWorkgroupSize = 32,
  saDecorationBuiltIn_GlobalOffset = 33,
  saDecorationBuiltIn_GlobalLinearId = 34,
  saDecorationBuiltIn_SubgroupSize = 36,
  saDecorationBuiltIn_SubgroupMaxSize = 37,
  saDecorationBuiltIn_NumSubgroups = 38,
  saDecorationBuiltIn_NumEnqueuedSubgroups = 39,
  saDecorationBuiltIn_SubgroupId = 40,
  saDecorationBuiltIn_SubgroupLocalInvocationId = 41,
  saDecorationBuiltIn_VertexIndex = 42,
  saDecorationBuiltIn_InstanceIndex = 43
};

enum sa__SelectionControl_e {
  saSelectionControl_None = 0x0,
  saSelectionControl_Flatten = 0x1,
  saSelectionControl_DontFlatten = 0x2
};

enum sa__LoopControl_e {
  saLoopControl_None = 0x0,
  saLoopControl_Unroll = 0x1,
  saLoopControl_DontUnroll = 0x2,
  saLoopControl_DependencyInfinite = 0x4,
  saLoopControl_DependencyLength = 0x8,
  saLoopControl_MinIterations = 0x10,
  saLoopControl_MaxIterations = 0x20,
  saLoopControl_IterationMultiple = 0x40,
  saLoopControl_PeelCount = 0x80,
  saLoopControl_PartialCount = 0x100
};

enum sa__FunctionControl_e {
  saFunctionControl_None = 0x0,
  saFunctionControl_Inline = 0x1,
  saFunctionControl_DontInline = 0x2,
  saFunctionControl_Pure = 0x4,
  saFunctionControl_Const = 0x8
};

enum sa__MemorySemantics_e {
  saMemorySemantics_None = 0x0,
  saMemorySemantics_Acquire = 0x2,
  saMemorySemantics_Release = 0x4,
  saMemorySemantics_AcquireRelease = 0x8,
  saMemorySemantics_SequentiallyConsistent = 0x10,
  saMemorySemantics_UniformMemory = 0x40,
  saMemorySemantics_SubgroupMemory = 0x80,
  saMemorySemantics_WorkgroupMemory = 0x100,
  saMemorySemantics_CrossWorkgroupMemory = 0x200,
  saMemorySemantics_AtomicCounterMemory = 0x400,
  saMemorySemantics_ImageMemory = 0x800,
  saMemorySemantics_OutputMemory = 0x1000,
  saMemorySemantics_MakeAvailable = 0x2000,
  saMemorySemantics_MakeVisible = 0x4000,
  saMemorySemantics_Volatile = 0x8000
};

enum sa__MemoryOperands_e {
  saMemoryOperands_None = 0x0,
  saMemoryOperands_Volatile = 0x1,
  saMemoryOperands_Aligned = 0x2,
  saMemoryOperands_Nontemporal = 0x4,
  saMemoryOperands_MakePointerAvailable = 0x8,
  saMemoryOperands_MakePointerVisible = 0x10,
  saMemoryOperands_NonPrivatePointer = 0x20
};

enum sa__Scope_e {
  saScope_CrossDevice = 0,
  saScope_Device = 1,
  saScope_Workgroup = 2,
  saScope_Subgroup = 3,
  saScope_Invocation = 4,
  saScope_QueueFamily = 5,
  saScope_ShaderCallKHR = 6
};

enum sa__GroupOperation_e {
  saGroupOperation_Reduce = 0,
  saGroupOperation_InclusiveScan = 1,
  saGroupOperation_ExclusiveScan = 2,
  saGroupOperation_ClusterReduce = 3
};

enum sa__KernelEnqueueFlags_e {
  saKernelEnqueueFlag_NoWait = 0,
  saKernelEnqueueFlag_WaitKernels = 1,
  saKernelEnqueueFlag_WaitWorkgroup = 2
};

enum sa__KernelProfilingInfo_e {
  saKernelProfilingInfo_None = 0x0,
  saKernelProfilingInfo_CmdExecTime = 0x1
};

// Used by OpCapability
// XXX: For now there are only needed capabilities
enum sa__Capability_e {
  // Basically useless, because it is defined with Shader capability
  saCapability_Matrix = 0,
  // Used by vertex, fragment and compute shaders
  saCapability_Shader = 1,
  saCapability_Geometry = 2,
  saCapability_Tesselation = 3,
  // TODO: Add all of capabilities
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
  saOp_GroupDecorate = 74,
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
  saOp_SConvert = 114,
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
  saOp_NoLine = 317,
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

/*
  saOp_Nop = 0, nop
  saOp_ExtInstImport = 11, import
  saOp_ExtInst = 12, @
  saOp_EntryPoint = 15, entrypoint
  saOp_TypeVoid = 19, type VOID
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
  saOp_TypeForwardPointer = 39,
  saOp_ConstantTrue = 41, true
  saOp_ConstantFalse = 42, false
  saOp_Constant = 43, const or automaticly declared
  saOp_ConstantComposite = 44,
  saOp_ConstantSampler = 45,
  saOp_ConstantNull = 46, null
  saOp_SpecConstantTrue = 48,
  saOp_SpecConstantFalse = 49,
  saOp_SpecConstant = 50,
  saOp_SpecConstantComposite = 51,
  saOp_SpecConstantOp = 52,
  saOp_Function = 54, fn
  saOp_FunctionParameter = 55, ()
  saOp_FunctionEnd = 56, endfn
  saOp_FunctionCall = 57, @
  saOp_Variable = 59,
  saOp_ImageTexelPointer = 60,
  saOp_Load = 61, load
  saOp_Store = 62, store
  saOp_CopyMemory = 63, copymemory
  saOp_CopyMemorySized = 64, copymemorysized
  saOp_AccessChain = 65,
  saOp_InBoundsAccessChain = 66,
  saOp_PtrAccessChain = 67,
  saOp_ArrayLength = 68,
  saOp_InBoundsPtrAccessChain = 70,
  saOp_Decorate = 71, decorate
  saOp_MemberDecorate = 72,
  saOp_DecorationGroup = 73,
  saOp_GroupDecorate = 74,
  saOp_GroupMemberDecorate = 75,
  saOp_VectorExtractDynamic = 77,
  saOp_VectorInsertDynamic = 78,
  saOp_VectorShuffle = 79,
  saOp_CompositeConstruct = 80,
  saOp_CompositeExtract = 81,
  saOp_CompositeInsert = 82,
  saOp_CopyObject = 83, copyobject
  saOp_Transpose = 84, transpose
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
  saOp_ImageRead = 98, imageread
  saOp_ImageWrite = 99, imagewrite
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
  saOp_SConvert = 114,
  saOp_FConvert = 115,
  saOp_QuantizeToF16 = 116,
  saOp_ConvertPtrToU = 117,
  saOp_SatConvertSToU = 118,
  saOp_SatConvertUToS = 119,
  saOp_ConvertUToPtr = 120,
  saOp_Bitcast = 124, bitcast
  saOp_OuterProduct = 147, outer
  saOp_Dot = 148, dot
  saOp_Any = 154, any
  saOp_All = 155, all
  saOp_IsNan = 156, isnan
  saOp_IsInf = 157, isinf
  saOp_LessOrGreater = 161,
  saOp_Ordered = 162,
  saOp_Unordered = 163,
  saOp_BitwiseOr = 197, |
  saOp_BitwiseXor = 198, ^
  saOp_BitwiseAnd = 199, &
  saOp_Not = 200, ~
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
  saOp_EmitVertex = 218, emitvertex
  saOp_EndPrimitive = 219, emitprimitive
  saOp_EmitStreamVertex = 220, emitstreamvertex
  saOp_EndStreamPrimitive = 221, endstreamprimitive
  saOp_ControlBarrier = 224,
  saOp_MemoryBarrier = 225,
  saOp_AtomicLoad = 227, atomload
  saOp_AtomicStore = 228, atomstore
  saOp_Phi = 245,
  saOp_LoopMerge = 246,
  saOp_SelectionMerge = 247,
  saOp_Label = 248, :
  saOp_Branch = 249, branch
  saOp_BranchConditional = 250, branchcond
  saOp_Switch = 251, switch
  saOp_Kill = 252, kill
  saOp_Return = 253, return
  saOp_ReturnValue = 254, return x
  saOp_Unreachable = 255,
  saOp_LifetimeStart = 256,
  saOp_LifetimeStop = 257,
  saOp_GroupAsyncCopy = 259,
  saOp_GroupWaitEvents = 260,
  saOp_GroupAll = 261,
  saOp_GroupAny = 262,
  saOp_GroupBroadcast = 263,
  saOp_ImageSparseSampleImplicitLod = 305,
  saOp_ImageSparseSampleExplicitLod = 306,
  saOp_ImageSparseSampleDrefImplicitLod = 307,
  saOp_ImageSparseSampleDrefExplicitLod = 308,
  saOp_ImageSparseFetch = 313,
  saOp_ImageSparseGather = 314,
  saOp_ImageSparseDrefGather = 315,
  saOp_ImageSparseTexelResident = 316,
  saOp_AtomicFlagTestAndSet = 318, atomftest
  saOp_AtomicFlagClear = 319, atomfclear
  saOp_ImageSparseRead = 320,
  saOp_SizeOf = 321, sizeof
  saOp_CopyLogical = 400,
  saOp_PtrEqual = 401,
  saOp_PtrNotEqual = 402,
  saOp_PtrDiff = 403
*/

enum sa__AssemblerLowLevelEnum {
  saAsmEnum_EntryPoint = 0,
  saAsmEnum_StorageClass,
  saAsmEnum_ImageDimmension,
  saAsmEnum_SamplerAddressingMode,
  saAsmEnum_SamplerFilterMode,
  saAsmEnum_ImageFormat,
  saAsmEnum_ImageChannelOrder,
  saAsmEnum_ImageChannelDataFormat,
  saAsmEnum_ImageOperand,
  saAsmEnum_FPFastMath,
  saAsmEnum_FPRoundingMode,
  saAsmEnum_LinkageType,
  saAsmEnum_AccessQualifier,
  saAsmEnum_FunctionParameterAttrib,
  saAsmEnum_Decoration,
  saAsmEnum_DecorationBuiltIn,
  saAsmEnum_SelectionControl,
  saAsmEnum_LoopControl,
  saAsmEnum_FunctionControl,
  saAsmEnum_MemorySemantics,
  saAsmEnum_MemoryOperand,
  saAsmEnum_Scope,
  saAsmEnum_GroupOperation,
  saAsmEnum_ExecutionMode,

  saAsmEnum_COUNT
};

enum sa__AssemblySectionType_e {
  saSectionType_Capability = 0,
  saSectionType_Extensions,
  saSectionType_Imports,
  saSectionType_MemoryModel,
  saSectionType_EntryPoints,
  saSectionType_ExecutionModes,
  saSectionType_Debug,
  saSectionType_Annotations,
  saSectionType_Types,
  saSectionType_Functions,
  saSectionType_COUNT
};

enum sa__Tokens_e {
  saToken_None = 0,
  saToken_Identificator,
  saToken_Import,
  saToken_Module,
  saToken_Type,
  saToken_InputVar,
  saToken_OutputVar,
  saToken_UniformVar,
  saToken_PrivateVar,
  saToken_FunctionVar,

};

// TODO: Lexer for SBA
/*typedef struct sa__token_t {
  char tokenId[512];
  sa_uint32_t token;
} sa__token_t;

typedef struct sa__lexer_s {

} sa__lexer_t;*/

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

typedef struct sa__assemblySection_s {
  sa__assemblyInstruction_t* pInst;
  sa_uint32_t instCount;
} sa__assemblySection_t;

typedef struct sa_assembly_s {
  sa__assemblyHeader_t header;
  sa__assemblySection_t section[saSectionType_COUNT];
} sa_assembly_t;

struct sa__assemblerErrorMessages_s {
  char** pMessages;
  sa_uint32_t messagesAmount;
};

struct sa__assemblerLowLevelOpCodeConnection_s {
  const char* opcodeMnemonic;
  const sa_uint32_t opcode;
  const sa_uint32_t wordCount;
  const sa_bool plusVariable;
  // Could be that all are possible, SA_UINT32_MAX mean end of the values
  const sa_uint32_t possibleEnumerant[saAsmEnum_COUNT];
};

struct sa__assemblerLowLevelEnumerantConnection_s {
  const char* enumerantMnemonic;
  sa_uint32_t enumerant;
};

static sa_uint32_t __gIdGeneratorHoldValue = 0;
static struct sa__assemblerErrorMessages_s __gAssemblerErrorMessages = {0};

const struct sa__assemblerLowLevelOpCodeConnection_s SA_ASSEMBLER_LOW_LEVEL_OPCODES[] = {
  // semantic,                                  opcode,                                       argc, +var
  { "Nop",                                      saOp_Nop,                                       1, SA_FALSE, { SA_UINT32_MAX } },
  { "Undef",                                    saOp_Undef,                                     3, SA_FALSE, { SA_UINT32_MAX } },
  { "SourceContinued",                          saOp_SourceContinued,                           2, SA_TRUE,  { SA_UINT32_MAX } },
  { "Source",                                   saOp_Source,                                    3, SA_TRUE,  { SA_UINT32_MAX } },
  { "SourceExtension",                          saOp_SourceExtension,                           2, SA_TRUE,  { SA_UINT32_MAX } },
  { "Name",                                     saOp_Name,                                      3, SA_TRUE,  { SA_UINT32_MAX } },
  { "MemberName",                               saOp_MemberName,                                4, SA_TRUE,  { SA_UINT32_MAX } },
  { "String",                                   saOp_String,                                    3, SA_TRUE,  { SA_UINT32_MAX } },
  { "Line",                                     saOp_Line,                                      4, SA_FALSE, { SA_UINT32_MAX } },
  { "Extension",                                saOp_Extension,                                 2, SA_TRUE,  { SA_UINT32_MAX } },
  { "ExtInstImport",                            saOp_ExtInstImport,                             3, SA_TRUE,  { SA_UINT32_MAX } },
  { "ExtInst",                                  saOp_ExtInst,                                   5, SA_TRUE,  { SA_UINT32_MAX } },
  { "MemoryModel",                              saOp_MemoryModel,                               3, SA_FALSE, { SA_UINT32_MAX } },
  { "EntryPoint",                               saOp_EntryPoint,                                4, SA_TRUE,  { saAsmEnum_EntryPoint, SA_UINT32_MAX } },
  { "ExecutionMode",                            saOp_ExecutionMode,                             3, SA_TRUE,  { saAsmEnum_ExecutionMode, SA_UINT32_MAX /*XXX: Possibly execution mode later*/ } },
  { "Capability",                               saOp_Capability,                                2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeVoid",                                 saOp_TypeVoid,                                  2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeBool",                                 saOp_TypeBool,                                  2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeInt",                                  saOp_TypeInt,                                   4, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeFloat",                                saOp_TypeFloat,                                 3, SA_TRUE,  { SA_UINT32_MAX } },
  { "TypeVector",                               saOp_TypeVector,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeMatrix",                               saOp_TypeMatrix,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeImage",                                saOp_TypeImage,                                 9, SA_TRUE,  { saAsmEnum_ImageFormat, saAsmEnum_ImageDimmension, saAsmEnum_AccessQualifier, SA_UINT32_MAX } },
  { "TypeSampler",                              saOp_TypeSampler,                               2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeSampledImage",                         saOp_TypeSampledImage,                          3, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeArray",                                saOp_TypeArray,                                 4, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeRuntimeArray",                         saOp_TypeRuntimeArray,                          3, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeStruct",                               saOp_TypeStruct,                                2, SA_TRUE,  { SA_UINT32_MAX } },
  { "TypeOpaque",                               saOp_TypeOpaque,                                3, SA_TRUE,  { SA_UINT32_MAX } },
  { "TypePointer",                              saOp_TypePointer,                               4, SA_FALSE, { saAsmEnum_StorageClass, SA_UINT32_MAX } },
  { "TypeFunction",                             saOp_TypeFunction,                              3, SA_TRUE,  { SA_UINT32_MAX } },
  { "TypeEvent",                                saOp_TypeEvent,                                 2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeDeviceEvent",                          saOp_TypeDeviceEvent,                           2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeReserveId",                            saOp_TypeReserveId,                             2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeQueue",                                saOp_TypeQueue,                                 2, SA_FALSE, { SA_UINT32_MAX } },
  { "TypePipe",                                 saOp_TypePipe,                                  3, SA_FALSE, { saAsmEnum_AccessQualifier, SA_UINT32_MAX } },
  { "TypeForwardPointer",                       saOp_TypeForwardPointer,                        3, SA_FALSE, { saAsmEnum_StorageClass, SA_UINT32_MAX } },
  { "ConstantTrue",                             saOp_ConstantTrue,                              3, SA_FALSE, { SA_UINT32_MAX } },
  { "ConstantFalse",                            saOp_ConstantFalse,                             3, SA_FALSE, { SA_UINT32_MAX } },
  { "Constant",                                 saOp_Constant,                                  4, SA_TRUE,  { SA_UINT32_MAX } },
  { "ConstantComposite",                        saOp_ConstantComposite,                         3, SA_TRUE,  { SA_UINT32_MAX } },
  { "ConstantSampler",                          saOp_ConstantSampler,                           6, SA_FALSE, { saAsmEnum_SamplerAddressingMode, saAsmEnum_SamplerFilterMode, SA_UINT32_MAX } },
  { "ConstantNull",                             saOp_ConstantNull,                              3, SA_FALSE, { SA_UINT32_MAX } },
  { "SpecConstantTrue",                         saOp_SpecConstantTrue,                          3, SA_FALSE, { SA_UINT32_MAX } },
  { "SpecConstantFalse",                        saOp_SpecConstantFalse,                         3, SA_FALSE, { SA_UINT32_MAX } },
  { "SpecConstant",                             saOp_SpecConstant,                              4, SA_TRUE,  { SA_UINT32_MAX } },
  { "SpecConstantComposite",                    saOp_SpecConstantComposite,                     3, SA_TRUE,  { SA_UINT32_MAX } },
  { "SpecConstantOp",                           saOp_SpecConstantOp,                            4, SA_TRUE,  { SA_UINT32_MAX } },
  { "Function",                                 saOp_Function,                                  5, SA_FALSE, { saAsmEnum_FunctionControl, SA_UINT32_MAX } },
  { "FunctionParameter",                        saOp_FunctionParameter,                         3, SA_FALSE, { SA_UINT32_MAX } },
  { "FunctionEnd",                              saOp_FunctionEnd,                               1, SA_FALSE, { SA_UINT32_MAX } },
  { "FunctionCall",                             saOp_FunctionCall,                              4, SA_TRUE,  { SA_UINT32_MAX } },
  { "Variable",                                 saOp_Variable,                                  4, SA_TRUE,  { saAsmEnum_StorageClass, SA_UINT32_MAX } },
  { "ImageTexelPointer",                        saOp_ImageTexelPointer,                         6, SA_FALSE, { SA_UINT32_MAX } },
  { "Load",                                     saOp_Load,                                      4, SA_TRUE,  { saAsmEnum_MemoryOperand, SA_UINT32_MAX } },
  { "Store",                                    saOp_Store,                                     3, SA_TRUE,  { saAsmEnum_MemoryOperand, SA_UINT32_MAX } },
  { "CopyMemory",                               saOp_CopyMemory,                                3, SA_TRUE,  { saAsmEnum_MemoryOperand, SA_UINT32_MAX } },
  { "CopyMemorySized",                          saOp_CopyMemorySized,                           4, SA_TRUE,  { saAsmEnum_MemoryOperand, SA_UINT32_MAX } },
  { "AccessChain",                              saOp_AccessChain,                               4, SA_TRUE,  { SA_UINT32_MAX } },
  { "InBoundsAccessChain",                      saOp_InBoundsAccessChain,                       4, SA_TRUE,  { SA_UINT32_MAX } },
  { "PtrAccessChain",                           saOp_PtrAccessChain,                            5, SA_TRUE,  { SA_UINT32_MAX } },
  { "ArrayLength",                              saOp_ArrayLength,                               5, SA_FALSE, { SA_UINT32_MAX } },
  { "GenericPtrMemSemantics",                   saOp_GenericPtrMemSemantics,                    4, SA_FALSE, { SA_UINT32_MAX } },
  { "InBoundsPtrAccessChain",                   saOp_InBoundsPtrAccessChain,                    5, SA_TRUE,  { SA_UINT32_MAX } },
  { "Decorate",                                 saOp_Decorate,                                  3, SA_TRUE,  { saAsmEnum_Decoration, saAsmEnum_DecorationBuiltIn, SA_UINT32_MAX } },
  { "MemberDecorate",                           saOp_MemberDecorate,                            4, SA_TRUE,  { saAsmEnum_Decoration, saAsmEnum_DecorationBuiltIn, SA_UINT32_MAX } },
  { "DecorationGroup",                          saOp_DecorationGroup,                           2, SA_FALSE, { SA_UINT32_MAX } },
  { "GroupDecorate",                            saOp_GroupDecorate,                             2, SA_TRUE,  { SA_UINT32_MAX } },
  { "GroupMemberDecorate",                      saOp_GroupMemberDecorate,                       2, SA_TRUE,  { SA_UINT32_MAX } },
  { "VectorExtractDynamic",                     saOp_VectorExtractDynamic,                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "VectorInsertDynamic",                      saOp_VectorInsertDynamic,                       6, SA_FALSE, { SA_UINT32_MAX } },
  { "VectorShuffle",                            saOp_VectorShuffle,                             5, SA_TRUE,  { SA_UINT32_MAX } },
  { "CompositeConstruct",                       saOp_CompositeConstruct,                        3, SA_TRUE,  { SA_UINT32_MAX } },
  { "CompositeExtract",                         saOp_CompositeExtract,                          4, SA_TRUE,  { SA_UINT32_MAX } },
  { "CompositeInsert",                          saOp_CompositeInsert,                           5, SA_TRUE,  { SA_UINT32_MAX } },
  { "CopyObject",                               saOp_CopyObject,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "Transpose",                                saOp_Transpose,                                 4, SA_FALSE, { SA_UINT32_MAX } },
  { "SampledImage",                             saOp_SampledImage,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageSampleImplicitLod",                   saOp_ImageSampleImplicitLod,                    5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleExplicitLod",                   saOp_ImageSampleExplicitLod,                    7, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleDrefImplicitLod",               saOp_ImageSampleDrefImplicitLod,                6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleDrefExplicitLod",               saOp_ImageSampleDrefExplicitLod,                8, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleProjImplicitLod",               saOp_ImageSampleProjImplicitLod,                5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleProjExplicitLod",               saOp_ImageSampleProjExplicitLod,                7, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleProjDrefImplicitLod",           saOp_ImageSampleProjDrefImplicitLod,            6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSampleProjDrefExplicitLod",           saOp_ImageSampleProjDrefExplicitLod,            8, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageFetch",                               saOp_ImageFetch,                                5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageGather",                              saOp_ImageGather,                               6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageDrefGather",                          saOp_ImageDrefGather,                           6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageRead",                                saOp_ImageRead,                                 5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageWrite",                               saOp_ImageWrite,                                4, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "Image",                                    saOp_Image,                                     4, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQueryFormat",                         saOp_ImageQueryFormat,                          4, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQueryOrder",                          saOp_ImageQueryOrder,                           4, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQuerySizeLod",                        saOp_ImageQuerySizeLod,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQuerySize",                           saOp_ImageQuerySize,                            4, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQueryLod",                            saOp_ImageQueryLod,                             5, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQueryLevels",                         saOp_ImageQueryLevels,                          4, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageQuerySamples",                        saOp_ImageQuerySamples,                         4, SA_FALSE, { SA_UINT32_MAX } },
  { "ConvertFToU",                              saOp_ConvertFToU,                               4, SA_FALSE, { SA_UINT32_MAX } },
  { "ConvertFToS",                              saOp_ConvertFToS,                               4, SA_FALSE, { SA_UINT32_MAX } },
  { "ConvertSToF",                              saOp_ConvertSToF,                               4, SA_FALSE, { SA_UINT32_MAX } },
  { "ConvertUToF",                              saOp_ConvertUToF,                               4, SA_FALSE, { SA_UINT32_MAX } },
  { "UConvert",                                 saOp_UConvert,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "SConvert",                                 saOp_SConvert,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "FConvert",                                 saOp_FConvert,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "QuantizeToF16",                            saOp_QuantizeToF16,                             4, SA_FALSE, { SA_UINT32_MAX } },
  { "ConvertPtrToU",                            saOp_ConvertPtrToU,                             4, SA_FALSE, { SA_UINT32_MAX } },
  { "SatConvertSToU",                           saOp_SatConvertSToU,                            4, SA_FALSE, { SA_UINT32_MAX } },
  { "SatConvertUToS",                           saOp_SatConvertUToS,                            4, SA_FALSE, { SA_UINT32_MAX } },
  { "ConvertUToPtr",                            saOp_ConvertUToPtr,                             4, SA_FALSE, { SA_UINT32_MAX } },
  { "PtrCastToGeneric",                         saOp_PtrCastToGeneric,                          4, SA_FALSE, { SA_UINT32_MAX } },
  { "GenericCastToPtr",                         saOp_GenericCastToPtr,                          4, SA_FALSE, { SA_UINT32_MAX } },
  { "GenericCastToPtrExplicit",                 saOp_GenericCastToPtrExplicit,                  5, SA_FALSE, { saAsmEnum_StorageClass, SA_UINT32_MAX } },
  { "Bitcast",                                  saOp_Bitcast,                                   4, SA_FALSE, { SA_UINT32_MAX } },
  { "SNegate",                                  saOp_SNegate,                                   4, SA_FALSE, { SA_UINT32_MAX } },
  { "FNegate",                                  saOp_FNegate,                                   4, SA_FALSE, { SA_UINT32_MAX } },
  { "IAdd",                                     saOp_IAdd,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FAdd",                                     saOp_FAdd,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "ISub",                                     saOp_ISub,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FSub",                                     saOp_FSub,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "IMul",                                     saOp_IMul,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FMul",                                     saOp_FMul,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "UDiv",                                     saOp_UDiv,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "SDiv",                                     saOp_SDiv,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FDiv",                                     saOp_FDiv,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "UMul",                                     saOp_UMul,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "SRem",                                     saOp_SRem,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "SMod",                                     saOp_SMod,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FRem",                                     saOp_FRem,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FMod",                                     saOp_FMod,                                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "VectorTimesScalar",                        saOp_VectorTimesScalar,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "MatrixTimesScalar",                        saOp_MatrixTimesScalar,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "VectorTimesMatrix",                        saOp_VectorTimesMatrix,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "MatrixTimesVector",                        saOp_MatrixTimesVector,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "MatrixTimesMatrix",                        saOp_MatrixTimesMatrix,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "OuterProduct",                             saOp_OuterProduct,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "Dot",                                      saOp_Dot,                                       5, SA_FALSE, { SA_UINT32_MAX } },
  { "IAddCarry",                                saOp_IAddCarry,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "ISubBorrow",                               saOp_ISubBorrow,                                5, SA_FALSE, { SA_UINT32_MAX } },
  { "UMulExtended",                             saOp_UMulExtended,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "SMulExtended",                             saOp_SMulExtended,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "Any",                                      saOp_Any,                                       4, SA_FALSE, { SA_UINT32_MAX } },
  { "All",                                      saOp_All,                                       4, SA_FALSE, { SA_UINT32_MAX } },
  { "IsNan",                                    saOp_IsNan,                                     4, SA_FALSE, { SA_UINT32_MAX } },
  { "IsInf",                                    saOp_IsInf,                                     4, SA_FALSE, { SA_UINT32_MAX } },
  { "IsFinite",                                 saOp_IsFinite,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "IsNormal",                                 saOp_IsNormal,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "SignBitSet",                               saOp_SignBitSet,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "LessOrGreater",                            saOp_LessOrGreater,                             5, SA_FALSE, { SA_UINT32_MAX } },
  { "Ordered",                                  saOp_Ordered,                                   5, SA_FALSE, { SA_UINT32_MAX } },
  { "Unordered",                                saOp_Unordered,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "LogicalEqual",                             saOp_LogicalEqual,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "LogicalNotEqual",                          saOp_LogicalNotEqual,                           5, SA_FALSE, { SA_UINT32_MAX } },
  { "LogicalOr",                                saOp_LogicalOr,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "LogicalAnd",                               saOp_LogicalAnd,                                5, SA_FALSE, { SA_UINT32_MAX } },
  { "LogicalNot",                               saOp_LogicalNot,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "Select",                                   saOp_Select,                                    6, SA_FALSE, { SA_UINT32_MAX } },
  { "IEqual",                                   saOp_IEqual,                                    5, SA_FALSE, { SA_UINT32_MAX } },
  { "INotEqual",                                saOp_INotEqual,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "UGreaterThan",                             saOp_UGreaterThan,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "SGreaterThan",                             saOp_SGreaterThan,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "UGreaterThanEqual",                        saOp_UGreaterThanEqual,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "SGreaterThanEqual",                        saOp_SGreaterThanEqual,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "ULessThan",                                saOp_ULessThan,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "SLessThan",                                saOp_SLessThan,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "ULessThanEqual",                           saOp_ULessThanEqual,                            5, SA_FALSE, { SA_UINT32_MAX } },
  { "SLessThanEqual",                           saOp_SLessThanEqual,                            5, SA_FALSE, { SA_UINT32_MAX } },
  { "FOrdEqual",                                saOp_FOrdEqual,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "FUnordEqual",                              saOp_FUnordEqual,                               5, SA_FALSE, { SA_UINT32_MAX } },
  { "FOrdNotEqual",                             saOp_FOrdNotEqual,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "FUnordNotEqual",                           saOp_FUnordNotEqual,                            5, SA_FALSE, { SA_UINT32_MAX } },
  { "FOrdLessThan",                             saOp_FOrdLessThan,                              5, SA_FALSE, { SA_UINT32_MAX } },
  { "FUnordLessThan",                           saOp_FUnordLessThan,                            5, SA_FALSE, { SA_UINT32_MAX } },
  { "FOrdGreaterThan",                          saOp_FOrdGreaterThan,                           5, SA_FALSE, { SA_UINT32_MAX } },
  { "FUnordGreaterThan",                        saOp_FUnordGreaterThan,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "FOrdLessThanEqual",                        saOp_FOrdLessThanEqual,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "FUnordLessThanEqual",                      saOp_FUnordLessThanEqual,                       5, SA_FALSE, { SA_UINT32_MAX } },
  { "FOrdGreaterThanEqual",                     saOp_FOrdGreaterThanEqual,                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "FUnordGreaterThanEqual",                   saOp_FUnordGreaterThanEqual,                    5, SA_FALSE, { SA_UINT32_MAX } },
  { "ShiftRightLogical",                        saOp_ShiftRightLogical,                         5, SA_FALSE, { SA_UINT32_MAX } },
  { "ShiftRightArithmetic",                     saOp_ShiftRightArithmetic,                      5, SA_FALSE, { SA_UINT32_MAX } },
  { "ShiftLeftLogical",                         saOp_ShiftLeftLogical,                          5, SA_FALSE, { SA_UINT32_MAX } },
  { "BitwiseOr",                                saOp_BitwiseOr,                                 5, SA_FALSE, { SA_UINT32_MAX } },
  { "BitwiseXor",                               saOp_BitwiseXor,                                5, SA_FALSE, { SA_UINT32_MAX } },
  { "BitwiseAnd",                               saOp_BitwiseAnd,                                5, SA_FALSE, { SA_UINT32_MAX } },
  { "Not",                                      saOp_Not,                                       4, SA_FALSE, { SA_UINT32_MAX } },
  { "BitFieldInsert",                           saOp_BitFieldInsert,                            7, SA_FALSE, { SA_UINT32_MAX } },
  { "BitFieldSExtract",                         saOp_BitFieldSExtract,                          6, SA_FALSE, { SA_UINT32_MAX } },
  { "BitFieldUExtract",                         saOp_BitFieldUExtract,                          6, SA_FALSE, { SA_UINT32_MAX } },
  { "BitReverse",                               saOp_BitReverse,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "BitCount",                                 saOp_BitCount,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "DPdx",                                     saOp_DPdx,                                      4, SA_FALSE, { SA_UINT32_MAX } },
  { "DPdy",                                     saOp_DPdy,                                      4, SA_FALSE, { SA_UINT32_MAX } },
  { "Fwidth",                                   saOp_Fwidth,                                    4, SA_FALSE, { SA_UINT32_MAX } },
  { "DPdxFine",                                 saOp_DPdxFine,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "DPdyFine",                                 saOp_DPdyFine,                                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "FwidthFine",                               saOp_FwidthFine,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "DPdxCoarse",                               saOp_DPdxCoarse,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "DPdyCoarse",                               saOp_DPdyCoarse,                                4, SA_FALSE, { SA_UINT32_MAX } },
  { "FwidthCoarse",                             saOp_FwidthCoarse,                              4, SA_FALSE, { SA_UINT32_MAX } },
  { "EmitVertex",                               saOp_EmitVertex,                                1, SA_FALSE, { SA_UINT32_MAX } },
  { "EndPrimitive",                             saOp_EndPrimitive,                              1, SA_FALSE, { SA_UINT32_MAX } },
  { "EmitStreamVertex",                         saOp_EmitStreamVertex,                          2, SA_FALSE, { SA_UINT32_MAX } },
  { "EndStreamPrimitive",                       saOp_EndStreamPrimitive,                        2, SA_FALSE, { SA_UINT32_MAX } },
  { "ControlBarrier",                           saOp_ControlBarrier,                            4, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "MemoryBarrier",                            saOp_MemoryBarrier,                             3, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicLoad",                               saOp_AtomicLoad,                                6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicStore",                              saOp_AtomicStore,                               5, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicExchange",                           saOp_AtomicExchange,                            7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicCompareExchange",                    saOp_AtomicCompareExchange,                     9, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicCompareExchangeWeak",                saOp_AtomicCompareExchangeWeak,                 9, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicIIncrement",                         saOp_AtomicIIncrement,                          6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicIDecrement",                         saOp_AtomicIDecrement,                          6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicIAdd",                               saOp_AtomicIAdd,                                7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicISub",                               saOp_AtomicISub,                                7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicSMin",                               saOp_AtomicSMin,                                7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicUMin",                               saOp_AtomicUMin,                                7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicSMax",                               saOp_AtomicSMax,                                7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicUMax",                               saOp_AtomicUMax,                                7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicAnd",                                saOp_AtomicAnd,                                 7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicOr",                                 saOp_AtomicOr,                                  7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicXor",                                saOp_AtomicXor,                                 7, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "Phi",                                      saOp_Phi,                                       3, SA_TRUE,  { SA_UINT32_MAX } },
  { "LoopMerge",                                saOp_LoopMerge,                                 4, SA_TRUE,  { saAsmEnum_LoopControl, SA_UINT32_MAX } },
  { "SelectionMerge",                           saOp_SelectionMerge,                            3, SA_FALSE, { saAsmEnum_SelectionControl, SA_UINT32_MAX } },
  { "Label",                                    saOp_Label,                                     2, SA_FALSE, { SA_UINT32_MAX } },
  { "Branch",                                   saOp_Branch,                                    2, SA_FALSE, { SA_UINT32_MAX } },
  { "BranchConditional",                        saOp_BranchConditional,                         4, SA_TRUE,  { SA_UINT32_MAX } },
  { "Switch",                                   saOp_Switch,                                    3, SA_TRUE,  { SA_UINT32_MAX } },
  { "Kill",                                     saOp_Kill,                                      1, SA_FALSE, { SA_UINT32_MAX } },
  { "Return",                                   saOp_Return,                                    1, SA_FALSE, { SA_UINT32_MAX } },
  { "ReturnValue",                              saOp_ReturnValue,                               2, SA_FALSE, { SA_UINT32_MAX } },
  { "Unreachable",                              saOp_Unreachable,                               1, SA_FALSE, { SA_UINT32_MAX } },
  { "LifetimeStart",                            saOp_LifetimeStart,                             3, SA_FALSE, { SA_UINT32_MAX } },
  { "LifetimeStop",                             saOp_LifetimeStop,                              3, SA_FALSE, { SA_UINT32_MAX } },
  { "GroupAsyncCopy",                           saOp_GroupAsyncCopy,                            9, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupWaitEvents",                          saOp_GroupWaitEvents,                           4, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupAll",                                 saOp_GroupAll,                                  5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupAny",                                 saOp_GroupAny,                                  5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupBroadcast",                           saOp_GroupBroadcast,                            6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupIAdd",                                saOp_GroupIAdd,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupFAdd",                                saOp_GroupFAdd,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupFMin",                                saOp_GroupFMin,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupUMin",                                saOp_GroupUMin,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupSMin",                                saOp_GroupSMin,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupFMax",                                saOp_GroupFMax,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupUMax",                                saOp_GroupUMax,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupSMax",                                saOp_GroupSMax,                                 6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "ReadPipe",                                 saOp_ReadPipe,                                  7, SA_FALSE, { SA_UINT32_MAX } },
  { "WritePipe",                                saOp_WritePipe,                                 7, SA_FALSE, { SA_UINT32_MAX } },
  { "ReservedReadPipe",                         saOp_ReservedReadPipe,                          9, SA_FALSE, { SA_UINT32_MAX } },
  { "ReservedWritePipe",                        saOp_ReservedWritePipe,                         9, SA_FALSE, { SA_UINT32_MAX } },
  { "ReservedReadPipePackets",                  saOp_ReservedReadPipePackets,                   7, SA_FALSE, { SA_UINT32_MAX } },
  { "ReservedWritePipePackets",                 saOp_ReservedWritePipePackets,                  7, SA_FALSE, { SA_UINT32_MAX } },
  { "CommitReadPipe",                           saOp_CommitReadPipe,                            5, SA_FALSE, { SA_UINT32_MAX } },
  { "CommitWritePipe",                          saOp_CommitWritePipe,                           5, SA_FALSE, { SA_UINT32_MAX } },
  { "IsValidReservedId",                        saOp_IsValidReservedId,                         4, SA_FALSE, { SA_UINT32_MAX } },
  { "GetNumPipePackets",                        saOp_GetNumPipePackets,                         6, SA_FALSE, { SA_UINT32_MAX } },
  { "GetMaxPipePackets",                        saOp_GetMaxPipePackets,                         6, SA_FALSE, { SA_UINT32_MAX } },
  { "GroupReserveReadPipePackets",              saOp_GroupReserveReadPipePackets,               8, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupReserveWritePipePackets",             saOp_GroupReserveWritePipePackets,              8, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupCommitReadPipe",                      saOp_GroupCommitReadPipe,                       6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupCommitWritePipe",                     saOp_GroupCommitWritePipe,                      6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "EnqueueMarker",                            saOp_EnqueueMarker,                             7, SA_FALSE, { SA_UINT32_MAX } },
  { "EnqueueKernel",                            saOp_EnqueueKernel,                             13, SA_TRUE, { SA_UINT32_MAX } },
  { "GetKernelINDrangeSubGroupCount",           saOp_GetKernelINDrangeSubGroupCount,            8, SA_FALSE, { SA_UINT32_MAX } },
  { "GetKernelINDrangeMaxSubGroupSize",         saOp_GetKernelINDrangeMaxSubGroupSize,          8, SA_FALSE, { SA_UINT32_MAX } },
  { "GetKernelIWorkGroupSize",                  saOp_GetKernelIWorkGroupSize,                   7, SA_FALSE, { SA_UINT32_MAX } },
  { "GetKernelIPreferredWorkGroupSizeMultiple", saOp_GetKernelIPreferredWorkGroupSizeMultiple,  7, SA_FALSE, { SA_UINT32_MAX } },
  { "RetainEvent",                              saOp_RetainEvent,                               2, SA_FALSE, { SA_UINT32_MAX } },
  { "ReleaseEvent",                             saOp_ReleaseEvent,                              2, SA_FALSE, { SA_UINT32_MAX } },
  { "CreateUserEvent",                          saOp_CreateUserEvent,                           3, SA_FALSE, { SA_UINT32_MAX } },
  { "IsValidEvent",                             saOp_IsValidEvent,                              4, SA_FALSE, { SA_UINT32_MAX } },
  { "SetUserEventStatus",                       saOp_SetUserEventStatus,                        3, SA_FALSE, { SA_UINT32_MAX } },
  { "CaptureEventProfilingInfo",                saOp_CaptureEventProfilingInfo,                 4, SA_FALSE, { SA_UINT32_MAX } },
  { "GetDefaultQueue",                          saOp_GetDefaultQueue,                           3, SA_FALSE, { SA_UINT32_MAX } },
  { "BuildNDRange",                             saOp_BuildNDRange,                              6, SA_FALSE, { SA_UINT32_MAX } },
  { "ImageSparseSampleImplicitLod",             saOp_ImageSparseSampleImplicitLod,              5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseSampleExplicitLod",             saOp_ImageSparseSampleExplicitLod,              7, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseSampleDrefImplicitLod",         saOp_ImageSparseSampleDrefImplicitLod,          6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseSampleDrefExplicitLod",         saOp_ImageSparseSampleDrefExplicitLod,          8, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseFetch",                         saOp_ImageSparseFetch,                          5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseGather",                        saOp_ImageSparseGather,                         6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseDrefGather",                    saOp_ImageSparseDrefGather,                     6, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "ImageSparseTexelResident",                 saOp_ImageSparseTexelResident,                  4, SA_FALSE, { SA_UINT32_MAX } },
  { "NoLine",                                   saOp_NoLine,                                    1, SA_FALSE, { SA_UINT32_MAX } },
  { "AtomicFlagTestAndSet",                     saOp_AtomicFlagTestAndSet,                      6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "AtomicFlagClear",                          saOp_AtomicFlagClear,                           4, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "ImageSparseRead",                          saOp_ImageSparseRead,                           5, SA_TRUE,  { saAsmEnum_ImageOperand, SA_UINT32_MAX } },
  { "SizeOf",                                   saOp_SizeOf,                                    4, SA_FALSE, { SA_UINT32_MAX } },
  { "TypePipeStorage",                          saOp_TypePipeStorage,                           2, SA_FALSE, { SA_UINT32_MAX } },
  { "ConstantPipeStorage",                      saOp_ConstantPipeStorage,                       6, SA_FALSE, { SA_UINT32_MAX } },
  { "CreatePipeFromPipeStorage",                saOp_CreatePipeFromPipeStorage,                 4, SA_FALSE, { SA_UINT32_MAX } },
  { "GetKernelLocalSizeForSubgroupCount",       saOp_GetKernelLocalSizeForSubgroupCount,        8, SA_FALSE, { SA_UINT32_MAX } },
  { "GetKernelMaxNumSubgroups",                 saOp_GetKernelMaxNumSubgroups,                  7, SA_FALSE, { SA_UINT32_MAX } },
  { "TypeNamedBarrier",                         saOp_TypeNamedBarrier,                          2, SA_FALSE, { SA_UINT32_MAX } },
  { "NamedBarrierInitialize",                   saOp_NamedBarrierInitialize,                    4, SA_FALSE, { SA_UINT32_MAX } },
  { "MemoryNamedBarrier",                       saOp_MemoryNamedBarrier,                        4, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_MemorySemantics, SA_UINT32_MAX } },
  { "ModuleProcessed",                          saOp_ModuleProcessed,                           2, SA_TRUE,  { SA_UINT32_MAX } },
  { "ExecutionModeId",                          saOp_ExecutionModeId,                           5, SA_TRUE,  { SA_UINT32_MAX /*XXX: Possibly execution mode later*/} },
  { "DecorateId",                               saOp_DecorateId,                                3, SA_TRUE,  { saAsmEnum_Decoration, saAsmEnum_DecorationBuiltIn, SA_UINT32_MAX } },
  { "GroupNonUniformElect",                     saOp_GroupNonUniformElect,                      4, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformAll",                       saOp_GroupNonUniformAll,                        5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformAny",                       saOp_GroupNonUniformAny,                        5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformAllEqual",                  saOp_GroupNonUniformAllEqual,                   5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformBroadcast",                 saOp_GroupNonUniformBroadcast,                  6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformBroadcastFirst",            saOp_GroupNonUniformBroadcastFirst,             5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformBallot",                    saOp_GroupNonUniformBallot,                     5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformInverseBallot",             saOp_GroupNonUniformInverseBallot,              5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformBallotBitExtract",          saOp_GroupNonUniformBallotBitExtract,           6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformBallotBitCount",            saOp_GroupNonUniformBallotBitCount,             6, SA_FALSE, { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformBallotFindLSB",             saOp_GroupNonUniformBallotFindLSB,              5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformBallotFindMSB",             saOp_GroupNonUniformBallotFindMSB,              5, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformShuffle",                   saOp_GroupNonUniformShuffle,                    6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformShuffleXor",                saOp_GroupNonUniformShuffleXor,                 6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformShuffleUp",                 saOp_GroupNonUniformShuffleUp,                  6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformShuffleDown",               saOp_GroupNonUniformShuffleDown,                6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformIAdd",                      saOp_GroupNonUniformIAdd,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformFAdd",                      saOp_GroupNonUniformFAdd,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformIMul",                      saOp_GroupNonUniformIMul,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformFMul",                      saOp_GroupNonUniformFMul,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformSMin",                      saOp_GroupNonUniformSMin,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformUMin",                      saOp_GroupNonUniformUMin,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformFMin",                      saOp_GroupNonUniformFMin,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformSMax",                      saOp_GroupNonUniformSMax,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformUMax",                      saOp_GroupNonUniformUMax,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformFMax",                      saOp_GroupNonUniformFMax,                       6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformBitwiseAnd",                saOp_GroupNonUniformBitwiseAnd,                 6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformBitwiseOr",                 saOp_GroupNonUniformBitwiseOr,                  6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformBitwiseXor",                saOp_GroupNonUniformBitwiseXor,                 6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformLogicalAnd",                saOp_GroupNonUniformLogicalAnd,                 6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformLogicalOr",                 saOp_GroupNonUniformLogicalOr,                  6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformLogicalXor",                saOp_GroupNonUniformLogicalXor,                 6, SA_TRUE,  { saAsmEnum_Scope, saAsmEnum_GroupOperation, SA_UINT32_MAX } },
  { "GroupNonUniformQuadBroadcast",             saOp_GroupNonUniformQuadBroadcast,              6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "GroupNonUniformQuadSwap",                  saOp_GroupNonUniformQuadSwap,                   6, SA_FALSE, { saAsmEnum_Scope, SA_UINT32_MAX } },
  { "CopyLogical",                              saOp_CopyLogical,                               4, SA_FALSE, { SA_UINT32_MAX } },
  { "PtrEqual",                                 saOp_PtrEqual,                                  5, SA_FALSE, { SA_UINT32_MAX } },
  { "PtrNotEqual",                              saOp_PtrNotEqual,                               5, SA_FALSE, { SA_UINT32_MAX } },
  { "PtrDiff",                                  saOp_PtrDiff,                                   5, SA_FALSE, { SA_UINT32_MAX } }
};

// Table with all keywords possible in SPA/SBA
const struct sa__assemblerLowLevelEnumerantConnection_s SA_ASSEMBLER_LOW_LEVEL_ENUMS[saAsmEnum_COUNT][SA_MAX_ENUMERANT_ENTRIES] = {
  {
    { "Vertex", saEntryPoint_Vertex },
    { "TessellationControl", saEntryPoint_TessellationControl },
    { "TessellationEvaluation", saEntryPoint_TessellationEvaluation },
    { "Geometry", saEntryPoint_Geometry },
    { "Fragment", saEntryPoint_Fragment },
    { "GLCompute", saEntryPoint_GLCompute },
    { "Kernel", saEntryPoint_Kernel },
    { "TaskNV", saEntryPoint_TaskNV },
    { "MeshNV", saEntryPoint_MeshNV },
    { "RayGenerationKHR", saEntryPoint_RayGenerationKHR },
    { "IntersectionKHR", saEntryPoint_IntersectionKHR },
    { "AnyHitKHR", saEntryPoint_AnyHitKHR },
    { "ClosestHitKHR", saEntryPoint_ClosestHitKHR },
    { "MissKHR", saEntryPoint_MissKHR },
    { "CallableKHR", saEntryPoint_CallableKHR },
    { "TaskEXT", saEntryPoint_TaskEXT },
    { "MeshEXT", saEntryPoint_MeshEXT },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "UniformConstant", saStorageClass_UniformConstant },
    { "Input", saStorageClass_Input },
    { "Uniform", saStorageClass_Uniform },
    { "Output", saStorageClass_Output },
    { "Workgroup", saStorageClass_Workgroup },
    { "CrossWorkgroup", saStorageClass_CrossWorkgroup },
    { "Private", saStorageClass_Private },
    { "Function", saStorageClass_Function },
    { "Generic", saStorageClass_Generic },
    { "PushConstant", saStorageClass_PushConstant },
    { "AtomicCounter", saStorageClass_AtomicCounter },
    { "Image", saStorageClass_Image },
    { "StorageBuffer", saStorageClass_StorageBuffer },
    { "TileImageEXT", saStorageClass_TileImageEXT },
    { "NodePayloadAMDX", saStorageClass_NodePayloadAMDX },
    { "CallableDataKHR", saStorageClass_CallableDataKHR },
    { "IncomingCallableDataKHR", saStorageClass_IncomingCallableDataKHR },
    { "RayPayloadKHR", saStorageClass_RayPayloadKHR },
    { "HitAttributeKHR", saStorageClass_HitAttributeKHR },
    { "IncomingRayPayloadKHR", saStorageClass_IncomingRayPayloadKHR },
    { "ShaderRecordBufferKHR", saStorageClass_ShaderRecordBufferKHR },
    { "PhysicalStorageBuffer", saStorageClass_PhysicalStorageBuffer },
    { "HitObjectAttributeNV", saStorageClass_HitObjectAttributeNV },
    { "TaskPayloadWorkgroupEXT", saStorageClass_TaskPayloadWorkgroupEXT },
    { "CodeSelectionINTEL", saStorageClass_CodeSelectionINTEL },
    { "DeviceOnlyINTEL", saStorageClass_DeviceOnlyINTEL },
    { "HostOnlyINTEL", saStorageClass_HostOnlyINTEL },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "1D", saImageDimmension_1D },
    { "2D", saImageDimmension_2D },
    { "3D", saImageDimmension_3D },
    { "Cube", saImageDimmension_Cube },
    { "Rect", saImageDimmension_Rect },
    { "Buffer", saImageDimmension_Buffer },
    { "SubpassData", saImageDimmension_SubpassData },
    { "TileImageDataEXT", saImageDimmension_TileImageDataEXT },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saSamplerAddressingMode_None },
    { "ClampToEdge", saSamplerAddressingMode_ClampToEdge },
    { "Clamp", saSamplerAddressingMode_Clamp },
    { "Repeat", saSamplerAddressingMode_Repeat },
    { "RepeatMirrored", saSamplerAddressingMode_RepeatMirrored },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "Nearest", saSamplerFilterMode_Nearest },
    { "Linear", saSamplerFilterMode_Linear },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "Unknown", saImageFormat_Unknown },
    { "Rgba32f", saImageFormat_Rgba32f },
    { "Rgba16f", saImageFormat_Rgba16f },
    { "R32f", saImageFormat_R32f },
    { "Rgba8", saImageFormat_Rgba8 },
    { "Rgba8Snorm", saImageFormat_Rgba8Snorm },
    { "Rg32f", saImageFormat_Rg32f },
    { "Rg16f", saImageFormat_Rg16f },
    { "R11fG11fB10f", saImageFormat_R11fG11fB10f },
    { "R16f", saImageFormat_R16f },
    { "Rgba16", saImageFormat_Rgba16 },
    { "Rgb10A2", saImageFormat_Rgb10A2 },
    { "Rg16", saImageFormat_Rg16 },
    { "Rg8", saImageFormat_Rg8 },
    { "R16", saImageFormat_R16 },
    { "R8", saImageFormat_R8 },
    { "Rgba16Snorm", saImageFormat_Rgba16Snorm },
    { "Rg16Snorm", saImageFormat_Rg16Snorm },
    { "Rg8Snorm", saImageFormat_Rg8Snorm },
    { "R16Snorm", saImageFormat_R16Snorm },
    { "R8Snorm", saImageFormat_R8Snorm },
    { "Rgba32i", saImageFormat_Rgba32i },
    { "Rgba16i", saImageFormat_Rgba16i },
    { "Rgba8i", saImageFormat_Rgba8i },
    { "R32i", saImageFormat_R32i },
    { "Rg32i", saImageFormat_Rg32i },
    { "Rg16i", saImageFormat_Rg16i },
    { "Rg8i", saImageFormat_Rg8i },
    { "R16i", saImageFormat_R16i },
    { "R8i", saImageFormat_R8i },
    { "Rgba32ui", saImageFormat_Rgba32ui },
    { "Rgba16ui", saImageFormat_Rgba16ui },
    { "Rgba8ui", saImageFormat_Rgba8ui },
    { "R32ui", saImageFormat_R32ui },
    { "Rgb10a2ui", saImageFormat_Rgb10a2ui },
    { "Rg32ui", saImageFormat_Rg32ui },
    { "Rg16ui", saImageFormat_Rg16ui },
    { "Rg8ui", saImageFormat_Rg8ui },
    { "R16ui", saImageFormat_R16ui },
    { "R8ui", saImageFormat_R8ui },
    { "R64ui", saImageFormat_R64ui },
    { "R64i", saImageFormat_R64i },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "R", saImageChannelOrder_R },
    { "A", saImageChannelOrder_A },
    { "RG", saImageChannelOrder_RG },
    { "RA", saImageChannelOrder_RA },
    { "RGB", saImageChannelOrder_RGB },
    { "RGBA", saImageChannelOrder_RGBA },
    { "BGRA", saImageChannelOrder_BGRA },
    { "ARGB", saImageChannelOrder_ARGB },
    { "Intensity", saImageChannelOrder_Intensity },
    { "Luminance", saImageChannelOrder_Luminance },
    { "Rx", saImageChannelOrder_Rx },
    { "RGx", saImageChannelOrder_RGx },
    { "RGBx", saImageChannelOrder_RGBx },
    { "Depth", saImageChannelOrder_Depth },
    { "DepthStencil", saImageChannelOrder_DepthStencil },
    { "sRGB", saImageChannelOrder_sRGB },
    { "sRGBx", saImageChannelOrder_sRGBx },
    { "sRGBA", saImageChannelOrder_sRGBA },
    { "sBGRA", saImageChannelOrder_sBGRA },
    { "ABGR", saImageChannelOrder_ABGR },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "SnormInt8", saImageChannelDataFormat_SnormInt8 },
    { "SnormInt16", saImageChannelDataFormat_SnormInt16 },
    { "UnormInt8", saImageChannelDataFormat_UnormInt8 },
    { "UnormInt16", saImageChannelDataFormat_UnormInt16 },
    { "UnormShort565", saImageChannelDataFormat_UnormShort565 },
    { "UnormShort555", saImageChannelDataFormat_UnormShort555 },
    { "UnormInt101010", saImageChannelDataFormat_UnormInt101010 },
    { "SignedInt8", saImageChannelDataFormat_SignedInt8 },
    { "SignedInt16", saImageChannelDataFormat_SignedInt16 },
    { "SignedInt32", saImageChannelDataFormat_SignedInt32 },
    { "UnsignedInt8", saImageChannelDataFormat_UnsignedInt8 },
    { "UnsignedInt16", saImageChannelDataFormat_UnsignedInt16 },
    { "UnsignedInt32", saImageChannelDataFormat_UnsignedInt32 },
    { "HalfFloat", saImageChannelDataFormat_HalfFloat },
    { "Float", saImageChannelDataFormat_Float },
    { "UnormInt24", saImageChannelDataFormat_UnormInt24 },
    { "UnormInt101010_2", saImageChannelDataFormat_UnormInt101010_2 },
    { "UnsignedIntRaw10EXT", saImageChannelDataFormat_UnsignedIntRaw10EXT },
    { "UnsignedIntRaw12EXT", saImageChannelDataFormat_UnsignedIntRaw12EXT },
    { "UnormInt2_101010EXT", saImageChannelDataFormat_UnormInt2_101010EXT },
    { "UnsignedInt10X6EXT", saImageChannelDataFormat_UnsignedInt10X6EXT },
    { "UnsignedInt12X4EXT", saImageChannelDataFormat_UnsignedInt12X4EXT },
    { "UnsignedInt14X2EXT", saImageChannelDataFormat_UnsignedInt14X2EXT },
    { "UnormInt12X4EXT", saImageChannelDataFormat_UnormInt12X4EXT },
    { "UnormInt14X2EXT", saImageChannelDataFormat_UnormInt14X2EXT },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saImageOperands_None },
    { "Bias", saImageOperands_Bias },
    { "Lod", saImageOperands_Lod },
    { "Grad", saImageOperands_Grad },
    { "ConstOffset", saImageOperands_ConstOffset },
    { "Offset", saImageOperands_Offset },
    { "ConstOffsets", saImageOperands_ConstOffsets },
    { "Sample", saImageOperands_Sample },
    { "MinLod", saImageOperands_MinLod },
    { "MakeTexelAvailable", saImageOperands_MakeTexelAvailable },
    { "MakeTexelVisible", saImageOperands_MakeTexelVisible },
    { "NonPrivateTexel", saImageOperands_NonPrivateTexel },
    { "VolatileTexel", saImageOperands_VolatileTexel },
    { "SignExtent", saImageOperands_SignExtent },
    { "ZeroExtent", saImageOperands_ZeroExtent },
    { "Nontemporal", saImageOperands_Nontemporal },
    { "Offsets", saImageOperands_Offsets },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saFPFastMathMode_None },
    { "NotNan", saFPFastMathMode_NotNan },
    { "NotInf", saFPFastMathMode_NotInf },
    { "NSZ", saFPFastMathMode_NSZ },
    { "AllowReciprocal", saFPFastMathMode_AllowReciprocal },
    { "Fast", saFPFastMathMode_Fast },
    { "AllowContract", saFPFastMathMode_AllowContract },
    { "AllowReassoc", saFPFastMathMode_AllowReassoc },
    { "AllowTransform", saFPFastMathMode_AllowTransform },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "ToNearestEven", saFPRoundingMode_ToNearestEven },
    { "ToZero", saFPRoundingMode_ToZero },
    { "ToPositiveInfinity", saFPRoundingMode_ToPositiveInfinity },
    { "ToNegativeInfinity", saFPRoundingMode_ToNegativeInfinity },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "Export", saLinkageType_Export },
    { "Import", saLinkageType_Import },
    { "LinkOnceODR", saLinkageType_LinkOnceODR },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "ReadOnly", saAccessQualifiers_ReadOnly },
    { "WriteOnly", saAccessQualifiers_WriteOnly },
    { "ReadWrite", saAccessQualifiers_ReadWrite },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "ZeroExtend", saFunctionParameterAttrib_ZeroExtend },
    { "SignExtend", saFunctionParameterAttrib_SignExtend },
    { "ByValue", saFunctionParameterAttrib_ByValue },
    { "StructReturn", saFunctionParameterAttrib_StructReturn },
    { "NoAlias", saFunctionParameterAttrib_NoAlias },
    { "NoCaptute", saFunctionParameterAttrib_NoCaptute },
    { "NoWrite", saFunctionParameterAttrib_NoWrite },
    { "NoReadWrite", saFunctionParameterAttrib_NoReadWrite },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "RelaxedPrecision", saDecoration_RelaxedPrecision },
    { "SpecId", saDecoration_SpecId },
    { "Block", saDecoration_Block },
    { "BufferBlock", saDecoration_BufferBlock },
    { "RowMajor", saDecoration_RowMajor },
    { "ColMajor", saDecoration_ColMajor },
    { "ArrayStride", saDecoration_ArrayStride },
    { "MatrixStride", saDecoration_MatrixStride },
    { "GLSLShared", saDecoration_GLSLShared },
    { "GLSLPacked", saDecoration_GLSLPacked },
    { "CPacked", saDecoration_CPacked },
    { "BuiltIn", saDecoration_BuiltIn },
    { "NoPerspective", saDecoration_NoPerspective },
    { "Flat", saDecoration_Flat },
    { "Patch", saDecoration_Patch },
    { "Centroid", saDecoration_Centroid },
    { "Sample", saDecoration_Sample },
    { "Invariant", saDecoration_Invariant },
    { "Restrict", saDecoration_Restrict },
    { "Aliased", saDecoration_Aliased },
    { "Volatile", saDecoration_Volatile },
    { "Constant", saDecoration_Constant },
    { "Coherent", saDecoration_Coherent },
    { "NonWritable", saDecoration_NonWritable },
    { "NonReadable", saDecoration_NonReadable },
    { "Uniform", saDecoration_Uniform },
    { "UniformId", saDecoration_UniformId },
    { "SaturatedConversion", saDecoration_SaturatedConversion },
    { "Stream", saDecoration_Stream },
    { "Location", saDecoration_Location },
    { "Component", saDecoration_Component },
    { "Index", saDecoration_Index },
    { "Binding", saDecoration_Binding },
    { "DescriptorSet", saDecoration_DescriptorSet },
    { "Offset", saDecoration_Offset },
    { "XfbBuffer", saDecoration_XfbBuffer },
    { "XfbStride", saDecoration_XfbStride },
    { "FuncParamAttrib", saDecoration_FuncParamAttrib },
    { "FPRoundingMode", saDecoration_FPRoundingMode },
    { "FPFastMathMode", saDecoration_FPFastMathMode },
    { "LinkageAttribs", saDecoration_LinkageAttribs },
    { "NoContraction", saDecoration_NoContraction },
    { "InputAttachmentIndex", saDecoration_InputAttachmentIndex },
    { "Alignment", saDecoration_Alignment },
    { "MaxByteOffset", saDecoration_MaxByteOffset },
    { "AlignmentId", saDecoration_AlignmentId },
    { "MaxByteOffsetId", saDecoration_MaxByteOffsetId },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "Position", saDecorationBuiltIn_Position },
    { "PointSize", saDecorationBuiltIn_PointSize },
    { "ClipDistance", saDecorationBuiltIn_ClipDistance },
    { "CullDistance", saDecorationBuiltIn_CullDistance },
    { "VertexId", saDecorationBuiltIn_VertexId },
    { "InstanceId", saDecorationBuiltIn_InstanceId },
    { "PrimitiveId", saDecorationBuiltIn_PrimitiveId },
    { "InvocationId", saDecorationBuiltIn_InvocationId },
    { "Layer", saDecorationBuiltIn_Layer },
    { "ViewportIndex", saDecorationBuiltIn_ViewportIndex },
    { "TessLevelOuter", saDecorationBuiltIn_TessLevelOuter },
    { "TessLevelInner", saDecorationBuiltIn_TessLevelInner },
    { "TessCoord", saDecorationBuiltIn_TessCoord },
    { "PatchVertices", saDecorationBuiltIn_PatchVertices },
    { "FragCoord", saDecorationBuiltIn_FragCoord },
    { "PointCoord", saDecorationBuiltIn_PointCoord },
    { "FrontFacing", saDecorationBuiltIn_FrontFacing },
    { "SampleId", saDecorationBuiltIn_SampleId },
    { "SamplePosition", saDecorationBuiltIn_SamplePosition },
    { "SampleMask", saDecorationBuiltIn_SampleMask },
    { "FragDepth", saDecorationBuiltIn_FragDepth },
    { "HelperInvocation", saDecorationBuiltIn_HelperInvocation },
    { "NumWorkgroups", saDecorationBuiltIn_NumWorkgroups },
    { "WorkgroupSize", saDecorationBuiltIn_WorkgroupSize },
    { "WorkgroupId", saDecorationBuiltIn_WorkgroupId },
    { "LocalInvocationId", saDecorationBuiltIn_LocalInvocationId },
    { "GlobalInvocationId", saDecorationBuiltIn_GlobalInvocationId },
    { "LocalInvocationIndex", saDecorationBuiltIn_LocalInvocationIndex },
    { "WorkDim", saDecorationBuiltIn_WorkDim },
    { "GlobalSize", saDecorationBuiltIn_GlobalSize },
    { "EnqueuedWorkgroupSize", saDecorationBuiltIn_EnqueuedWorkgroupSize },
    { "GlobalOffset", saDecorationBuiltIn_GlobalOffset },
    { "GlobalLinearId", saDecorationBuiltIn_GlobalLinearId },
    { "SubgroupSize", saDecorationBuiltIn_SubgroupSize },
    { "SubgroupMaxSize", saDecorationBuiltIn_SubgroupMaxSize },
    { "NumSubgroups", saDecorationBuiltIn_NumSubgroups },
    { "NumEnqueuedSubgroups", saDecorationBuiltIn_NumEnqueuedSubgroups },
    { "SubgroupId", saDecorationBuiltIn_SubgroupId },
    { "SubgroupLocalInvocationId", saDecorationBuiltIn_SubgroupLocalInvocationId },
    { "VertexIndex", saDecorationBuiltIn_VertexIndex },
    { "InstanceIndex", saDecorationBuiltIn_InstanceIndex },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saSelectionControl_None },
    { "Flatten", saSelectionControl_Flatten },
    { "DontFlatten", saSelectionControl_DontFlatten },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saLoopControl_None },
    { "Unroll", saLoopControl_Unroll },
    { "DontUnroll", saLoopControl_DontUnroll },
    { "DependencyInfinite", saLoopControl_DependencyInfinite },
    { "DependencyLength", saLoopControl_DependencyLength },
    { "MinIterations", saLoopControl_MinIterations },
    { "MaxIterations", saLoopControl_MaxIterations },
    { "IterationMultiple", saLoopControl_IterationMultiple },
    { "PeelCount", saLoopControl_PeelCount },
    { "PartialCount", saLoopControl_PartialCount },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saFunctionControl_None },
    { "Inline", saFunctionControl_Inline },
    { "DontInline", saFunctionControl_DontInline },
    { "Pure", saFunctionControl_Pure },
    { "Const", saFunctionControl_Const },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saMemorySemantics_None },
    { "Acquire", saMemorySemantics_Acquire },
    { "Release", saMemorySemantics_Release },
    { "AcquireRelease", saMemorySemantics_AcquireRelease },
    { "SequentiallyConsistent", saMemorySemantics_SequentiallyConsistent },
    { "UniformMemory", saMemorySemantics_UniformMemory },
    { "SubgroupMemory", saMemorySemantics_SubgroupMemory },
    { "WorkgroupMemory", saMemorySemantics_WorkgroupMemory },
    { "CrossWorkgroupMemory", saMemorySemantics_CrossWorkgroupMemory },
    { "AtomicCounterMemory", saMemorySemantics_AtomicCounterMemory },
    { "ImageMemory", saMemorySemantics_ImageMemory },
    { "OutputMemory", saMemorySemantics_OutputMemory },
    { "MakeAvailable", saMemorySemantics_MakeAvailable },
    { "MakeVisible", saMemorySemantics_MakeVisible },
    { "Volatile", saMemorySemantics_Volatile },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "None", saMemoryOperands_None },
    { "Volatile", saMemoryOperands_Volatile },
    { "Aligned", saMemoryOperands_Aligned },
    { "Nontemporal", saMemoryOperands_Nontemporal },
    { "MakePointerAvailable", saMemoryOperands_MakePointerAvailable },
    { "MakePointerVisible", saMemoryOperands_MakePointerVisible },
    { "NonPrivatePointer", saMemoryOperands_NonPrivatePointer },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "CrossDevice", saScope_CrossDevice },
    { "Device", saScope_Device },
    { "Workgroup", saScope_Workgroup },
    { "Subgroup", saScope_Subgroup },
    { "Invocation", saScope_Invocation },
    { "QueueFamily", saScope_QueueFamily },
    { "ShaderCallKHR", saScope_ShaderCallKHR },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "Reduce", saGroupOperation_Reduce },
    { "InclusiveScan", saGroupOperation_InclusiveScan },
    { "ExclusiveScan", saGroupOperation_ExclusiveScan },
    { "ClusterReduce", saGroupOperation_ClusterReduce },
    { SA_NULL, SA_UINT32_MAX }
  },
  {
    { "Invocations", saExecutionMode_Invocations },
    { "SpacingEqual", saExecutionMode_SpacingEqual },
    { "SpacingFractionalEven", saExecutionMode_SpacingFractionalEven },
    { "SpacingFractionalOdd", saExecutionMode_SpacingFractionalOdd },
    { "VertexOrderCw", saExecutionMode_VertexOrderCw },
    { "VertexOrderCcw", saExecutionMode_VertexOrderCcw },
    { "PixelCenterInteger", saExecutionMode_PixelCenterInteger },
    { "OriginUpperLeft", saExecutionMode_OriginUpperLeft },
    { "OriginLowerLeft", saExecutionMode_OriginLowerLeft },
    { "EarlyFragmentTests", saExecutionMode_EarlyFragmentTests },
    { "PointMode", saExecutionMode_PointMode },
    { "Xfb", saExecutionMode_Xfb },
    { "DepthReplacing", saExecutionMode_DepthReplacing },
    { "DepthGreater", saExecutionMode_DepthGreater },
    { "DepthLess", saExecutionMode_DepthLess },
    { "DepthUnchanged", saExecutionMode_DepthUnchanged },
    { "LocalSize", saExecutionMode_LocalSize },
    { "LocalSizeHint", saExecutionMode_LocalSizeHint },
    { "InputPoints", saExecutionMode_InputPoints },
    { "InputLines", saExecutionMode_InputLines },
    { "InputLinesAdjacency", saExecutionMode_InputLinesAdjacency },
    { "InputTriangles", saExecutionMode_InputTriangles },
    { "InputTrianglesAdjacency", saExecutionMode_InputTrianglesAdjacency },
    { "Quads", saExecutionMode_Quads },
    { "Isolines", saExecutionMode_Isolines },
    { "OutputVertices", saExecutionMode_OutputVertices },
    { "OutputPoints", saExecutionMode_OutputPoints },
    { "OutputLineStrip", saExecutionMode_OutputLineStrip },
    { "OutputTriangleStrip", saExecutionMode_OutputTriangleStrip },
    { "VecTypeHint", saExecutionMode_VecTypeHint },
    { "ContractionOff", saExecutionMode_ContractionOff },
    { "Initializer", saExecutionMode_Initializer },
    { "Finalizer", saExecutionMode_Finalizer },
    { "SubgroupSize", saExecutionMode_SubgroupSize },
    { "SubgroupsPerWorkgroup", saExecutionMode_SubgroupsPerWorkgroup },
    { "SubgroupsPerWorkgroupId", saExecutionMode_SubgroupsPerWorkgroupId },
    { "LocalSizeId", saExecutionMode_LocalSizeId },
    { "LocalSizeHintId", saExecutionMode_LocalSizeHintId },
    { "NonCoherentColorAttachmentReadEXT", saExecutionMode_NonCoherentColorAttachmentReadEXT },
    { "NonCoherentDepthAttachmentReadEXT", saExecutionMode_NonCoherentDepthAttachmentReadEXT },
    { "NonCoherentStencilAttachmentReadEXT", saExecutionMode_NonCoherentStencilAttachmentReadEXT },
    { "SubgroupUniformControlFlowKHR", saExecutionMode_SubgroupUniformControlFlowKHR },
    { "PostDepthCoverage", saExecutionMode_PostDepthCoverage },
    { "DenormPreserve", saExecutionMode_DenormPreserve },
    { "DenormFlushToZero", saExecutionMode_DenormFlushToZero },
    { "SignedZeroInfNanPreserve", saExecutionMode_SignedZeroInfNanPreserve },
    { "RoundingModeRTE", saExecutionMode_RoundingModeRTE },
    { "RoundingModeRTZ", saExecutionMode_RoundingModeRTZ },
    { "EarlyAndLateFragmentTestsAMD", saExecutionMode_EarlyAndLateFragmentTestsAMD },
    { "StencilRefReplacingEXT", saExecutionMode_StencilRefReplacingEXT },
    { "CoalesaingAMDX", saExecutionMode_CoalesaingAMDX },
    { "IsApiEntryAMDX", saExecutionMode_IsApiEntryAMDX },
    { "MaxNodeRecursionAMDX", saExecutionMode_MaxNodeRecursionAMDX },
    { "StaticNumWorkgroupsAMDX", saExecutionMode_StaticNumWorkgroupsAMDX },
    { "ShaderIndexAMDX", saExecutionMode_ShaderIndexAMDX },
    { "MaxNumWorkgroupsAMDX", saExecutionMode_MaxNumWorkgroupsAMDX },
    { "StencilRefUnchangedFrontAMD", saExecutionMode_StencilRefUnchangedFrontAMD },
    { "StencilRefGreaterFrontAMD", saExecutionMode_StencilRefGreaterFrontAMD },
    { "StencilRefLessFrontAMD", saExecutionMode_StencilRefLessFrontAMD },
    { "StencilRefUnchangedBackAMD", saExecutionMode_StencilRefUnchangedBackAMD },
    { "StencilRefGreaterBackAMD", saExecutionMode_StencilRefGreaterBackAMD },
    { "StencilRefLessBackAMD", saExecutionMode_StencilRefLessBackAMD },
    { "QuadDerivativesKHR", saExecutionMode_QuadDerivativesKHR },
    { "RequireFullQuadsKHR", saExecutionMode_RequireFullQuadsKHR },
    { "ShareInputWithAMDX", saExecutionMode_ShareInputWithAMDX },
    { "OutputLinesEXT", saExecutionMode_OutputLinesEXT },
    { "OutputPrimitivesEXT", saExecutionMode_OutputPrimitivesEXT },
    { "DerivativeGroupQuadsKHR", saExecutionMode_DerivativeGroupQuadsKHR },
    { "DerivativeGroupLinearKHR", saExecutionMode_DerivativeGroupLinearKHR },
    { "OutputTrianglesEXT", saExecutionMode_OutputTrianglesEXT },
    { "PixelInterlockOrderedEXT", saExecutionMode_PixelInterlockOrderedEXT },
    { "PixelInterlockUnorderedEXT", saExecutionMode_PixelInterlockUnorderedEXT },
    { "SamplerInterlockOrderedEXT", saExecutionMode_SamplerInterlockOrderedEXT },
    { "SamplerInterlockUnorderedEXT", saExecutionMode_SamplerInterlockUnorderedEXT },
    { "ShadingRateInterlockOrderedEXT", saExecutionMode_ShadingRateInterlockOrderedEXT },
    { "ShadingRateInterlockUnorderedEXT", saExecutionMode_ShadingRateInterlockUnorderedEXT },
    { "SharedLocalMemorySizeINTEL", saExecutionMode_SharedLocalMemorySizeINTEL },
    { "RoundingModeRTPINTEL", saExecutionMode_RoundingModeRTPINTEL },
    { "RoundingModeRTNINTEL", saExecutionMode_RoundingModeRTNINTEL },
    { "FloatingPointModeALTINTEL", saExecutionMode_FloatingPointModeALTINTEL },
    { "FloatingPointModeIEEEINTEL", saExecutionMode_FloatingPointModeIEEEINTEL },
    { "MaxWorkgroupSizeINTEL", saExecutionMode_MaxWorkgroupSizeINTEL },
    { "MaxWorkDimIMTEL", saExecutionMode_MaxWorkDimIMTEL },
    { "NoGlobalOffsetINTEL", saExecutionMode_NoGlobalOffsetINTEL },
    { "NumSIMDWorkitemsINTEL", saExecutionMode_NumSIMDWorkitemsINTEL },
    { "schedulerTargetFmaxMhzINTEL", saExecutionMode_schedulerTargetFmaxMhzINTEL },
    { "MaximallyReconvergesKHR", saExecutionMode_MaximallyReconvergesKHR },
    { "FPFastMathDefault", saExecutionMode_FPFastMathDefault },
    { "StreamingInterfaceINTEL", saExecutionMode_StreamingInterfaceINTEL },
    { "RegisterMapInterfaceINTEL", saExecutionMode_RegisterMapInterfaceINTEL },
    { "NamedBarrierCountINTEL", saExecutionMode_NamedBarrierCountINTEL },
    { "MaximumRegistersINTEL", saExecutionMode_MaximumRegistersINTEL },
    { "MaximumRegistersIdINTEL", saExecutionMode_MaximumRegistersIdINTEL },
    { "NamedMaximumRegistersINTEL", saExecutionMode_NamedMaximumRegistersINTEL },
    { SA_NULL, SA_UINT32_MAX }
  }
};

// This is the big blob of every basic opcode, could be done better with some lookup table
static const char* sa__opcodeToString(sa_uint16_t opcode) {
  switch(opcode) {
    case saOp_Nop: return "Nop"; break;
    case saOp_Undef: return "Undef"; break;
    case saOp_SourceContinued: return "SourceContinued"; break;
    case saOp_Source: return "Source"; break;
    case saOp_SourceExtension: return "SourceExtension"; break;
    case saOp_Name: return "Name"; break;
    case saOp_MemberName: return "MemberName"; break;
    case saOp_String: return "String"; break;
    case saOp_Line: return "Line"; break;
    case saOp_Extension: return "Extension"; break;
    case saOp_ExtInstImport: return "ExtInstImport"; break;
    case saOp_ExtInst: return "ExtInst"; break;
    case saOp_MemoryModel: return "MemoryModel"; break;
    case saOp_EntryPoint: return "EntryPoint"; break;
    case saOp_ExecutionMode: return "ExecutionMode"; break;
    case saOp_Capability: return "Capability"; break;
    case saOp_TypeVoid: return "TypeVoid"; break;
    case saOp_TypeBool: return "TypeBool"; break;
    case saOp_TypeInt: return "TypeInt"; break;
    case saOp_TypeFloat: return "TypeFloat"; break;
    case saOp_TypeVector: return "TypeVector"; break;
    case saOp_TypeMatrix: return "TypeMatrix"; break;
    case saOp_TypeImage: return "TypeImage"; break;
    case saOp_TypeSampler: return "TypeSampler"; break;
    case saOp_TypeSampledImage: return "TypeSampledImage"; break;
    case saOp_TypeArray: return "TypeArray"; break;
    case saOp_TypeRuntimeArray: return "TypeRuntimeArray"; break;
    case saOp_TypeStruct: return "TypeStruct"; break;
    case saOp_TypeOpaque: return "TypeOpaque"; break;
    case saOp_TypePointer: return "TypePointer"; break;
    case saOp_TypeFunction: return "TypeFunction"; break;
    case saOp_TypeEvent: return "TypeEvent"; break;
    case saOp_TypeDeviceEvent: return "TypeDeviceEvent"; break;
    case saOp_TypeReserveId: return "TypeReserveId"; break;
    case saOp_TypeQueue: return "TypeQueue"; break;
    case saOp_TypePipe: return "TypePipe"; break;
    case saOp_TypeForwardPointer: return "TypeForwardPointer"; break;
    case saOp_ConstantTrue: return "ConstantTrue"; break;
    case saOp_ConstantFalse: return "ConstantFalse"; break;
    case saOp_Constant: return "Constant"; break;
    case saOp_ConstantComposite: return "ConstantComposite"; break;
    case saOp_ConstantSampler: return "ConstantSampler"; break;
    case saOp_ConstantNull: return "ConstantNull"; break;
    case saOp_SpecConstantTrue: return "SpecConstantTrue"; break;
    case saOp_SpecConstantFalse: return "SpecConstantFalse"; break;
    case saOp_SpecConstant: return "SpecConstant"; break;
    case saOp_SpecConstantComposite: return "SpecConstantComposite"; break;
    case saOp_SpecConstantOp: return "SpecConstantOp"; break;
    case saOp_Function: return "Function"; break;
    case saOp_FunctionParameter: return "FunctionParameter"; break;
    case saOp_FunctionEnd: return "FunctionEnd"; break;
    case saOp_FunctionCall: return "FunctionCall"; break;
    case saOp_Variable: return "Variable"; break;
    case saOp_ImageTexelPointer: return "ImageTexelPointer"; break;
    case saOp_Load: return "Load"; break;
    case saOp_Store: return "Store"; break;
    case saOp_CopyMemory: return "CopyMemory"; break;
    case saOp_CopyMemorySized: return "CopyMemorySized"; break;
    case saOp_AccessChain: return "AccessChain"; break;
    case saOp_InBoundsAccessChain: return "InBoundsAccessChain"; break;
    case saOp_PtrAccessChain: return "PtrAccessChain"; break;
    case saOp_ArrayLength: return "ArrayLength"; break;
    case saOp_GenericPtrMemSemantics: return "GenericPtrMemSemantics"; break;
    case saOp_InBoundsPtrAccessChain: return "InBoundsPtrAccessChain"; break;
    case saOp_Decorate: return "Decorate"; break;
    case saOp_MemberDecorate: return "MemberDecorate"; break;
    case saOp_DecorationGroup: return "DecorationGroup"; break;
    case saOp_GroupDecorate: return "GroupDecorate"; break;
    case saOp_GroupMemberDecorate: return "GroupMemberDecorate"; break;
    case saOp_VectorExtractDynamic: return "VectorExtractDynamic"; break;
    case saOp_VectorInsertDynamic: return "VectorInsertDynamic"; break;
    case saOp_VectorShuffle: return "VectorShuffle"; break;
    case saOp_CompositeConstruct: return "CompositeConstruct"; break;
    case saOp_CompositeExtract: return "CompositeExtract"; break;
    case saOp_CompositeInsert: return "CompositeInsert"; break;
    case saOp_CopyObject: return "CopyObject"; break;
    case saOp_Transpose: return "Transpose"; break;
    case saOp_SampledImage: return "SampledImage"; break;
    case saOp_ImageSampleImplicitLod: return "ImageSampleImplicitLod"; break;
    case saOp_ImageSampleExplicitLod: return "ImageSampleExplicitLod"; break;
    case saOp_ImageSampleDrefImplicitLod: return "ImageSampleDrefImplicitLod"; break;
    case saOp_ImageSampleDrefExplicitLod: return "ImageSampleDrefExplicitLod"; break;
    case saOp_ImageSampleProjImplicitLod: return "ImageSampleProjImplicitLod"; break;
    case saOp_ImageSampleProjExplicitLod: return "ImageSampleProjExplicitLod"; break;
    case saOp_ImageSampleProjDrefImplicitLod: return "ImageSampleProjDrefImplicitLod"; break;
    case saOp_ImageSampleProjDrefExplicitLod: return "ImageSampleProjDrefExplicitLod"; break;
    case saOp_ImageFetch: return "ImageFetch"; break;
    case saOp_ImageGather: return "ImageGather"; break;
    case saOp_ImageDrefGather: return "ImageDrefGather"; break;
    case saOp_ImageRead: return "ImageRead"; break;
    case saOp_ImageWrite: return "ImageWrite"; break;
    case saOp_Image: return "Image"; break;
    case saOp_ImageQueryFormat: return "ImageQueryFormat"; break;
    case saOp_ImageQueryOrder: return "ImageQueryOrder"; break;
    case saOp_ImageQuerySizeLod: return "ImageQuerySizeLod"; break;
    case saOp_ImageQuerySize: return "ImageQuerySize"; break;
    case saOp_ImageQueryLod: return "ImageQueryLod"; break;
    case saOp_ImageQueryLevels: return "ImageQueryLevels"; break;
    case saOp_ImageQuerySamples: return "ImageQuerySamples"; break;
    case saOp_ConvertFToU: return "ConvertFToU"; break;
    case saOp_ConvertFToS: return "ConvertFToS"; break;
    case saOp_ConvertSToF: return "ConvertSToF"; break;
    case saOp_ConvertUToF: return "ConvertUToF"; break;
    case saOp_UConvert: return "UConvert"; break;
    case saOp_SConvert: return "SConvert"; break;
    case saOp_FConvert: return "FConvert"; break;
    case saOp_QuantizeToF16: return "QuantizeToF16"; break;
    case saOp_ConvertPtrToU: return "ConvertPtrToU"; break;
    case saOp_SatConvertSToU: return "SatConvertSToU"; break;
    case saOp_SatConvertUToS: return "SatConvertUToS"; break;
    case saOp_ConvertUToPtr: return "ConvertUToPtr"; break;
    case saOp_PtrCastToGeneric: return "PtrCastToGeneric"; break;
    case saOp_GenericCastToPtr: return "GenericCastToPtr"; break;
    case saOp_GenericCastToPtrExplicit: return "GenericCastToPtrExplicit"; break;
    case saOp_Bitcast: return "Bitcast"; break;
    case saOp_SNegate: return "SNegate"; break;
    case saOp_FNegate: return "FNegate"; break;
    case saOp_IAdd: return "IAdd"; break;
    case saOp_FAdd: return "FAdd"; break;
    case saOp_ISub: return "ISub"; break;
    case saOp_FSub: return "FSub"; break;
    case saOp_IMul: return "IMul"; break;
    case saOp_FMul: return "FMul"; break;
    case saOp_UDiv: return "UDiv"; break;
    case saOp_SDiv: return "SDiv"; break;
    case saOp_FDiv: return "FDiv"; break;
    case saOp_UMul: return "UMul"; break;
    case saOp_SRem: return "SRem"; break;
    case saOp_SMod: return "SMod"; break;
    case saOp_FRem: return "FRem"; break;
    case saOp_FMod: return "FMod"; break;
    case saOp_VectorTimesScalar: return "VectorTimesScalar"; break;
    case saOp_MatrixTimesScalar: return "MatrixTimesScalar"; break;
    case saOp_VectorTimesMatrix: return "VectorTimesMatrix"; break;
    case saOp_MatrixTimesVector: return "MatrixTimesVector"; break;
    case saOp_MatrixTimesMatrix: return "MatrixTimesMatrix"; break;
    case saOp_OuterProduct: return "OuterProduct"; break;
    case saOp_Dot: return "Dot"; break;
    case saOp_IAddCarry: return "IAddCarry"; break;
    case saOp_ISubBorrow: return "ISubBorrow"; break;
    case saOp_UMulExtended: return "UMulExtended"; break;
    case saOp_SMulExtended: return "SMulExtended"; break;
    case saOp_Any: return "Any"; break;
    case saOp_All: return "All"; break;
    case saOp_IsNan: return "IsNan"; break;
    case saOp_IsInf: return "IsInf"; break;
    case saOp_IsFinite: return "IsFinite"; break;
    case saOp_IsNormal: return "IsNormal"; break;
    case saOp_SignBitSet: return "SignBitSet"; break;
    case saOp_LessOrGreater: return "LessOrGreater"; break;
    case saOp_Ordered: return "Ordered"; break;
    case saOp_Unordered: return "Unordered"; break;
    case saOp_LogicalEqual: return "LogicalEqual"; break;
    case saOp_LogicalNotEqual: return "LogicalNotEqual"; break;
    case saOp_LogicalOr: return "LogicalOr"; break;
    case saOp_LogicalAnd: return "LogicalAnd"; break;
    case saOp_LogicalNot: return "LogicalNot"; break;
    case saOp_Select: return "Select"; break;
    case saOp_IEqual: return "IEqual"; break;
    case saOp_INotEqual: return "INotEqual"; break;
    case saOp_UGreaterThan: return "UGreaterThan"; break;
    case saOp_SGreaterThan: return "SGreaterThan"; break;
    case saOp_UGreaterThanEqual: return "UGreaterThanEqual"; break;
    case saOp_SGreaterThanEqual: return "SGreaterThanEqual"; break;
    case saOp_ULessThan: return "ULessThan"; break;
    case saOp_SLessThan: return "SLessThan"; break;
    case saOp_ULessThanEqual: return "ULessThanEqual"; break;
    case saOp_SLessThanEqual: return "SLessThanEqual"; break;
    case saOp_FOrdEqual: return "FOrdEqual"; break;
    case saOp_FUnordEqual: return "FUnordEqual"; break;
    case saOp_FOrdNotEqual: return "FOrdNotEqual"; break;
    case saOp_FUnordNotEqual: return "FUnordNotEqual"; break;
    case saOp_FOrdLessThan: return "FOrdLessThan"; break;
    case saOp_FUnordLessThan: return "FUnordLessThan"; break;
    case saOp_FOrdGreaterThan: return "FOrdGreaterThan"; break;
    case saOp_FUnordGreaterThan: return "FUnordGreaterThan"; break;
    case saOp_FOrdLessThanEqual: return "FOrdLessThanEqual"; break;
    case saOp_FUnordLessThanEqual: return "FUnordLessThanEqual"; break;
    case saOp_FOrdGreaterThanEqual: return "FOrdGreaterThanEqual"; break;
    case saOp_FUnordGreaterThanEqual: return "FUnordGreaterThanEqual"; break;
    case saOp_ShiftRightLogical: return "ShiftRightLogical"; break;
    case saOp_ShiftRightArithmetic: return "ShiftRightArithmetic"; break;
    case saOp_ShiftLeftLogical: return "ShiftLeftLogical"; break;
    case saOp_BitwiseOr: return "BitwiseOr"; break;
    case saOp_BitwiseXor: return "BitwiseXor"; break;
    case saOp_BitwiseAnd: return "BitwiseAnd"; break;
    case saOp_Not: return "Not"; break;
    case saOp_BitFieldInsert: return "BitFieldInsert"; break;
    case saOp_BitFieldSExtract: return "BitFieldSExtract"; break;
    case saOp_BitFieldUExtract: return "BitFieldUExtract"; break;
    case saOp_BitReverse: return "BitReverse"; break;
    case saOp_BitCount: return "BitCount"; break;
    case saOp_DPdx: return "DPdx"; break;
    case saOp_DPdy: return "DPdy"; break;
    case saOp_Fwidth: return "Fwidth"; break;
    case saOp_DPdxFine: return "DPdxFine"; break;
    case saOp_DPdyFine: return "DPdyFine"; break;
    case saOp_FwidthFine: return "FwidthFine"; break;
    case saOp_DPdxCoarse: return "DPdxCoarse"; break;
    case saOp_DPdyCoarse: return "DPdyCoarse"; break;
    case saOp_FwidthCoarse: return "FwidthCoarse"; break;
    case saOp_EmitVertex: return "EmitVertex"; break;
    case saOp_EndPrimitive: return "EndPrimitive"; break;
    case saOp_EmitStreamVertex: return "EmitStreamVertex"; break;
    case saOp_EndStreamPrimitive: return "EndStreamPrimitive"; break;
    case saOp_ControlBarrier: return "ControlBarrier"; break;
    case saOp_MemoryBarrier: return "MemoryBarrier"; break;
    case saOp_AtomicLoad: return "AtomicLoad"; break;
    case saOp_AtomicStore: return "AtomicStore"; break;
    case saOp_AtomicExchange: return "AtomicExchange"; break;
    case saOp_AtomicCompareExchange: return "AtomicCompareExchange"; break;
    case saOp_AtomicCompareExchangeWeak: return "AtomicCompareExchangeWeak"; break;
    case saOp_AtomicIIncrement: return "AtomicIIncrement"; break;
    case saOp_AtomicIDecrement: return "AtomicIDecrement"; break;
    case saOp_AtomicIAdd: return "AtomicIAdd"; break;
    case saOp_AtomicISub: return "AtomicISub"; break;
    case saOp_AtomicSMin: return "AtomicSMin"; break;
    case saOp_AtomicUMin: return "AtomicUMin"; break;
    case saOp_AtomicSMax: return "AtomicSMax"; break;
    case saOp_AtomicUMax: return "AtomicUMax"; break;
    case saOp_AtomicAnd: return "AtomicAnd"; break;
    case saOp_AtomicOr: return "AtomicOr"; break;
    case saOp_AtomicXor: return "AtomicXor"; break;
    case saOp_Phi: return "Phi"; break;
    case saOp_LoopMerge: return "LoopMerge"; break;
    case saOp_SelectionMerge: return "SelectionMerge"; break;
    case saOp_Label: return "Label"; break;
    case saOp_Branch: return "Branch"; break;
    case saOp_BranchConditional: return "BranchConditional"; break;
    case saOp_Switch: return "Switch"; break;
    case saOp_Kill: return "Kill"; break;
    case saOp_Return: return "Return"; break;
    case saOp_ReturnValue: return "ReturnValue"; break;
    case saOp_Unreachable: return "Unreachable"; break;
    case saOp_LifetimeStart: return "LifetimeStart"; break;
    case saOp_LifetimeStop: return "LifetimeStop"; break;
    case saOp_GroupAsyncCopy: return "GroupAsyncCopy"; break;
    case saOp_GroupWaitEvents: return "GroupWaitEvents"; break;
    case saOp_GroupAll: return "GroupAll"; break;
    case saOp_GroupAny: return "GroupAny"; break;
    case saOp_GroupBroadcast: return "GroupBroadcast"; break;
    case saOp_GroupIAdd: return "GroupIAdd"; break;
    case saOp_GroupFAdd: return "GroupFAdd"; break;
    case saOp_GroupFMin: return "GroupFMin"; break;
    case saOp_GroupUMin: return "GroupUMin"; break;
    case saOp_GroupSMin: return "GroupSMin"; break;
    case saOp_GroupFMax: return "GroupFMax"; break;
    case saOp_GroupUMax: return "GroupUMax"; break;
    case saOp_GroupSMax: return "GroupSMax"; break;
    case saOp_ReadPipe: return "ReadPipe"; break;
    case saOp_WritePipe: return "WritePipe"; break;
    case saOp_ReservedReadPipe: return "ReservedReadPipe"; break;
    case saOp_ReservedWritePipe: return "ReservedWritePipe"; break;
    case saOp_ReservedReadPipePackets: return "ReservedReadPipePackets"; break;
    case saOp_ReservedWritePipePackets: return "ReservedWritePipePackets"; break;
    case saOp_CommitReadPipe: return "CommitReadPipe"; break;
    case saOp_CommitWritePipe: return "CommitWritePipe"; break;
    case saOp_IsValidReservedId: return "IsValidReservedId"; break;
    case saOp_GetNumPipePackets: return "GetNumPipePackets"; break;
    case saOp_GetMaxPipePackets: return "GetMaxPipePackets"; break;
    case saOp_GroupReserveReadPipePackets: return "GroupReserveReadPipePackets"; break;
    case saOp_GroupReserveWritePipePackets: return "GroupReserveWritePipePackets"; break;
    case saOp_GroupCommitReadPipe: return "GroupCommitReadPipe"; break;
    case saOp_GroupCommitWritePipe: return "GroupCommitWritePipe"; break;
    case saOp_EnqueueMarker: return "EnqueueMarker"; break;
    case saOp_EnqueueKernel: return "EnqueueKernel"; break;
    case saOp_GetKernelINDrangeSubGroupCount: return "GetKernelINDrangeSubGroupCount"; break;
    case saOp_GetKernelINDrangeMaxSubGroupSize: return "GetKernelINDrangeMaxSubGroupSize"; break;
    case saOp_GetKernelIWorkGroupSize: return "GetKernelIWorkGroupSize"; break;
    case saOp_GetKernelIPreferredWorkGroupSizeMultiple: return "GetKernelIPreferredWorkGroupSizeMultiple"; break;
    case saOp_RetainEvent: return "RetainEvent"; break;
    case saOp_ReleaseEvent: return "ReleaseEvent"; break;
    case saOp_CreateUserEvent: return "CreateUserEvent"; break;
    case saOp_IsValidEvent: return "IsValidEvent"; break;
    case saOp_SetUserEventStatus: return "SetUserEventStatus"; break;
    case saOp_CaptureEventProfilingInfo: return "CaptureEventProfilingInfo"; break;
    case saOp_GetDefaultQueue: return "GetDefaultQueue"; break;
    case saOp_BuildNDRange: return "BuildNDRange"; break;
    case saOp_ImageSparseSampleImplicitLod: return "ImageSparseSampleImplicitLod"; break;
    case saOp_ImageSparseSampleExplicitLod: return "ImageSparseSampleExplicitLod"; break;
    case saOp_ImageSparseSampleDrefImplicitLod: return "ImageSparseSampleDrefImplicitLod"; break;
    case saOp_ImageSparseSampleDrefExplicitLod: return "ImageSparseSampleDrefExplicitLod"; break;
    case saOp_ImageSparseFetch: return "ImageSparseFetch"; break;
    case saOp_ImageSparseGather: return "ImageSparseGather"; break;
    case saOp_ImageSparseDrefGather: return "ImageSparseDrefGather"; break;
    case saOp_ImageSparseTexelResident: return "ImageSparseTexelResident"; break;
    case saOp_NoLine: return "NoLine"; break;
    case saOp_AtomicFlagTestAndSet: return "AtomicFlagTestAndSet"; break;
    case saOp_AtomicFlagClear: return "AtomicFlagClear"; break;
    case saOp_ImageSparseRead: return "ImageSparseRead"; break;
    case saOp_SizeOf: return "SizeOf"; break;
    case saOp_TypePipeStorage: return "TypePipeStorage"; break;
    case saOp_ConstantPipeStorage: return "ConstantPipeStorage"; break;
    case saOp_CreatePipeFromPipeStorage: return "CreatePipeFromPipeStorage"; break;
    case saOp_GetKernelLocalSizeForSubgroupCount: return "GetKernelLocalSizeForSubgroupCount"; break;
    case saOp_GetKernelMaxNumSubgroups: return "GetKernelMaxNumSubgroups"; break;
    case saOp_TypeNamedBarrier: return "TypeNamedBarrier"; break;
    case saOp_NamedBarrierInitialize: return "NamedBarrierInitialize"; break;
    case saOp_MemoryNamedBarrier: return "MemoryNamedBarrier"; break;
    case saOp_ModuleProcessed: return "ModuleProcessed"; break;
    case saOp_ExecutionModeId: return "ExecutionModeId"; break;
    case saOp_DecorateId: return "DecorateId"; break;
    case saOp_GroupNonUniformElect: return "GroupNonUniformElect"; break;
    case saOp_GroupNonUniformAll: return "GroupNonUniformAll"; break;
    case saOp_GroupNonUniformAny: return "GroupNonUniformAny"; break;
    case saOp_GroupNonUniformAllEqual: return "GroupNonUniformAllEqual"; break;
    case saOp_GroupNonUniformBroadcast: return "GroupNonUniformBroadcast"; break;
    case saOp_GroupNonUniformBroadcastFirst: return "GroupNonUniformBroadcastFirst"; break;
    case saOp_GroupNonUniformBallot: return "GroupNonUniformBallot"; break;
    case saOp_GroupNonUniformInverseBallot: return "GroupNonUniformInverseBallot"; break;
    case saOp_GroupNonUniformBallotBitExtract: return "GroupNonUniformBallotBitExtract"; break;
    case saOp_GroupNonUniformBallotBitCount: return "GroupNonUniformBallotBitCount"; break;
    case saOp_GroupNonUniformBallotFindLSB: return "GroupNonUniformBallotFindLSB"; break;
    case saOp_GroupNonUniformBallotFindMSB: return "GroupNonUniformBallotFindMSB"; break;
    case saOp_GroupNonUniformShuffle: return "GroupNonUniformShuffle"; break;
    case saOp_GroupNonUniformShuffleXor: return "GroupNonUniformShuffleXor"; break;
    case saOp_GroupNonUniformShuffleUp: return "GroupNonUniformShuffleUp"; break;
    case saOp_GroupNonUniformShuffleDown: return "GroupNonUniformShuffleDown"; break;
    case saOp_GroupNonUniformIAdd: return "GroupNonUniformIAdd"; break;
    case saOp_GroupNonUniformFAdd: return "GroupNonUniformFAdd"; break;
    case saOp_GroupNonUniformIMul: return "GroupNonUniformIMul"; break;
    case saOp_GroupNonUniformFMul: return "GroupNonUniformFMul"; break;
    case saOp_GroupNonUniformSMin: return "GroupNonUniformSMin"; break;
    case saOp_GroupNonUniformUMin: return "GroupNonUniformUMin"; break;
    case saOp_GroupNonUniformFMin: return "GroupNonUniformFMin"; break;
    case saOp_GroupNonUniformSMax: return "GroupNonUniformSMax"; break;
    case saOp_GroupNonUniformUMax: return "GroupNonUniformUMax"; break;
    case saOp_GroupNonUniformFMax: return "GroupNonUniformFMax"; break;
    case saOp_GroupNonUniformBitwiseAnd: return "GroupNonUniformBitwiseAnd"; break;
    case saOp_GroupNonUniformBitwiseOr: return "GroupNonUniformBitwiseOr"; break;
    case saOp_GroupNonUniformBitwiseXor: return "GroupNonUniformBitwiseXor"; break;
    case saOp_GroupNonUniformLogicalAnd: return "GroupNonUniformLogicalAnd"; break;
    case saOp_GroupNonUniformLogicalOr: return "GroupNonUniformLogicalOr"; break;
    case saOp_GroupNonUniformLogicalXor: return "GroupNonUniformLogicalXor"; break;
    case saOp_GroupNonUniformQuadBroadcast: return "GroupNonUniformQuadBroadcast"; break;
    case saOp_GroupNonUniformQuadSwap: return "GroupNonUniformQuadSwap"; break;
    case saOp_CopyLogical: return "CopyLogical"; break;
    case saOp_PtrEqual: return "PtrEqual"; break;
    case saOp_PtrNotEqual: return "PtrNotEqual"; break;
    case saOp_PtrDiff: return "PtrDiff"; break;  
  }

  return "";
}

static const char* sa__sectionToString(sa_uint32_t section) {
  switch(section) {
    case saSectionType_Capability: return "Capability"; break;
    case saSectionType_Extensions: return "Extensions"; break;
    case saSectionType_Imports: return "Imports"; break;
    case saSectionType_MemoryModel: return "MemoryModel"; break;
    case saSectionType_EntryPoints: return "EntryPoints"; break;
    case saSectionType_ExecutionModes: return "ExecutionModes"; break;
    case saSectionType_Debug: return "Debug"; break;
    case saSectionType_Annotations: return "Annotations"; break;
    case saSectionType_Types: return "Types"; break;
    case saSectionType_Functions: return "Functions"; break;
  }

  return "";
}

/**
 * @brief Short whites are spaces or tabs
 * 
 * @param c 
 * @return sa_bool 
 */
static sa_bool sa__isShortWhite(sa_uint8_t c) {
  return c == ' ' || c == '\t';
}

static void* sa__copyMemory(const void* pSrc, void* pDst, sa_uint32_t size) {
  for(sa_uint32_t i = 0; i < size; i++) {
    sa_ptr8(pDst)[i] = sa_ptr8(pSrc)[i];
  }

  return pDst;
}

static void* sa__setMemory(void* pMem, int value, sa_uint32_t size) {
  for(sa_uint32_t i = 0; i < size; i++) {
    sa_ptr8(pMem)[i] = value;
  }

  return pMem;
}

static sa_uint32_t sa__compareString(const char* a, const char* b) {
  sa_uint32_t index = 0;

  while(a[index] && b[index]) {
    if(a[index] - b[index] != 0) 
      return a[index] - b[index];

    index++;
  }

  return a[index] - b[index];
}

static sa_uint32_t sa__lengthString(const char* str) {
  sa_uint32_t len = 0;
  
  while(str[len] != '\0')
    len++;

  return len;
}

static sa_bool sa__isCharDigit(sa_uint8_t c) {
  return c >= '0' && c <= '9';
}

static sa_bool sa__isStringInteger(const char* str) {
  for(sa_uint32_t i = 0; i < sa__lengthString(str); i++) {
    if(!sa__isCharDigit(str[i]))
      return SA_FALSE;
  }
  
  return SA_TRUE;
}

static sa_bool sa__isStringFloat(const char* str) {
  for(sa_uint32_t i = 0; i < sa__lengthString(str); i++) {
    if(!sa__isCharDigit(str[i]) && str[i] != '.')
      return SA_FALSE;
  }
  
  return SA_TRUE;
}

static sa_int32_t sa__stringToInt(const char* str) {
  char* p = (char*)&str[0];
  sa_int32_t result = 0;

  if(*p == '-')
    p++;

  while(*p && sa__isCharDigit(*p)) {
    result *= 10;
    result += *p - '0';
    p++;
  }

  return str[0] == '-' ? -result : result;
}

static float sa__stringToFloat(const char* str) {
  char* p = (char*)&str[0];
  float left = 0.0f;
  float right = 0.0f;

  if(*p == '-')
    p++;

  while(*p && sa__isCharDigit(*p)) {
    left *= 10.0f;
    left += (float)(*p - '0');
    p++;
  }

  if(*p == '.') {
    while(*p && sa__isCharDigit(*p)) {
      right += (float)(*p - '0');
      right /= 10.0f;
      p++;
    }
  }

  return str[0] == '-' ? -(left + right) : (left + right);
}

static char* sa__intToString(sa_int32_t value) {
  static char str[32] = {0};

  sa__setMemory(str, 0, 32 * sizeof(char));

  sa_uint32_t num = value;
  sa_uint32_t numCounter = 0;

  if(num < 0) {
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

  if(value < 0)
    str[0] = '-';

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

#include <stdio.h>
static void sa__errMsg(const char* fmt, ...) {
  printf("%\n", fmt);

  sa_uint32_t count = 0;

  for(sa_uint32_t i = 0; i < sa__lengthString(fmt); i++) {
    if(fmt[i] == '%')
    count++;
  }

  __gAssemblerErrorMessages.messagesAmount++;
  __gAssemblerErrorMessages.pMessages = (char**)sa_realloc(__gAssemblerErrorMessages.pMessages, sizeof(char*) * __gAssemblerErrorMessages.messagesAmount);
  // If you look closely at how this is handled, it is so unoptimally done
  __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_malloc(sizeof(char));
  
  va_list args;
  va_start(args, count);
  
  sa_uint32_t msgSize = 0;

  for(sa_uint32_t i = 0; i < sa__lengthString(fmt); i++) {
    if(fmt[i] == '%') {
      msgSize++;

      switch(fmt[i + 1]) {
      // Char
      case 'c':
        msgSize++;
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - 1] = va_arg(args, char);
        break;

      // Int
      case 'd':
        sa_int32_t numberInt = va_arg(args, sa_int32_t); 
        char* number = sa__intToString(numberInt);
        sa_uint32_t numLength = sa__lengthString(number);

        msgSize += numLength;
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
        sa__copyMemory(number, &__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - numLength - 1], numLength);
        break;

      // Float
      case 'f':
        float fnumberFloat = va_arg(args, float);
        char* fnumber = sa__floatToString(fnumberFloat, 6);
        sa_uint32_t fnumLength = sa__lengthString(fnumber);

        msgSize += fnumLength;
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
        sa__copyMemory(fnumber, &__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - fnumLength - 1], fnumLength);
        break;

      // String
      case 's':
        char* string = va_arg(args, char*);
        sa_uint32_t length = sa__lengthString(string);

        msgSize += length;
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
        sa__copyMemory(string, &__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - length - 1], length);
        break;

      // Hex
      case 'x':
        sa_uint32_t hnumberUint = va_arg(args, sa_uint32_t); 
        char* hnumber = sa__hexToString(hnumberUint);
        sa_uint32_t hnumLength = sa__lengthString(hnumber);

        msgSize += hnumLength;
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
        sa__copyMemory(hnumber, &__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - hnumLength - 1], hnumLength);
        break;

      // The percent chacarter
      case '%':
        msgSize++;
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
        __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - 1] = '%';
        break;

      default:
        break;
      }

      i += 2;
      msgSize--;
    }

    msgSize++;
    __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1] = (char*)sa_realloc(__gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1], sizeof(char) * msgSize);
    __gAssemblerErrorMessages.pMessages[__gAssemblerErrorMessages.messagesAmount - 1][msgSize - 1] = fmt[i];
  }

  va_end(args);
}

static sa_uint32_t sa__genId() {
  return __gIdGeneratorHoldValue++;
}

static void sa__resetId() {
  __gIdGeneratorHoldValue = 0;
}

static sa_uint32_t sa__getOperandSectionType(sa_uint16_t op) {
  switch(op) {
  case saOp_Capability:
    return saSectionType_Capability;

  case saOp_Extension:
    return saSectionType_Extensions;

  case saOp_ExtInstImport:
    return saSectionType_Imports;

  case saOp_MemoryModel:
    return saSectionType_MemoryModel;

  case saOp_EntryPoint:
    return saSectionType_EntryPoints;

  case saOp_ExecutionMode:
  case saOp_ExecutionModeId:
    return saSectionType_ExecutionModes;

  case saOp_String:
  case saOp_Source:
  case saOp_SourceExtension:
  case saOp_SourceContinued:
  case saOp_Name:
  case saOp_MemberName:
  case saOp_ModuleProcessed:
    return saSectionType_Debug;

  case saOp_Decorate:
  case saOp_MemberDecorate:
  case saOp_DecorationGroup:
  case saOp_GroupDecorate:
  case saOp_GroupMemberDecorate:
  case saOp_DecorateId:
    return saSectionType_Annotations;

  case saOp_TypeArray:
  case saOp_TypeBool:
  case saOp_TypeDeviceEvent:
  case saOp_TypeEvent:
  case saOp_TypeFloat:
  case saOp_TypeForwardPointer:
  case saOp_TypeFunction:
  case saOp_TypeImage:
  case saOp_TypeInt:
  case saOp_TypeMatrix:
  case saOp_TypeNamedBarrier:
  case saOp_TypeOpaque:
  case saOp_TypePipe:
  case saOp_TypePipeStorage:
  case saOp_TypePointer:
  case saOp_TypeQueue:
  case saOp_TypeReserveId:
  case saOp_TypeRuntimeArray:
  case saOp_TypeSampledImage:
  case saOp_TypeSampler:
  case saOp_TypeStruct:
  case saOp_TypeVector:
  case saOp_TypeVoid:
  case saOp_Variable:
  case saOp_Undef:
  case saOp_Line:
  case saOp_Constant:
  case saOp_ConstantComposite:
    return saSectionType_Types;
  }

  return saSectionType_Functions;
}

static sa_uint32_t sa__getOrCreateSpirvId(sa__spirvIdTable_t* pIds, const char* name) {
  for(sa_uint32_t i = 0; i < pIds->idCount; i++) {
    if(sa__compareString(name, pIds->pIds[i].textId) == 0)
      return pIds->pIds[i].binaryId;
  }

  pIds->idCount++;
  pIds->pIds = (sa__spirvId_t*)sa_realloc(pIds->pIds, sizeof(sa__spirvId_t) * pIds->idCount);
  sa__setMemory(&pIds->pIds[pIds->idCount - 1], 0, sizeof(pIds->pIds[pIds->idCount - 1]));
  pIds->pIds[pIds->idCount - 1].binaryId = sa__genId();
  sa__copyMemory(name, pIds->pIds[pIds->idCount - 1].textId, sa__lengthString(name));

  return pIds->pIds[pIds->idCount - 1].binaryId;
}

static sa_bool sa__spirvIdExist(sa__spirvIdTable_t* pIds, sa_uint32_t id) {
   for(sa_uint32_t i = 0; i < pIds->idCount; i++) {
    if(pIds->pIds[i].binaryId == id)
      return SA_TRUE;
  }

  return SA_FALSE;
}

static void sa__createJustId(sa__spirvIdTable_t* pIds, sa_uint32_t id) {
  if(sa__spirvIdExist(pIds, id))
    return;

  pIds->idCount++;
  pIds->pIds = (sa__spirvId_t*)sa_realloc(pIds->pIds, sizeof(sa__spirvId_t) * pIds->idCount);
  sa__setMemory(&pIds->pIds[pIds->idCount - 1], 0, sizeof(pIds->pIds[pIds->idCount - 1]));
  pIds->pIds[pIds->idCount - 1].binaryId = sa__genId();
}

static const char* sa__getSpirvName(sa__spirvIdTable_t* pIds, sa_uint32_t id) {
  for(sa_uint32_t i = 0; i < pIds->idCount; i++) {
    if(pIds->pIds[i].binaryId == id)
      return pIds->pIds[i].textId;
  }

  return SA_NULL;
}

static void sa__freeSpirvIdTable(sa__spirvIdTable_t* pIds) {
  sa_free(pIds->pIds);
  pIds->pIds = SA_NULL;
  pIds->idCount = 0;
}

/**
 * @brief Checks if such a mnemonic is valid SPIRV operand
 * 
 * @param mnemonic 
 * @return sa_uint32_t index to that operand table or UINT32_MAX if non existant
 */
static sa_uint32_t sa__findLowLevelMnemonic(const char* mnemonic) {
  for(sa_uint32_t i = 0; i < sizeof(SA_ASSEMBLER_LOW_LEVEL_OPCODES) / sizeof(SA_ASSEMBLER_LOW_LEVEL_OPCODES[0]); i++) {
    if(sa__compareString(SA_ASSEMBLER_LOW_LEVEL_OPCODES[i].opcodeMnemonic, mnemonic) == 0)
      return i;
  }

  return SA_UINT32_MAX;
}

static void sa__addInstructionFormatted(sa__assemblySection_t* pSection, sa_uint16_t wordSize, sa_uint16_t op, ...) {
  if(!pSection) {
    sa__errMsg("Cannot find section");
    
    return;
  }

  pSection->instCount++;
  pSection->pInst = (sa__assemblyInstruction_t*)sa_realloc(pSection->pInst, sizeof(sa__assemblyInstruction_t) * pSection->instCount);
  pSection->pInst[pSection->instCount - 1].opCode = op;
  pSection->pInst[pSection->instCount - 1].wordSize = wordSize;
  pSection->pInst[pSection->instCount - 1].words = (sa_uint32_t*)sa_realloc(pSection->pInst[pSection->instCount - 1].words, sizeof(sa_uint32_t) * wordSize);

  va_list args;
  va_start(args, wordSize - 1);

  for(sa_uint32_t i = 0; i < wordSize - 1; i++) {
    pSection->pInst[pSection->instCount - 1].words[i] = va_arg(args, sa_uint32_t);
  }

  va_end(args);
}

static void sa__addInstruction(sa__assemblySection_t* pSection, sa_uint16_t wordSize, sa_uint16_t op, sa_uint32_t* words) {
  if(!pSection) {
    sa__errMsg("Cannot find section");
    
    return;
  }
  
  pSection->instCount++;

  if(pSection->pInst)
    pSection->pInst = (sa__assemblyInstruction_t*)sa_realloc(pSection->pInst, sizeof(sa__assemblyInstruction_t) * pSection->instCount);
  else
    pSection->pInst = (sa__assemblyInstruction_t*)sa_calloc(pSection->instCount, sizeof(sa__assemblyInstruction_t));

  // -1 for last instruction
  sa__setMemory(&pSection->pInst[pSection->instCount - 1], 0, sizeof(pSection->pInst[pSection->instCount - 1]));

  pSection->pInst[pSection->instCount - 1].opCode = op;
  pSection->pInst[pSection->instCount - 1].wordSize = wordSize;

  if(pSection->pInst[pSection->instCount - 1].words)
    pSection->pInst[pSection->instCount - 1].words = (sa_uint32_t*)sa_realloc(pSection->pInst[pSection->instCount - 1].words, sizeof(sa_uint32_t) * wordSize);
  else
    pSection->pInst[pSection->instCount - 1].words = (sa_uint32_t*)sa_calloc(wordSize, sizeof(sa_uint32_t));

  if(!pSection->pInst[pSection->instCount - 1].words) {
    sa__errMsg("Cannot allocate memory for pSection->pInst[pSection->instCount - 1].words");

    return;
  }

  sa__copyMemory(words, pSection->pInst[pSection->instCount - 1].words, (wordSize - 1) * sizeof(sa_uint32_t));
}

static void sa_freeAssembly(sa_assembly_t* pAsm) {
  for(sa_uint32_t sect = 0; sect < saSectionType_COUNT; sect++) {
    for(sa_uint32_t i = 0; i < pAsm->section[sect].instCount; i++) {
      sa_free(pAsm->section[sect].pInst[i].words);
      pAsm->section[sect].pInst[i].words = SA_NULL;
      pAsm->section[sect].pInst[i].opCode = 0;
      pAsm->section[sect].pInst[i].wordSize = 0;
    }

    sa_free(pAsm->section[sect].pInst);
    pAsm->section[sect].pInst = SA_NULL;
    pAsm->section[sect].instCount = 0;
  }

  pAsm->header.bounds = 0;
  pAsm->header.generator = 0;
  pAsm->header.magic = 0;
  pAsm->header.schema = 0;
  pAsm->header.version = 0;
}

static void sa_freeSPIRV(sa_uint8_t* spirvBin) {
  sa_free(spirvBin);
}

/**
 * @brief Gets enum for instruction. Instruction must support that enum
 * 
 * @param instructionIndex index to the instruction/opcode table
 * @param enumName enum mnemonic
 * @return sa_uint32_t enum or SA_UINT32_t on error
 */
static sa_uint32_t sa__getLowLevelInstructionEnum(sa_uint32_t instructionIndex, const char* enumName) {
  sa_uint32_t enumsCount = 0;

  for(; enumsCount < saAsmEnum_COUNT; enumsCount++) {
    if(SA_ASSEMBLER_LOW_LEVEL_OPCODES[instructionIndex].possibleEnumerant[enumsCount] == SA_UINT32_MAX)
      break;
  }

  for(sa_uint32_t eId = 0; eId < enumsCount; eId++) {
    sa_uint32_t tableId = SA_ASSEMBLER_LOW_LEVEL_OPCODES[instructionIndex].possibleEnumerant[eId];

    printf("Checking %d\n", tableId);

    for(sa_uint32_t i = 0; i < SA_MAX_ENUMERANT_ENTRIES; i++) {
      if(!SA_ASSEMBLER_LOW_LEVEL_ENUMS[tableId][i].enumerantMnemonic)
        break;

      if(sa__compareString(SA_ASSEMBLER_LOW_LEVEL_ENUMS[tableId][i].enumerantMnemonic, enumName) == 0)
        return SA_ASSEMBLER_LOW_LEVEL_ENUMS[tableId][i].enumerant;
    }
  }

  return SA_UINT32_MAX;
}

#include <stdio.h>

static sa_uint32_t sa__parserLowLevelSkipComment(const char * const spirvAssemblyStrStart) {
  char* p = (char*)spirvAssemblyStrStart;

  // Skip all white chars at the beginning as they are valid 
  while(sa__isShortWhite(*p))
    p++;

  // If after skipping all the white spaces p == NL, return it as a just an empty line
  if(*p == '\n' || *p == '\r') {
    if(*p == '\r')
      p++;

    p++;

    return p - spirvAssemblyStrStart; 
  }

  if(*p == ';') {
    // Skip comment (full line)
    while(*p != '\n' && *p)
      p++;

    if(*p == 0)
      return p - spirvAssemblyStrStart;

    // Skip NL char
    p++;

    // return pointer to the next line
    return p - spirvAssemblyStrStart;
  }

  if(*p == 0)
    return p - spirvAssemblyStrStart;

  return SA_UINT32_MAX;
}

/**
 * @brief Parses instruction to the spirv assembly struct
 * 
 * @param spirvAssemblyStrStart string of a spirv assembly where spirvAssemblyStrStart[0] == first letter of a operand mnemonic
 * @param pAssembly pointer to the assemblt
 * @param pInsideFn set to true if operand is inside function, this is required to skip most of opcodes that would be created in wrong section, like variables 
 * @return sa_uint32_t index to the next operand or SA_UINT32_MAX on error
 */
static sa_uint32_t sa__parseLowLevelInstruction(const char * const spirvAssemblyStrStart, sa_assembly_t* pAssembly, sa__spirvIdTable_t* pIds, sa_bool* pInsideFn, sa_uint32_t line) {
  if(!spirvAssemblyStrStart) {
    sa__errMsg("Cannot find start point for assembly plain");

    return SA_UINT32_MAX;
  }

  if(!pAssembly) {
    sa__errMsg("Cannot find assembly struct");

    return SA_UINT32_MAX;
  }

  if(!pIds) {
    sa__errMsg("Cannot find spirv IDs table");

    return SA_UINT32_MAX;
  }

  char* p = (char*)spirvAssemblyStrStart;

  sa_uint32_t skipCommentSize = sa__parserLowLevelSkipComment(p);

  if(skipCommentSize != SA_UINT32_MAX)
    return skipCommentSize;

  // That should be mnemonic start
  char* mnemonicStart = p;

  // Everything in lowest level assembly is divided by spaces, 1 instruction per line
  while(!sa__isShortWhite(*p) && *p != '\n' && *p != 0)
    p++;

  char* mnemonicEnd = p;
  char mnemonic[256] = {0};

  // Copy read mnemonic to temp mnemonic buffer
  sa__copyMemory(mnemonicStart, mnemonic, mnemonicEnd - mnemonicStart);

  // Check if mnemonic actually exist
  sa_uint32_t instructionIndex = sa__findLowLevelMnemonic(mnemonic);

  if(instructionIndex == SA_UINT32_MAX) {
    sa__errMsg("Mnemonic \"%s\" does not exist @ line %d", mnemonic, line);

    return SA_UINT32_MAX;
  }

  sa_uint16_t operand = SA_ASSEMBLER_LOW_LEVEL_OPCODES[instructionIndex].opcode;
  sa_uint16_t minimalWordCount = SA_ASSEMBLER_LOW_LEVEL_OPCODES[instructionIndex].wordCount;

  if(operand == saOp_Function)
    *pInsideFn = SA_TRUE;

  if(operand == saOp_FunctionEnd)
    *pInsideFn = SA_FALSE;

  // -1 to cut off mnemonic of opcode
  char** allArgs = (char**)sa_calloc(minimalWordCount - 1, sizeof(char*));
  sa_uint32_t argsCounter = 0;

  if(!allArgs) {
    sa__errMsg("Cannot allocate memory for allArgs");

    return SA_UINT32_MAX;
  }
  
  while(*p != '\n' && *p != 0) {
    while(sa__isShortWhite(*p))
      p++;

    char* argStart = p;

    while(!sa__isShortWhite(*p) && *p != '\n' && *p != 0)
      p++;

    char* argEnd = p;

    allArgs[argsCounter] = (char*)sa_calloc(argEnd - argStart + 1, sizeof(char));
    argsCounter++;

    if(!allArgs[argsCounter - 1]) {
      sa__errMsg("Cannot allocate memory for allArgs[argsCounter - 1]");

      return SA_UINT32_MAX;
    }
  }

  sa_uint32_t* words = (sa_uint32_t*)sa_calloc(minimalWordCount - 1, sizeof(sa_uint32_t));
  sa_uint32_t wordsCount = 0;

  if(!words) {
    sa__errMsg("Cannot allocate memory for words");

    return SA_UINT32_MAX;
  }

  for(sa_uint32_t i = 0; i < argsCounter; i++) {
    char* arg = allArgs[i];
    
    // Check if function is not extendable yet exceeds word count
    if(!SA_ASSEMBLER_LOW_LEVEL_OPCODES[instructionIndex].plusVariable && wordsCount > (minimalWordCount - 1)) {
      sa__errMsg("%s is not extendable type, yet assembler found more than %d arguments @ line %d", mnemonic, minimalWordCount, line);

      return SA_UINT32_MAX;
    }

    if(arg[0] == '%') {
      sa_uint32_t id = sa__getOrCreateSpirvId(pIds, &arg[1]);
      words[wordsCount] = SA_CONVERT(id);
      wordsCount++;
    }
    else if(sa__isStringInteger(arg)) {
      sa_uint32_t value = sa__stringToInt(arg);
      words[wordsCount] = SA_CONVERT(value);
      wordsCount++;
    }
    else if(sa__isStringFloat(arg)) {
      float value = sa__stringToFloat(arg);
      words[wordsCount] = SA_CONVERT(*sa_ptr32(&value));
      wordsCount++;
    }
    else if(arg[0] == '\"' && arg[sa__lengthString(arg) - 1] == '\"') {
      char* buffer = (char*)sa_calloc(sa__lengthString(arg) - 1, sizeof(arg[0]));
      if(!buffer) {
        sa__errMsg("Cannot allocate memory for buffer");

        return SA_UINT32_MAX;
      }
      sa__copyMemory(&arg[1], buffer, sa__lengthString(arg) - 2);

      for(sa_uint32_t j = 0; j < (sa__lengthString(buffer) + sizeof(sa_uint32_t) - 1) / sizeof(sa_uint32_t); j++) {
        // XXX: Can be that conversion to LE be deleted?
        words[wordsCount] = SA_CONVERT(SA_LITTLE_ENDIAN32(*sa_ptr32(&buffer[j * 4])));
        wordsCount++;

        if(wordsCount >= (minimalWordCount - 1))
          words = (sa_uint32_t*)sa_realloc(words, wordsCount * sizeof(sa_uint32_t));

        if(!words) {
          sa__errMsg("Cannot reallocate memory for words");

          return SA_UINT32_MAX;
        }
      }

      sa_free(buffer);
      buffer = SA_NULL;
    }
    else {
      sa_uint32_t enumerant = sa__getLowLevelInstructionEnum(instructionIndex, arg);

      words[wordsCount] = SA_CONVERT(enumerant);
      wordsCount++;

      if(enumerant == SA_UINT32_MAX) {
        sa__errMsg("Wrong enumerant for %s: %s @ line %d", sa__opcodeToString(operand), arg, line);

        return SA_UINT32_MAX;
      }
    }

    if(wordsCount >= (minimalWordCount - 1))
      words = (sa_uint32_t*)sa_realloc(words, wordsCount * sizeof(sa_uint32_t));

    if(!words) {
      sa__errMsg("Cannot reallocate memory for words");

      return SA_UINT32_MAX;
    }
  }

  // Add instruction
  sa__addInstruction(&pAssembly->section[*pInsideFn ? saSectionType_Functions : sa__getOperandSectionType(operand)], wordsCount + 1, operand, words);

  sa_free(words);
  words = SA_NULL;

  for(sa_uint32_t i = 0; i < argsCounter; i++) {
    sa_free(allArgs[i]);
    allArgs[i] = SA_NULL;
  }
  
  sa_free(allArgs);
  allArgs = SA_NULL;

  return p - spirvAssemblyStrStart;
}

/**
 * @brief Return code as a pointer
 * 
 * @param spirvBasicAssembly text to process
 * @param pCodeSizeOut assembled code size
 * @return sa_uint32_t* Assembled code
 */
static void sa_assembleSPIRV(const char* spirvBasicAssembly, sa_assembly_t* pAssembly) {
  sa__resetId();

  sa_uint32_t index = 0;

  sa__spirvIdTable_t ids = {0};
  sa_bool insideFn = SA_FALSE;
  sa_uint32_t line = 1;

  while(spirvBasicAssembly[index]) {
    sa_uint32_t next = sa__parseLowLevelInstruction(&spirvBasicAssembly[index], pAssembly, &ids, &insideFn, line);

    printf("There is next: %u\n", next);

    if(next == SA_UINT32_MAX) {
      while(spirvBasicAssembly[index] != '\n')
        index++;
      
      index++;

      sa__errMsg("Something went wrong @ line %d", line);

      return;
    }

    if(next != SA_UINT32_MAX) {
      index += next;
    }

    line++;
  }

  for(sa_uint32_t i = 0; i < ids.idCount; i++) {
    if(ids.pIds[i].textId[0] != '%') {
      // +3 to add 1 more uint if there is not equal amount of bytes
      const sa_uint32_t wordSize = ((sa__lengthString(ids.pIds[i].textId) + 3) / sizeof(sa_uint32_t)) + 1;
      // Allocate for string and 1 uint32 value for id
      sa_uint32_t* words = (sa_uint32_t*)sa_calloc(wordSize, sizeof(sa_uint32_t));

      words[0] = ids.pIds[i].binaryId;

      // -1 for already added binaryId
      for(sa_uint32_t j = 0; j < wordSize - 1; i++) {
        words[j + 1] = 
          (((sa_uint32_t)ids.pIds[i].textId[j * 4 + 0]) << 24) | 
          (((sa_uint32_t)ids.pIds[i].textId[j * 4 + 1]) << 16) | 
          (((sa_uint32_t)ids.pIds[i].textId[j * 4 + 2]) << 8) |
          ((sa_uint32_t)ids.pIds[i].textId[j * 4 + 3]);
      }

      // Word size needs to account operand, so +1
      sa__addInstruction(&pAssembly->section[sa__getOperandSectionType(saOp_Name)], wordSize + 1, saOp_Name, words);

      sa_free(words);
      words = SA_NULL;
    }
  }

  sa__freeSpirvIdTable(&ids);
}

/**
 * @brief Bakes assembly into actual binary data
 * 
 * @param pAssembly assembly to bake
 * @param pBinarySizeOut size of binary table 
 * @return sa_binary pointer to binary data itself
 */
static sa_uint8_t* sa_bakeSPIRV(const sa_assembly_t* const pAssembly, sa_uint32_t* pBinarySizeOut) {
  if(!pAssembly) {
    sa__errMsg("No assembly");

    return SA_NULL;
  }

  if(!pBinarySizeOut) {
    sa__errMsg("No size output");

    return SA_NULL;
  }

  sa_uint8_t* sbin = (sa_uint8_t*)sa_malloc(sizeof(sa__assemblyHeader_t));
  sa_uint32_t sbinSize = (sizeof(sa__assemblyHeader_t) / sizeof(sa_uint32_t));

  // Always converted to little endian
  sa_ptr32(sbin)[0] = SA_CONVERT(SA_SPIRV_MAGIC_NUMBER);
  sa_ptr32(sbin)[1] = SA_CONVERT(SA_SPIRV_VERSION);
  sa_ptr32(sbin)[2] = SA_CONVERT(SA_SPIRV_GENERATOR_ID);
  sa_ptr32(sbin)[3] = SA_CONVERT(pAssembly->header.bounds);
  sa_ptr32(sbin)[4] = SA_CONVERT(pAssembly->header.schema);

  for(sa_uint32_t sectId = 0; sectId < saSectionType_COUNT; sectId++) {
    const sa__assemblySection_t* const pSect = &pAssembly->section[sectId];

    for(sa_uint32_t instId = 0; instId < pSect->instCount; instId++) {
      const sa_uint32_t instStart = sbinSize;
      sa__assemblyInstruction_t* pInst = &pSect->pInst[instId];

      sbinSize += pInst->wordSize;
      sbin = (sa_uint8_t*)sa_realloc(sbin, sbinSize * sizeof(sa_uint32_t));

      sa_ptr32(sbin)[instStart + 0] = SA_CONVERT((((sa_uint32_t)pInst->wordSize) << 16) | pInst->opCode);

      for(sa_uint32_t argId = 1; argId < pInst->wordSize; argId++) {
        sa_ptr32(sbin)[instStart + argId] = SA_CONVERT(pInst->words[argId]);
      }
    }
  }

  return sbin;
}

// FIXME: Disassembler must recognise sections
/**
 * @brief disassemble SPIR-V shader
 * 
 * @param shaderBin 
 * @param shaderSize size must be as a amount of elements inside SPIR-V in 32bit format (so read file length / sizeof(int))
 */
static void sa_disassembleSPIRV(sa_assembly_t* pAsm, sa_uint8_t* shaderBin, sa_uint32_t shaderSize) {
  sa_uint32_t index = 0;

  sa__setMemory(pAsm, 0, sizeof(*pAsm));

  if(SA_CONVERT(sa_ptr32(shaderBin)[0]) != SA_SPIRV_MAGIC_NUMBER) {
    sa__errMsg("SPIR-V Magic number does not match file magic number!\n");

    return;
  }

  pAsm->header.magic = SA_CONVERT(sa_ptr32(shaderBin)[0]);
  pAsm->header.version = SA_CONVERT(sa_ptr32(shaderBin)[1]);
  pAsm->header.generator = SA_CONVERT(sa_ptr32(shaderBin)[2]);
  pAsm->header.bounds = SA_CONVERT(sa_ptr32(shaderBin)[3]);
  pAsm->header.schema = SA_CONVERT(sa_ptr32(shaderBin)[4]);
  // Advance index by 5 to skip header
  index += 5;

  sa_bool opcodeInFunction = SA_FALSE;

  for(; index < shaderSize; index++) {
    // This contain opcode and word count
    sa_uint32_t word = SA_CONVERT(sa_ptr32(shaderBin)[index]);

    // Mask off LSB as this is still in LE, then convert
    sa_uint16_t opcode = word & 0x0000FFFF;
    sa_uint16_t wordCount = (word >> 16) & 0x0000FFFF;

    // Add all words
    sa_uint32_t* words = (sa_uint32_t*)sa_calloc(wordCount - 1, sizeof(sa_uint32_t));

    if(opcode == saOp_Function)
      opcodeInFunction = SA_TRUE;

    if(opcode == saOp_FunctionEnd)
      opcodeInFunction = SA_FALSE;

    for(sa_uint32_t i = 0; i < wordCount - 1; i++) {
      index++;
      words[i] = SA_CONVERT(sa_ptr32(shaderBin)[index]);
    }

    // Make instruction
    sa__addInstruction(&pAsm->section[opcodeInFunction ? saSectionType_Functions : sa__getOperandSectionType(opcode)], wordCount, opcode, words);

    // Free temp garbage
    sa_free(words);
    words = SA_NULL;
  }
}

#endif