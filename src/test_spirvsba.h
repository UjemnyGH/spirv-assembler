#ifndef _SPRIVSBA_
#define _SPRIVSBA_

#include "spirva.h"
#include <stdio.h>

/* 
 * -----------------------------------
 *        SPIRV Basic Assembly
 * -----------------------------------
 */

#define SA_SBA_MODULE_NAME_PREFIX "__MODULE_NAME:"
#define SA_SBA_IMPORT_MODULE_NAME_PREFIX "_IMPORT_MODULE_NAME:" 

typedef enum sa__TokenType_e {
  saToken_None = 0,
  saToken_Identifier,
  saToken_Punctuator,
  saToken_Operator,
  saToken_Literal,

  saToken_Module,
  saToken_Import,
  saToken_Entry,
  saToken_Execmode,
  saToken_Uniform,
  saToken_Input,
  saToken_Output,
  saToken_Inout,
  saToken_Decorate,
  saToken_Fn,
  saToken_Endfn,
  saToken_Void,
  saToken_Bool,
  saToken_Int,
  saToken_Uint,
  saToken_Float,
  saToken_Vector,
  saToken_Matrix,
  saToken_Image,
  saToken_SampledImage,
  saToken_Var,
  saToken_Ret,
  saToken_Branch,
  saToken_Struct,
  saToken_True,
  saToken_False,
  saToken_Null,

  saToken_COUNT
} sa__TokenType_t;

// TODO: Lexer for SBA
typedef struct sa__token_t {
  char tokenId[512];
  sa__TokenType_t token;
} sa__token_t;

typedef struct sa_lexer_s {
  sa__token_t* pTokens;
  sa_uint32_t tokenCount;
} sa_lexer_t;

typedef struct sa__lexerKeywordConnection_s {
  const char* keyword;
  sa_uint32_t keywordToken;
} sa__lexerKeywordConnection_t;

const sa__lexerKeywordConnection_t __gsaSBAKeywords[] = {
  { "module", saToken_Module },
  { "import", saToken_Import },
  { "entry", saToken_Entry },
  { "exec_mode", saToken_Execmode },
  { "uniform", saToken_Uniform },
  { "input", saToken_Input },
  { "output", saToken_Output },
  { "inout", saToken_Inout },
  { "decorate", saToken_Decorate },
  { "fn", saToken_Fn },
  { "endfn", saToken_Endfn },
  { "void", saToken_Void },
  { "bool", saToken_Bool },
  { "int", saToken_Int },
  { "uint", saToken_Uint },
  { "float", saToken_Float },
  { "vector", saToken_Vector },
  { "matrix", saToken_Matrix },
  { "image", saToken_Image },
  { "sampled_image", saToken_SampledImage },
  { "var", saToken_Var },
  { "ret", saToken_Ret },
  { "branch", saToken_Branch },
  { "struct", saToken_Struct },
  { "true", saToken_True },
  { "false", saToken_False },
  { "null", saToken_Null },
};

const char __gsaSBAOperators[] = "+-*/=%^&|!~";
const char __gsaSBAPunctuators[] = "[]{}(),.:";

static void sa__lexerAddToken(sa_lexer_t* pLexer, sa__TokenType_t token, const char* text, sa_uint32_t textSize) {
  pLexer->tokenCount++;  
  pLexer->pTokens = (sa__token_t*)sa_realloc(pLexer->pTokens, pLexer->tokenCount * sizeof(sa__token_t));

  pLexer->pTokens[pLexer->tokenCount - 1].token = token;
  sa__setMemory(pLexer->pTokens[pLexer->tokenCount - 1].tokenId, 0, sizeof(pLexer->pTokens[pLexer->tokenCount - 1].tokenId));
  sa__copyMemory(text, pLexer->pTokens[pLexer->tokenCount - 1].tokenId, textSize);
}

static void sa_freeLexer(sa_lexer_t* pLexer) {
  if(pLexer->pTokens)
    sa_free(pLexer->pTokens);
  pLexer->pTokens = SA_NULL;
  pLexer->tokenCount = 0;
}

static sa_bool sa__lexerCheckPunctuator(char c) {
  for(sa_uint32_t i = 0; i < sa__lengthString(__gsaSBAPunctuators); i++) {
    if(__gsaSBAPunctuators[i] == c)
      return SA_TRUE;
  }

  return SA_FALSE;
}

static sa_bool sa__lexerCheckOperator(char c) {
  for(sa_uint32_t i = 0; i < sa__lengthString(__gsaSBAOperators); i++) {
    if(__gsaSBAOperators[i] == c)
      return SA_TRUE;
  }

  return SA_FALSE;
}

