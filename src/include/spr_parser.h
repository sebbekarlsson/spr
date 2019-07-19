#ifndef SPR_PARSER_H
#define SPR_PARSER_H
#include "spr_lexer.h"
#include "spr_token.h"
#include "spr_AST.h"
#include "spr.h"


typedef struct SPR_PARSER_STRUCT
{
    spr_lexer_T* lexer;
    spr_token_T* current_token;
    unsigned int row_width;
} spr_parser_T;

spr_parser_T* init_spr_parser(spr_lexer_T* lexer);

void spr_parser_eat(spr_parser_T* parser, int token_type);

spr_T* spr_parser_parse(spr_parser_T* parser);

spr_AST_T* spr_parser_parse_info(spr_parser_T* parser);

spr_AST_T* spr_parser_parse_frame(spr_parser_T* parser);

spr_AST_T* spr_parser_parse_pixel(spr_parser_T* parser);

spr_AST_T* spr_parser_parse_pixel_row(spr_parser_T* parser);
#endif
