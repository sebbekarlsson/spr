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
        spr_token_free(parser->current_token);
        parser->current_token = spr_lexer_get_next_token(parser->lexer);
    }
}

spr_T* spr_parser_parse(spr_parser_T* parser)
{
    spr_AST_T* ast_info = (void*) 0;
    spr_frame_T** frames = (void*) 0;
    size_t frames_size = 0;

    while (parser->current_token->type != TOKEN_EOF)
    {
        if (ast_info == (void*) 0)
        {
            ast_info = spr_parser_parse_info(parser);

            parser->row_width = (int) ast_info->info_value[0];
        }

        while (strcmp(parser->current_token->value, "frame") == 0)
        {
            frames_size += 1;
            frames = realloc(frames, frames_size * sizeof(struct SPR_AST_STRUCT*));
            frames[frames_size - 1] = spr_parser_parse_frame(parser);
        }
    }

    spr_T* spr = init_spr(
        ast_info->info_value[0],
        ast_info->info_value[1],
        ast_info->info_value[2],
        ast_info->info_value[3],
        ast_info->info_value[4],
        ast_info->info_value[5],
        ast_info->info_value[6],
        frames,
        frames_size
    );

    free(ast_info);
    free(parser->current_token);

    return spr;
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

spr_frame_T* spr_parser_parse_frame(spr_parser_T* parser)
{
    spr_parser_eat(parser, TOKEN_ID);

    spr_frame_T* frame = calloc(1, sizeof(struct SPR_FRAME_STRUCT));

    frame->pixel_rows = (void*) 0;
    frame->pixel_rows_size = 0;

    while (parser->current_token->type != TOKEN_ID)
    {
        if (parser->current_token->type == TOKEN_EOF)
            break;

        frame->pixel_rows_size += 1;
        frame->pixel_rows = realloc(
            frame->pixel_rows,
            frame->pixel_rows_size * sizeof(struct SPR_AST_STRUCT*)
        );
        frame->pixel_rows[frame->pixel_rows_size - 1] = spr_parser_parse_pixel_row(parser);
    }

    return frame;
}

spr_pixel_T* spr_parser_parse_pixel(spr_parser_T* parser)
{
    // 255,255,255,255;
    spr_pixel_T* pixel = calloc(1, sizeof(struct SPR_PIXEL_STRUCT));

    pixel->r = atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);


    spr_parser_eat(parser, TOKEN_COMMA);

    pixel->g = atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);
    spr_parser_eat(parser, TOKEN_COMMA);
    
    pixel->b = atoi(parser->current_token->value);

    spr_parser_eat(parser, TOKEN_INT);
    spr_parser_eat(parser, TOKEN_COMMA);

    
    pixel->a = atoi(parser->current_token->value);
    spr_parser_eat(parser, TOKEN_INT);

    return pixel;
}

spr_pixel_row_T* spr_parser_parse_pixel_row(spr_parser_T* parser)
{
    spr_pixel_row_T* pixel_row = calloc(1, sizeof(struct SPR_PIXEL_ROW_STRUCT));
    pixel_row->pixels_size = 0;
    pixel_row->pixels = (void*) 0;

    for (int i = 0; i < parser->row_width; i++)
    {
        pixel_row->pixels_size += 1;
        pixel_row->pixels = realloc(
            pixel_row->pixels,
            pixel_row->pixels_size * sizeof(struct SPR_AST_STRUCT*)
        );
        pixel_row->pixels[pixel_row->pixels_size - 1] = spr_parser_parse_pixel(parser);
        
        if (i < parser->row_width - 1)
            spr_parser_eat(parser, TOKEN_SEMI);
    }

    return pixel_row;
}