static sa_uint32_t sa__lexerCheckKeyword(const char* kw) {
  for(sa_uint32_t i = 0; i < sizeof(__gsaSBAKeywords) / sizeof(__gsaSBAKeywords[0]); i++) {
    if(sa__compareString(kw, __gsaSBAKeywords[i].keyword) == 0) 
      return __gsaSBAKeywords[i].keywordToken;
  }

  return SA_UINT32_MAX;
}

static sa_bool sa__lexerCheckLiteral(const char* literal, sa_uint32_t size) {
  for(sa_uint32_t i = 0; i < size; i++) {
    if(!sa__isCharDigit(literal[i]) && literal[i] != '.')
      return SA_FALSE;
  }

  return SA_TRUE;
}

static void sa__lexerAddKeyword(sa_lexer_t* pLexerData, sa__TokenType_t type, const char* text, sa_uint32_t textSize) {
  sa__lexerAddToken(pLexerData, (sa__TokenType_t)type, text, textSize);
}

static void sa__lexerAddIdentifier(sa_lexer_t* pLexerData, const char* text, sa_uint32_t textSize) {
  sa__lexerAddToken(pLexerData, saToken_Identifier, text, textSize);
}

static void sa__lexerAddLiteral(sa_lexer_t* pLexerData, const char* lit, sa_uint32_t litSize) {
  sa__lexerAddToken(pLexerData, saToken_Literal, lit, litSize);
}

static void sa__lexerAddPunctuator(sa_lexer_t* pLexerData, char punctuatorCh) {
  char punc[2] = { punctuatorCh, 0 };
  sa__lexerAddToken(pLexerData, saToken_Punctuator, punc, 2);
}

static void sa__lexerAddOperator(sa_lexer_t* pLexerData, char operatorCh) {
  char op[2] = { operatorCh, 0 };
  sa__lexerAddToken(pLexerData, saToken_Punctuator, op, 2);
}

static void sa__lexerAddAnything(sa_lexer_t* pLexerData, const char* start, const char* current) {
  // This function adds anything, not nothing, skip
  if(current - start < 1)
    return;
  
  char* textPart = (char*)sa_calloc(current - start + 1, sizeof(char));
  sa__copyMemory(start, textPart, current - start);

  sa_uint32_t keyword = sa__lexerCheckKeyword(textPart);

  if(keyword != SA_UINT32_MAX)
    sa__lexerAddKeyword(pLexerData, (sa__TokenType_t)keyword, textPart, current - start + 1);
  else if(sa__lexerCheckLiteral(textPart, current - start))
    sa__lexerAddLiteral(pLexerData, textPart, current - start + 1);
  else
    sa__lexerAddIdentifier(pLexerData, textPart, current - start + 1);
  
  sa_free(textPart);
  textPart = SA_NULL;
}

static void sa_lexSPIRV(const char* spirvBasicAssembly, sa_lexer_t* pLexerData) {
  const sa_uint32_t sbaSize = sa__lengthString(spirvBasicAssembly); 
  char* strippedSBA = (char*)sa_calloc(sbaSize, sizeof(char));
  sa_uint32_t strippedSBASize = 0;

  pLexerData->tokenCount = 0;
  pLexerData->pTokens = SA_NULL;

  for(sa_uint32_t i = 0; i < sbaSize; i++) {
    if(spirvBasicAssembly[i] != '\r') {
      strippedSBA[strippedSBASize] = spirvBasicAssembly[i];
      strippedSBASize++;
    }
  }

  strippedSBA = (char*)sa_realloc(strippedSBA, (strippedSBASize + 1) * sizeof(char));

  char* p = &strippedSBA[0];
  char* start = p;

  while(*p) {
    //  || sa__checkPunctuator(*p) || sa__checkOperator(*p)
    if(*p == ' ' || *p == '\n' || *p == '\t') {
      sa__lexerAddAnything(pLexerData, start, p);

      p++;
      start = p;

      while(*start == ' ' || *start == '\t' || *start == '\n')
        start++;
    }
    else if(sa__lexerCheckPunctuator(*p)) {
      char punc = *p;

      sa__lexerAddAnything(pLexerData, start, p);
      sa__lexerAddPunctuator(pLexerData, punc);

      p++;
      start = p;

      while(*start == ' ' || *start == '\t' || *start == '\n')
        start++;
    }
    else if(sa__lexerCheckOperator(*p)) {
      char op = *p;

      sa__lexerAddAnything(pLexerData, start, p);
      sa__lexerAddOperator(pLexerData, op);

      p++;
      start = p;

      while(*start == ' ' || *start == '\t' || *start == '\n')
        start++;
    }
    // Skip comment
    else if(*p == ';') {
      while(*p != '\n')
        p++;
      p++;
      start = p;

      while(*start == ' ' || *start == '\t' || *start == '\n')
        start++;
    }

    putchar(*p);

    p++;
  }

  sa_free(strippedSBA);
}

