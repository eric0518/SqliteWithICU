//
//  mmicu_tokenizer.h
//  Tokenizer
//
//  Created by wzm on 2018/9/5.
//  Copyright © 2018 wzm. All rights reserved.
//

#ifndef mmicu_tokenizer_h
#define mmicu_tokenizer_h

#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN
/**
 初始化分词器
 */
int fts5_mmicuCreate(void *pCtx, const char **azArg, int nArg, Fts5Tokenizer **ppOut);

/**
 删除缓存的分词器
 */
void fts5_mmicuDelete(Fts5Tokenizer *pTok);

/**
 分词
 xToken //插入token->字符串的映射
 pCtx   //上下文
 tflags //一般为0(准确)或上述值
 pToken //当前串中所对应的索引token(通常为整个串中的子串)
 nToken //pToken的字节大小
 iStart //pToken所在串中对应的起始位置
 iEnd  //pToken所在串的末尾位置
 */
int fts_mmicu_Tokenize(Fts5Tokenizer *pTokenizer,
                        void *pCtx,
                        int flags,
                        const char *pText,
                        int nText,
                        int (*xToken)(
                                      void *pCtx,
                                      int tflags,
                                      const char *pToken,
                                      int nToken,
                                      int iStart,
                                      int iEnd)
                        );

EXTERN_C_END
#endif /* mmicu_tokenizer_h */
