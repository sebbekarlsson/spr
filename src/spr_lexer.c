#include "include/spr_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


char* char_to_string(char c)
{
    char* str = calloc(2, sizeof(char));
    str[0] = c;
    str[1] = '\0';

    return str;
}

spr_lexer_T* init_spr_lexer(char* content)
{
    spr_lexer_T* lexer = calloc(1, sizeof(struct SPR_LEXER_STRUCT));
    lexer->content = content;
    lexer->c_i = 0;
    lexer->c = content[lexer->c_i];
    lexer->line_n = 0;

    return lexer;
}

void spr_lexer_advance(spr_lexer_T* lexer)
{
    if (lexer->c == '\n' || lexer->c == 10)
        lexer->line_n += 1;

    if (lexer->c != '\0' && lexer->c_i < strlen(lexer->content) - 1)
    {
        lexer->c_i += 1;
        lexer->c = lexer->content[lexer->c_i];
    }
}

void spr_lexer_skip_whitespace(spr_lexer_T* lexer)
{
    while (lexer->c == ' ' || (int) lexer->c == 10)
        spr_lexer_advance(lexer);
}

spr_token_T* spr_lexer_collect_id(spr_lexer_T* lexer)
{
    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    while (isalnum(lexer->c) || lexer->c == '_')
    {
        char* strchar = char_to_string(lexer->c);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        spr_lexer_advance(lexer);
    }

    return init_spr_token(TOKEN_ID, buffer);
}

spr_token_T* spr_lexer_collect_number(spr_lexer_T* lexer)
{
    int type = TOKEN_INT;

    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    while (isdigit(lexer->c))
    {
        char* strchar = char_to_string(lexer->c);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        spr_lexer_advance(lexer);
    }

    if (lexer->c == '.')
    {
        char* strchar = char_to_string(lexer->c);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        spr_lexer_advance(lexer);

        type = TOKEN_FLOAT;

        while (isdigit(lexer->c))
        {
            char* strchar = char_to_string(lexer->c);
            buffer = realloc(buffer, strlen(buffer) + 2);
            strcat(buffer, strchar);
            free(strchar);

            spr_lexer_advance(lexer);
        }
    } 

    return init_spr_token(type, buffer);
}

spr_token_T* lexer_advance_with_token(spr_lexer_T* lexer, int type)
{
    char* value = char_to_string(lexer->c);
    spr_lexer_advance(lexer);
    return init_spr_token(type, value);
}

spr_token_T* spr_lexer_get_next_token(spr_lexer_T* lexer)
{
    while (lexer->c != '\0' && lexer->c_i < strlen(lexer->content) - 1)
    {
        while (lexer->c == ' ' || (int) lexer->c == 10)
            spr_lexer_skip_whitespace(lexer);

        if (isdigit(lexer->c))
            return spr_lexer_collect_number(lexer);

        if (isalnum(lexer->c))
            return spr_lexer_collect_id(lexer);

        switch(lexer->c)
        {
            case ';': return lexer_advance_with_token(lexer, TOKEN_SEMI); break;
            case ',': return lexer_advance_with_token(lexer, TOKEN_COMMA); break;
            default: printf("[Line %d] Unexpected %c\n", lexer->line_n, lexer->c); exit(1); break;
        }
    }

    return init_spr_token(TOKEN_EOF, "\0");
}
