// For tests
//#include "test_spirvsba.h"
#include "spirva.h"
#include <stdio.h>

static void printAssembly(sa_assembly_t* pAsm) {
  sa__spirvIdTable_t ids = { SA_NULL, 0 };

  for(sa_uint32_t sect = 0; sect < saSectionType_COUNT; sect++) {
    for(sa_uint32_t i = 0; i < pAsm->section[sect].instCount; i++) {
      if(pAsm->section[sect].pInst[i].opCode == saOp_Name || pAsm->section[sect].pInst[i].opCode == saOp_ExtInstImport) {
        char name[512] = {0};
        sa_uint32_t nameSize = 0;

        for(sa_uint32_t j = 1; j < pAsm->section[sect].pInst[i].wordSize; j++) {
          sa_uint32_t name4b = pAsm->section[sect].pInst[i].words[j];

          name[nameSize++] = sa_ptr8(&name4b)[0];
          name[nameSize++] = sa_ptr8(&name4b)[1];
          name[nameSize++] = sa_ptr8(&name4b)[2];
          name[nameSize++] = sa_ptr8(&name4b)[3];
        }

        sa__createSpirvNameId(&ids, name, pAsm->section[sect].pInst[i].words[0]);
      }
    }
  }

  for(sa_uint32_t sect = 0; sect < saSectionType_COUNT; sect++) {
    printf("\nSection %s:\n", sa__sectionToString(sect));

    for(sa_uint32_t i = 0; i < pAsm->section[sect].instCount; i++) {
      //if(pAsm->section[sect].pInst[i].opCode == saOp_Name || 
      //  pAsm->section[sect].pInst[i].opCode == saOp_Source ||
      //  pAsm->section[sect].pInst[i].opCode == saOp_SourceExtension ||
      //  pAsm->section[sect].pInst[i].opCode == saOp_SourceContinued ||
      //  pAsm->section[sect].pInst[i].opCode == saOp_MemberName ||
      //  pAsm->section[sect].pInst[i].opCode == saOp_MemoryModel ||
      //  pAsm->section[sect].pInst[i].opCode == saOp_Capability
      //)
      //  continue;

      printf("%s ", sa__opcodeToString(pAsm->section[sect].pInst[i].opCode));
      
      for(sa_uint32_t j = 0; j < pAsm->section[sect].pInst[i].wordSize - 1; j++) {
        sa_uint32_t word = pAsm->section[sect].pInst[i].words[j];
        const char* name = sa__getSpirvName(&ids, word);
        if(name != SA_NULL) {
          printf("%%%s ", name);
        }
        else {
          printf("%%%d(%c%c%c%c) ", word, (char)word, (char)(word >> 8), (char)(word >> 16), (char)(word >> 24));
        }
      }

      puts("");
    }
  }

  sa__freeSpirvIdTable(&ids);
}

const char* tokToStr(sa_uint32_t tok) {
  switch(tok) {
  case saToken_Identifier: return "Identifier";
  case saToken_Punctuator: return "Punctuator";
  case saToken_Operator: return "Operator";
  case saToken_Literal: return "Literal";
  case saToken_Module: return "Module";
  case saToken_Import: return "Import";
  case saToken_Entry: return "Entry";
  case saToken_Execmode: return "Execmode";
  case saToken_Uniform: return "Uniform";
  case saToken_Input: return "Input";
  case saToken_Output: return "Output";
  case saToken_Inout: return "Inout";
  case saToken_Decorate: return "Decorate";
  case saToken_Fn: return "Fn";
  case saToken_Endfn: return "Endfn";
  case saToken_Void: return "Void";
  case saToken_Bool: return "Bool";
  case saToken_Int: return "Int";
  case saToken_Uint: return "Uint";
  case saToken_Float: return "Float";
  case saToken_Vector: return "Vector";
  case saToken_Matrix: return "Matrix";
  case saToken_Image: return "Image";
  case saToken_SampledImage: return "SampledImage";
  case saToken_Var: return "Var";
  case saToken_Ret: return "Ret";
  case saToken_Branch: return "Branch";
  case saToken_Struct: return "Struct";
  case saToken_True: return "True";
  case saToken_False: return "False";
  case saToken_Null: return "Null";
  }

  return "<UNKNOWN>";
}

void printLex(sa_lexer_t* pLex) {
  for(sa_uint32_t i = 0; i < pLex->tokenCount; i++) {
    printf("%s | %s\n", tokToStr(pLex->pTokens[i].token), pLex->pTokens[i].tokenId);
  }
} 

void saveBin(const char* filename, void* bin, sa_uint32_t binSize) {
  FILE* f = fopen(filename, "wb+");
  fwrite(bin, sizeof(sa_uint8_t), binSize, f);
  fclose(f);
}

int main() {
  FILE* spirv = fopen("../example1.sba", "rb");

  if(!spirv) {
    printf("[ERROR]: Cannot open spirv shader!\n");
    
    return 0;
  }

  fseek(spirv, 0, SEEK_END);
  int length = ftell(spirv);
  fseek(spirv, 0, SEEK_SET);

  sa_uint8_t* spirvBin = (sa_uint8_t*)calloc(length + 1, sizeof(sa_int8_t));
  fread(spirvBin, sizeof(sa_uint8_t), length, spirv);

  fclose(spirv);

  printf("%s\n", spirvBin);

  sa_lexer_t spaLex;
  sa_lexSPIRV(spirvBin, &spaLex);

  printLex(&spaLex);

  /*sa_assembly_t sasm = {0};
  sa_assembleSPIRV(spirvBin, &sasm);

  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("[ERROR]: %s\n", __gAssemblerErrorMessages.pMessages[i]);
  }

  sa_uint32_t binarySize = 0;
  sa_uint8_t* binary = sa_bakeSPIRV(&sasm, &binarySize);

  printf("Binary size: %d\n", binarySize);

  saveBin("test_comp_spv.spv", binary, binarySize);

  printf("Binary saved\n");

  sa_free(binary);

  //printAssembly(&sasm);
  sa_freeAssembly(&sasm);
  free(spirvBin);*/

  /*sa_assembly_t spvasm;
  sa_disassembleSPIRV(&spvasm, spirvBin, length / sizeof(sa_uint32_t));

  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("%s", __gAssemblerErrorMessages.pMessages[i]);
  }

  printAssembly(&spvasm);

  sa_freeAssembly(&spvasm);
  free(spirvBin);*/

  return 0;
}