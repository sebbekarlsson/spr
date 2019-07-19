#ifndef SPR_TOKEN_H
#define SPR_TOKEN_H
typedef struct SPR_TOKEN_STRUCT
{
    enum {
        TOKEN_FLOAT,
        TOKEN_INT,
        TOKEN_SEMI,
        TOKEN_ID,
        TOKEN_COMMA,
        TOKEN_EOF
    } type;

    char* value;
} spr_token_T;

spr_token_T* init_spr_token(int type, char* value);
#endif
