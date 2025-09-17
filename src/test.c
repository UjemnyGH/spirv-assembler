#include "spirva.h"
#include <stdio.h>

int main() {
  FILE* spirv = fopen("../test_comp.spv", "rb");

  if(!spirv) {
    printf("[ERROR]: Cannot open spirv shader!\n");
    
    return 0;
  }

  fseek(spirv, 0, SEEK_END);
  int length = ftell(spirv);
  fseek(spirv, 0, SEEK_SET);

  sa_int8_t* spirvBin = calloc(length, sizeof(sa_int8_t));
  fread(spirvBin, sizeof(sa_uint8_t), length, spirv);

  fclose(spirv);

  printf("Length: 0x%x,\n%s\n", length, spirvBin);

  sa_assembly_t spvasm;
  sa_disassembleSPIRV(&spvasm, (sa_uint32_t*)spirvBin, length / sizeof(sa_uint32_t));

  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("%s", __gAssemblerErrorMessages.pMessages[i]);
  }

  for(sa_uint32_t i = 0; i < spvasm.instCount; i++) {
    printf("%s ", sa__opcodeToString(spvasm.pInst[i].opCode));
    
    for(sa_uint32_t j = 0; j < spvasm.pInst[i].wordSize; j++) {
      sa_uint32_t word = spvasm.pInst[i].words[j];
      printf("%%%d (%c%c%c%c) ", word, (char)(word >> 24), (char)(word >> 16), (char)(word >> 8), (char)word);
    }

    puts("");
  }

  sa_freeAssembly(&spvasm);
  free(spirvBin);

  return 0;
}