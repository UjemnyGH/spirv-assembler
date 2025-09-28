<div align="center">
<span style="font-size: 48px">SPIR-V Assembler</span>

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](LICENSE)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg?style=for-the-badge)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Stage: Early Alpha](https://img.shields.io/badge/Stage-Early_Alpha-yellow?style=for-the-badge)]()

</div>

SPIR-V Assembler is made to ease out usage and importing shaders as well as enable developers to easily add it to the project, as it is single file library.

Everything here is based on [SPIR-V spec](https://registry.khronos.org/SPIR-V/specs/unified1/SPIRV.pdf), tho not every feature of SPIR-V is added here. Platform specific features are skipped for now!

---

# Current features
- Assembling .spa (SPIR-V Assembly) files (check [test_comp.spa](test_comp.spa))
- Disassembling .spv (SPIR-V Binary) files

# Usage
### For assembling .spa code:
```C
#include <stdio.h>
#include "spirva.h"

sa_uint8_t* readFile(const char* path, sa_uint32_t* pOutSize) {
  FILE* file = fopen(path, "rb");

  if(!file) {
    printf("[ERROR]: Cannot open file %s!\n", path);
    
    return SA_NULL;
  }

  fseek(file, 0, SEEK_END);
  // +1 for \0 at the end
  *pOutSize = ftell(file) + 1;
  fseek(file, 0, SEEK_SET);

  sa_uint8_t* buffer = (sa_uint8_t*)calloc(*pOutSize, sizeof(sa_uint8_t));
  // -1 to not read to much
  fread(buffer, sizeof(sa_uint8_t), *pOutSize - 1, file);

  fclose(spirfilev);

  return buffer;
}

int main() {
  sa_uint32_t spirvSrcSize = 0;
  sa_uint8_t* spirvSrc = readFile("../test_comp.spa", &spirvSrcSize);

  // Always initialize structs
  sa_assembly_t spirvAsm = {0};
  // Assemble file
  sa_assembleSPIRV(spirvSrc, &spirvAsm);

  free(spirvSrc);

  // Handle errors (optional, but useful)
  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("[ERROR]: %s\n", __gAssemblerErrorMessages.pMessages[i]);
  }

  sa_uint32_t binarySize = 0;
  // Bake SPIRV into binary, made that way to allow adding more than 1 file to the binary
  // tho you need to handle joining files yourself
  sa_uint8_t* binary = sa_bakeSPIRV(&spirvAsm, &binarySize);

  sa_freeAssembly(&spirvAsm);

  // Do whatever you want with SPIRV binary (mostly use in Vulkan)
  
  sa_free(binary);

  return 0;
}
```

### For disassembling .spv code:
```C
#include <stdio.h>
#include "spirva.h"

sa_uint8_t* readFile(const char* path, sa_uint32_t* pOutSize) {
  FILE* file = fopen(path, "rb");

  if(!file) {
    printf("[ERROR]: Cannot open file %s!\n", path);
    
    return SA_NULL;
  }

  fseek(file, 0, SEEK_END);
  // +1 for \0 at the end
  *pOutSize = ftell(file) + 1;
  fseek(file, 0, SEEK_SET);

  sa_uint8_t* buffer = (sa_uint8_t*)calloc(*pOutSize, sizeof(sa_uint8_t));
  // -1 to not read to much
  fread(buffer, sizeof(sa_uint8_t), *pOutSize - 1, file);

  fclose(spirfilev);

  return buffer;
}

int main() {
  sa_uint32_t spirvBinSize = 0;
  sa_uint8_t* spirvBin = readFile("../test_comp.spv", &spirvBinSize);

  // Always initialize structs
  sa_assembly_t spirvAsm = {0};
  // Assemble file
  sa_disassembleSPIRV(spirvBin, &spirvAsm);

  free(spirvBin);

  // Handle errors (optional, but useful)
  for(sa_uint32_t i = 0; i < __gAssemblerErrorMessages.messagesAmount; i++) {
    printf("[ERROR]: %s\n", __gAssemblerErrorMessages.pMessages[i]);
  }

  // Do whatever you want with SPIRV assembly

  sa_freeAssembly(&spirvAsm);

  return 0;
}
```

# License
MIT License

# Author
Made by Piotr "UjemnyGH" Plombon