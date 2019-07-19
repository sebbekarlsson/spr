#include "include/spr.h"
#include "include/io.h"
#include "include/spr_lexer.h"
#include "include/spr_parser.h"


spr_T* init_spr(float info[7], spr_AST_T** frames, size_t frames_size)
{
    spr_T* spr = calloc(1, sizeof(struct SPR_STRUCT));
    spr->info[0] = info[0];
    spr->info[1] = info[1];
    spr->info[2] = info[2];
    spr->info[3] = info[3];
    spr->info[4] = info[4];
    spr->info[5] = info[5];
    spr->info[6] = info[6];

    spr->frames = frames;
    spr->frames_size = frames_size;

    return spr;
}

spr_T* spr_load_from_file(const char* filename)
{
    char* contents = read_file((char*) filename);

    spr_lexer_T* lexer = init_spr_lexer(contents);
    spr_parser_T* parser = init_spr_parser(lexer);

    spr_T* spr = spr_parser_parse(parser);
    
    free(contents);

    return spr;
}
