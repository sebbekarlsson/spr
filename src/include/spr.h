#ifndef SPR_H
#define SPR_H
#include "spr_AST.h"

typedef struct SPR_PIXEL_STRUCT
{
    int r;
    int g;
    int b;
    int a;
} spr_pixel_T;

spr_pixel_T* init_spr_pixel(int r, int g, int b);

typedef struct SPR_PIXEL_ROW_STRUCT
{
    spr_pixel_T** pixels;
    size_t pixels_size;
} spr_pixel_row_T;

typedef struct SPR_FRAME_STRUCT
{
    spr_pixel_row_T** pixel_rows;
    size_t pixel_rows_size;
} spr_frame_T;

typedef struct SPR_STRUCT
{
    int width;
    int height;
    int r;
    int g;
    int b;
    float delay;
    unsigned int animated;
    spr_frame_T** frames;
    size_t frames_size;
} spr_T;

spr_T* init_spr(
    int width,
    int height,
    int r,
    int g,
    int b,
    float delay,
    unsigned int animated,
    spr_frame_T** frames,
    size_t frames_size
);

spr_T* spr_load_from_file(const char* filename);

void spr_write_to_file(spr_T* spr, const char* filename);
#endif
