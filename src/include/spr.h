#ifndef SPR_H
#define SPR_H
#include "spr_AST.h"

typedef struct SPR_STRUCT
{
    float info[7];
    spr_AST_T** frames;
    size_t frames_size;
} spr_T;

spr_T* init_spr(float info[7], spr_AST_T** frames, size_t frames_size);

spr_T* spr_load_from_file(const char* filename);
#endif
