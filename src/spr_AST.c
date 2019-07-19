#include "include/spr_AST.h"


spr_AST_T* init_spr_ast(int type)
{
    spr_AST_T* ast = calloc(1, sizeof(struct SPR_AST_STRUCT));

    ast->type = type;

    return ast;
}
