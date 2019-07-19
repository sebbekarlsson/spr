#ifndef SPR_LEXER_H
#define SPR_LEXER_H
#include "spr_token.h"


typedef struct SPR_LEXER_STRUCT
{
    char c;
    unsigned int c_i;
    unsigned int line_n;
    char* content;
} spr_lexer_T;

spr_lexer_T* init_spr_lexer(char* content);

void spr_lexer_advance(spr_lexer_T* lexer);

void spr_lexer_skip_whitespace(spr_lexer_T* lexer);

spr_token_T* spr_lexer_collect_id(spr_lexer_T* lexer);

spr_token_T* spr_lexer_collect_number(spr_lexer_T* lexer);

spr_token_T* lexer_advance_with_token(spr_lexer_T* lexer, int type);

spr_token_T* spr_lexer_get_next_token(spr_lexer_T* lexer);
#endif
