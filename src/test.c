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

        sa__getOrCreateSpirvId(&ids, name);
      }
    }
  }

  for(sa_uint32_t sect = 0; sect < saSectionType_COUNT; sect++) {
    printf("\nSection %s:\n", sa__sectionToString(sect));

    for(sa_uint32_t i = 0; i < pAsm->section[sect].instCount; i++) {
      if(pAsm->section[sect].pInst[i].opCode == saOp_Name || 
        pAsm->section[sect].pInst[i].opCode == saOp_Source ||
        pAsm->section[sect].pInst[i].opCode == saOp_SourceExtension ||
        pAsm->section[sect].pInst[i].opCode == saOp_SourceContinued ||
        pAsm->section[sect].pInst[i].opCode == saOp_MemberName ||
        pAsm->section[sect].pInst[i].opCode == saOp_MemoryModel ||
        pAsm->section[sect].pInst[i].opCode == saOp_Capability
      )
        continue;

      printf("%s ", sa__opcodeToString(pAsm->section[sect].pInst[i].opCode));
      
      for(sa_uint32_t j = 1; j < pAsm->section[sect].pInst[i].wordSize; j++) {
        sa_uint32_t word = pAsm->section[sect].pInst[i].words[j];
        const char* name = sa__getSpirvName(&ids, word);
        if(name != SA_NULL) {
          printf("%%%s ", name);
        }
        else {
          printf("%%%d ", word, (char)word, (char)(word >> 8), (char)(word >> 16), (char)(word >> 24));
        }
      }

      puts("");
    }
  }

  sa__freeSpirvIdTable(&ids);
}

int main() {
  FILE* spirv = fopen("../test_comp.spa", "rb");

  if(!spirv) {
    printf("[ERROR]: Cannot open spirv shader!\n");
    
    return 0;
  }

  fseek(spirv, 0, SEEK_END);
  int length = ftell(spirv);
  fseek(spirv, 0, SEEK_SET);

  sa_uint8_t* spirvBin = (sa_uint8_t*)calloc(length, sizeof(sa_int8_t));
  fread(spirvBin, sizeof(sa_uint8_t), length, spirv);

  fclose(spirv);

  sa_assembly_t sasm = {0};
  sa_assembleSPIRV(spirvBin, &sasm);

  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("%s\n", __gAssemblerErrorMessages.pMessages[i]);

    //for(sa_uint32_t j = 0; j < sa__lengthString(__gAssemblerErrorMessages.pMessages[i]); j++) {
    //  printf("%#X ", __gAssemblerErrorMessages.pMessages[i][j]);
    //}
  }

  printAssembly(&sasm);
  sa_freeAssembly(&sasm);
  free(spirvBin);

  /*sa_assembly_t spvasm;
  sa_disassembleSPIRV(&spvasm, spirvBin, length / sizeof(sa_uint32_t));

  sa__spirvIdTable_t ids = { SA_NULL, 0 };

  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("%s", __gAssemblerErrorMessages.pMessages[i]);
  }

  for(sa_uint32_t sect = 0; sect < saSectionType_COUNT; sect++) {
    for(sa_uint32_t i = 0; i < spvasm.section[sect].instCount; i++) {
      if(spvasm.section[sect].pInst[i].opCode == saOp_Name || spvasm.section[sect].pInst[i].opCode == saOp_ExtInstImport) {
        char name[512] = {0};
        sa_uint32_t nameSize = 0;

        for(sa_uint32_t j = 1; j < spvasm.section[sect].pInst[i].wordSize; j++) {
          sa_uint32_t name4b = spvasm.section[sect].pInst[i].words[j];

          name[nameSize++] = sa_ptr8(&name4b)[0];
          name[nameSize++] = sa_ptr8(&name4b)[1];
          name[nameSize++] = sa_ptr8(&name4b)[2];
          name[nameSize++] = sa_ptr8(&name4b)[3];
        }

        sa__getOrCreateSpirvId(&ids, name);
      }
    }
  }

  for(sa_uint32_t sect = 0; sect < saSectionType_COUNT; sect++) {
    printf("\nSection %s:\n", sa__sectionToString(sect));

    for(sa_uint32_t i = 0; i < spvasm.section[sect].instCount; i++) {
      if(spvasm.section[sect].pInst[i].opCode == saOp_Name || 
        spvasm.section[sect].pInst[i].opCode == saOp_ExtInstImport ||
        spvasm.section[sect].pInst[i].opCode == saOp_Source ||
        spvasm.section[sect].pInst[i].opCode == saOp_SourceExtension ||
        spvasm.section[sect].pInst[i].opCode == saOp_SourceContinued ||
        spvasm.section[sect].pInst[i].opCode == saOp_MemberName ||
        spvasm.section[sect].pInst[i].opCode == saOp_MemoryModel ||
        spvasm.section[sect].pInst[i].opCode == saOp_Capability
      )
        continue;

      printf("%s ", sa__opcodeToString(spvasm.section[sect].pInst[i].opCode));
      
      for(sa_uint32_t j = 1; j < spvasm.section[sect].pInst[i].wordSize; j++) {
        sa_uint32_t word = spvasm.section[sect].pInst[i].words[j];
        const char* name = sa__getSpirvName(&ids, word);
        if(name != SA_NULL) {
          printf("%%%s ", name);
        }
        else {
          printf("%%%d ", word, (char)word, (char)(word >> 8), (char)(word >> 16), (char)(word >> 24));
        }
      }

      puts("");
    }
  }

  sa_freeAssembly(&spvasm);
  free(spirvBin);*/

  return 0;
}