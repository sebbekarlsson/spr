#ifndef SPR_AST_H
#define SPR_AST_H
#include <stdlib.h>


typedef struct SPR_AST_STRUCT
{
    enum
    {
        AST_SPRITE,
        AST_INFO,
        AST_FRAME,
        AST_PIXEL,
        AST_PIXEL_ROW
    } type;

    float info_value[7];
    int pixel_value[4];
    void** pixel_row_value;
    size_t pixel_row_size;

    void** frame_value_pixel_rows;
    size_t frame_value_pixel_rows_size;
} spr_AST_T;

spr_AST_T* init_spr_ast(int type);
#endif
