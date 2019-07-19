#include "include/spr_token.h"
#include <stdlib.h>


spr_token_T* init_spr_token(int type, char* value)
{
    spr_token_T* token = calloc(1, sizeof(struct SPR_TOKEN_STRUCT));
    token->type = type;
    token->value = value;

    return token;
}
