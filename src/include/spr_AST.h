#ifndef SPR_AST_H
#define SPR_AST_H
#include <stdlib.h>


typedef struct SPR_AST_STRUCT
{
    enum
    {
        AST_INFO
    } type;

    float info_value[7];
} spr_AST_T;

spr_AST_T* init_spr_ast(int type);
#endif