/*
  saToken_Entry,
  saToken_Execmode,
  saToken_Uniform,
  saToken_Input,
  saToken_Output,
  saToken_Inout,
  saToken_Decorate,
  saToken_Fn,
  saToken_Endfn,
  saToken_Void,
  saToken_Bool,
  saToken_Int,
  saToken_Uint,
  saToken_Float,
  saToken_Vector,
  saToken_Matrix,
  saToken_Image,
  saToken_SampledImage,
  saToken_Var,
  saToken_Ret,
  saToken_Branch,
  saToken_Struct,
  saToken_True,
  saToken_False,
  saToken_Null,*/

#define SA_SBA_DEBUG_VOIDT "DEBUG_VOID_TYPE"
#define SA_SBA_SPECIAL_IMPORT "GLSLstd450"

static sa_uint32_t* sa__sbaMakeStringIntoWords(const char* str, sa_uint32_t* pOutSize) {
  sa_uint32_t* words = (sa_uint32_t*)sa_calloc((sa__lengthString(str) + 4) / sizeof(sa_uint32_t), sizeof(sa_uint32_t));
  sa__copyMemory(str, words, sa__lengthString(str));
  *pOutSize = (sa__lengthString(str) + 4) / sizeof(sa_uint32_t);

  return words;
}

static void sa__sbaAddDebugVoidType(sa_assembly_t* pAssembly, sa__spirvIdTable_t* pIds) {
  if(!sa__spirvIdNameExist(pIds, SA_SBA_DEBUG_VOIDT)) {
    sa_uint32_t id = sa__getOrCreateSpirvId(pIds, SA_SBA_DEBUG_VOIDT);

    sa__addInstruction(&pAssembly->section[saSectionType_Types], 2, saOp_TypeVoid, &id);
  }
}

static void sa__sbaAddName(sa_assembly_t* pAssembly, const char* name, sa_uint32_t id) {
  // Make that full token name into uint32
  sa_uint32_t nameWordsSize = 0;
  sa_uint32_t* nameWords = sa__sbaMakeStringIntoWords(name, sa__lengthString(name), &nameWordsSize);

  // Alloc memory for all the words needed
  sa_uint32_t* words = (sa_uint32_t*)sa_calloc(nameWordsSize + 1, sizeof(sa_uint32_t));
  words[0] = id;
  sa__copyMemory(nameWords, &words[1], nameWordsSize * sizeof(sa_uint32_t));

  sa__addInstruction(&pAssembly->section[saSectionType_Debug], nameWordsSize + 2, saOp_Name, words);
  
  sa_free(nameWords);
  sa_free(words);
}

static sa_uint32_t sa__sbaResolveModule(sa_assembly_t* pAssembly, sa__spirvIdTable_t* pIds, sa__token_t* pStartingToken) {
  if(pStartingToken[0].token == saToken_Module) {
    if(pStartingToken[1].token == saToken_Identifier) {
      // Add debug void type
      sa__sbaAddDebugVoidType(pAssembly, pIds);

      // Get id for that debug void
      sa_uint32_t id = sa__getOrCreateSpirvId(pIds, SA_SBA_DEBUG_VOIDT);

      // Make full module token name 
      const sa_uint32_t fullTokenSize = sa__lengthString(pStartingToken[1].tokenId) + sa__lengthString(SA_SBA_MODULE_NAME_PREFIX) + 1;
      char* fullToken = (char*)sa_calloc(fullTokenSize, sizeof(char));
      sa__copyMemory(SA_SBA_MODULE_NAME_PREFIX, &fullToken[0], sa__lengthString(SA_SBA_MODULE_NAME_PREFIX));
      sa__copyMemory(pStartingToken[1].tokenId, &fullToken[sa__lengthString(SA_SBA_MODULE_NAME_PREFIX)], sa__lengthString(pStartingToken[1].tokenId));

      sa__sbaAddName(pAssembly, fullToken, id);
      
      sa_free(fullToken);

      return 2;
    }
    else {
      sa__errMsg("Module needs identifier, found %s", pStartingToken[1].tokenId);
    }
  }

  return 0;
}

