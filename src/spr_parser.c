#include "include/spr_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


spr_parser_T* init_spr_parser(spr_lexer_T* lexer)
{
    spr_parser_T* parser = calloc(1, sizeof(struct SPR_PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = spr_lexer_get_next_token(lexer);
    parser->row_width = 0;

    return parser;
}

void spr_parser_eat(spr_parser_T* parser, int token_type)
{
    if (parser->current_token->type != token_type)
    {
        printf(
            "[Line %d] Unexpected token `%s`, was expecting `%d`.\n",
            parser->lexer->line_n,
            parser->current_token->value,
            token_type
        );
        exit(1);
    }
    else if (parser->current_token->type == token_type)
    {
        parser->current_token = spr_lexer_get_next_token(parser->lexer);
    }
}

spr_T* spr_parser_parse(spr_parser_T* parser)
{
    spr_AST_T* ast_info = (void*) 0;
    spr_AST_T** ast_frames = (void*) 0;
    size_t ast_frames_size = 0;

    while (parser->current_token->type != TOKEN_EOF)
    {
        if (ast_info == (void*) 0)
        {
            ast_info = spr_parser_parse_info(parser);

            parser->row_width = (int) ast_info->info_value[0];
        }

        while (strcmp(parser->current_token->value, "frame") == 0)
        {
            ast_frames_size += 1;
            ast_frames = realloc(ast_frames, ast_frames_size * sizeof(struct SPR_AST_STRUCT*));
            ast_frames[ast_frames_size - 1] = spr_parser_parse_frame(parser);
        }
    }

    return init_spr(ast_info->info_value, ast_frames, ast_frames_size);
}

spr_AST_T* spr_parser_parse_info(spr_parser_T* parser)
{
    // info 3 6 255 255 255 1.6 1
    spr_parser_eat(parser, TOKEN_ID);
    
    float width = (float) atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);

    float height = (float) atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);

    float red = (float) atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);

    float green = (float) atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);

    float blue = (float) atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);

    float delay = atof(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_FLOAT);
    
    unsigned int animated = (unsigned int) atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);

    spr_AST_T* ast_info = init_spr_ast(AST_INFO);

    ast_info->info_value[0] = width;
    ast_info->info_value[1] = height;
    ast_info->info_value[2] = red;
    ast_info->info_value[3] = green;
    ast_info->info_value[4] = blue;
    ast_info->info_value[5] = delay;
    ast_info->info_value[6] = animated;

    return ast_info;
}

spr_AST_T* spr_parser_parse_frame(spr_parser_T* parser)
{
    spr_parser_eat(parser, TOKEN_ID);

    spr_AST_T* ast_frame = init_spr_ast(AST_FRAME);
    ast_frame->frame_value_pixel_rows = (void*) 0;
    ast_frame->frame_value_pixel_rows_size = 0;

    while (parser->current_token->type != TOKEN_ID)
    {
        if (parser->current_token->type == TOKEN_EOF)
            break;

        ast_frame->frame_value_pixel_rows_size += 1;
        ast_frame->frame_value_pixel_rows = realloc(
            ast_frame->frame_value_pixel_rows,
            ast_frame->frame_value_pixel_rows_size * sizeof(struct SPR_AST_STRUCT*)
        );
        ast_frame->frame_value_pixel_rows[ast_frame->frame_value_pixel_rows_size - 1] = spr_parser_parse_pixel_row(parser);
    }

    return ast_frame;
}

spr_AST_T* spr_parser_parse_pixel(spr_parser_T* parser)
{
    // 255,255,255,255;
    spr_AST_T* ast_pixel = init_spr_ast(AST_PIXEL);

    ast_pixel->pixel_value[0] = atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);


    spr_parser_eat(parser, TOKEN_COMMA);

    ast_pixel->pixel_value[1] = atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);
    spr_parser_eat(parser, TOKEN_COMMA);
    
    ast_pixel->pixel_value[2] = atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);
    spr_parser_eat(parser, TOKEN_COMMA);

    
    ast_pixel->pixel_value[3] = atoi(parser->current_token->value);
    spr_parser_eat(parser, TOKEN_INT);

    return ast_pixel;
}

spr_AST_T* spr_parser_parse_pixel_row(spr_parser_T* parser)
{
    spr_AST_T* ast_pixel_row = init_spr_ast(AST_PIXEL_ROW);
    ast_pixel_row->pixel_row_size = 0;
    ast_pixel_row->pixel_row_value = (void*) 0;

    for (int i = 0; i < parser->row_width; i++)
    {
        ast_pixel_row->pixel_row_size += 1;
        ast_pixel_row->pixel_row_value = realloc(
            ast_pixel_row->pixel_row_value,
            ast_pixel_row->pixel_row_size * sizeof(struct SPR_AST_STRUCT*)
        );
        ast_pixel_row->pixel_row_value[ast_pixel_row->pixel_row_size - 1] = spr_parser_parse_pixel(parser);
        
        if (i < parser->row_width - 1)
            spr_parser_eat(parser, TOKEN_SEMI);
    }

    return ast_pixel_row;
}