static sa_uint32_t sa__sbaResolveImport(sa_assembly_t* pAssembly, sa__spirvIdTable_t* pIds, sa__token_t* pStartingToken) {
  if(pStartingToken[0].token == saToken_Import) {
    if(pStartingToken[1].token == saToken_Identifier && pStartingToken[2].token == saToken_Identifier) {
      sa_uint32_t id = sa__getOrCreateSpirvId(pIds, &pStartingToken[1].tokenId[1]);
      sa__sbaAddName(pAssembly, &pStartingToken[1].tokenId[1], id);

      if(sa__compareString(SA_SBA_SPECIAL_IMPORT, pStartingToken[2].tokenId) == 0) {
        sa_uint32_t nameWordsSize = 0;
        sa_uint32_t* nameWords = sa__sbaMakeStringIntoWords("GLSL.std.450", sa__lengthString("GLSL.std.450"), &nameWordsSize);

        // Alloc memory for all the words needed
        sa_uint32_t* words = (sa_uint32_t*)sa_calloc(nameWordsSize + 1, sizeof(sa_uint32_t));
        words[0] = id;
        sa__copyMemory(nameWords, &words[1], nameWordsSize * sizeof(sa_uint32_t));

        sa__addInstruction(&pAssembly->section[saSectionType_Extensions], nameWordsSize + 2, saOp_ExtInstImport, words);

        sa_free(words);
        sa_free(nameWords);

        return 3;
      }

      // Make full import name
      const sa_uint32_t fullTokenSize = sa__lengthString(pStartingToken[2].tokenId) + sa__lengthString(SA_SBA_IMPORT_MODULE_NAME_PREFIX) + 1;
      char* fullToken = (char*)sa_calloc(fullTokenSize, sizeof(char));
      sa__copyMemory(SA_SBA_IMPORT_MODULE_NAME_PREFIX, &fullToken[0], sa__lengthString(SA_SBA_IMPORT_MODULE_NAME_PREFIX));
      sa__copyMemory(pStartingToken[2].tokenId, &fullToken[sa__lengthString(SA_SBA_IMPORT_MODULE_NAME_PREFIX)], sa__lengthString(pStartingToken[2].tokenId));

      sa__sbaAddName(pAssembly, fullToken, id);

      sa_free(fullToken);

      return 3;
    }
    else {
      sa__errMsg("Values after import must be identifiers");
    }
  }

  return 0;
}

static sa_uint32_t sa__sbaResolveEntryPoint(sa_assembly_t* pAssembly, sa__spirvIdTable_t* pIds, sa__token_t* pStartingToken) {
  // Check for entry
  if(pStartingToken[0].token == saToken_Entry) {
    sa_uint32_t entryPointSize = 1;
    sa_uint32_t* words = (sa_uint32_t*)sa_calloc(entryPointSize, sizeof(sa_uint32_t));
    
    // Check for token validity
    words[0] = sa__getLowLevelInstructionEnum(saOp_EntryPoint, pStartingToken[1].tokenId); 

    if(words[0] != SA_UINT32_MAX && &pStartingToken[2] && pStartingToken[2].token == saToken_Identifier) {
      entryPointSize++;
      words = (sa_uint32_t*)sa_realloc(words, entryPointSize * sizeof(sa_uint32_t));
      sa_uint32_t fnId = sa__getOrCreateSpirvId(pIds, &pStartingToken[2].tokenId[1]);
      words[1] = fnId;

      sa__sbaAddName(pAssembly, &pStartingToken[2].tokenId[1], fnId);

      sa_uint32_t entryNameWordsSize = 0;
      sa_uint32_t* entryNameWords = sa__sbaMakeStringIntoWords(&pStartingToken[2].tokenId[1], &entryNameWordsSize);

      // TODO: Finish

      while(pStartingToken[1].token == saToken_Identifier) {
        words = (sa_uint32_t)

        entryPointSize++;
      }
    }
    else {
      sa__errMsg("Invalid shader type at entry: %s", pStartingToken[1].tokenId);
    }
  }

  return 0;
}

static void sa_assembleSBA(const char* sbaSource, sa_assembly_t* pAssembly) {
  sa__spirvIdTable_t ids;

  sa_lexer_t lex;
  sa_lexSPIRV(sbaSource, &lex);

  sa__token_t* tok = lex.pTokens;

  sa_uint32_t memoryModelWords[2] = {saAddresingModel_Logical, saMemoryModel_Vulkan};
  sa__addInstruction(&pAssembly->section[saSectionType_MemoryModel], 3, saOp_MemoryModel, memoryModelWords);

  for(sa_uint32_t i = 0; i < lex.tokenCount; i++) {

  }

  sa_freeLexer(&lex);
}

#endif