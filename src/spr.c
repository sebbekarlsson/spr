#include "include/spr.h"
#include "include/io.h"
#include "include/spr_lexer.h"
#include "include/spr_parser.h"
#include <string.h>


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
)
{
    spr_T* spr = calloc(1, sizeof(struct SPR_STRUCT));
    spr->width = width;
    spr->height = height;
    spr->r = r;
    spr->g = g;
    spr->b = b;
    spr->delay = delay;
    spr->animated = animated;
    spr->frames = frames;
    spr->frames_size = frames_size;

    return spr;
}

spr_pixel_T* init_spr_pixel(int r, int g, int b, int a)
{
    spr_pixel_T* pixel = calloc(1, sizeof(struct SPR_PIXEL_STRUCT));
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
    pixel->a = a;

    return pixel;
}

void spr_pixel_free(spr_pixel_T* pixel)
{
    free(pixel);
}

void spr_pixel_row_free(spr_pixel_row_T* pixel_row)
{
    for (int i = 0; i < pixel_row->pixels_size; i++)
        spr_pixel_free(pixel_row->pixels[i]);

    free(pixel_row->pixels);
    free(pixel_row);
}

void spr_frame_free(spr_frame_T* frame)
{
    for (int i = 0; i < frame->pixel_rows_size; i++)
        spr_pixel_row_free(frame->pixel_rows[i]);

    free(frame->pixel_rows);
    free(frame);
}

void spr_free(spr_T* spr)
{
    for (int i = 0; i < spr->frames_size; i++)
        spr_frame_free(spr->frames[i]);

    free(spr->frames);
    free(spr);
}

spr_frame_T* spr_init_frame_from_data(unsigned char* data, int width, int height)
{
    spr_frame_T* frame = calloc(1, sizeof(struct SPR_FRAME_STRUCT));
    frame->pixel_rows_size = 0;
    frame->pixel_rows = (void*) 0;

    for (int y = 0; y < height; y++)
    {
        spr_pixel_row_T* pixel_row = calloc(1, sizeof(struct SPR_PIXEL_ROW_STRUCT));
        pixel_row->pixels_size = 0;
        pixel_row->pixels = (void*) 0;

        for (int x = 0; x < width; x++)
        { 
            unsigned int channels = 4;
            unsigned char* pixel = data + (y * width + x) * channels;
            unsigned char r = pixel[0];
            unsigned char g = pixel[1];
            unsigned char b = pixel[2];
            unsigned char a = pixel[3];

            pixel_row->pixels_size += 1;
            pixel_row->pixels = realloc(pixel_row->pixels, pixel_row->pixels_size * sizeof(struct SPR_PIXEL_STRUCT));
            pixel_row->pixels[pixel_row->pixels_size-1] = init_spr_pixel(r, g, b, a);
        }

        frame->pixel_rows_size += 1;
        frame->pixel_rows = realloc(frame->pixel_rows, frame->pixel_rows_size * sizeof(struct SPR_PIXEL_ROW_STRUCT*));
        frame->pixel_rows[frame->pixel_rows_size-1] = pixel_row;
    }

    return frame;
}

spr_T* spr_load_from_file(const char* filename)
{
    if (!spr_file_exists(filename))
        return (void*) 0;

    char* contents = spr_read_file((char*) filename);
    
    if (contents == 0 || contents == (void*)0)
        return (void*) 0;

    spr_lexer_T* lexer = init_spr_lexer(contents);
    spr_parser_T* parser = init_spr_parser(lexer);

    spr_T* spr = spr_parser_parse(parser);
    
    free(contents);
    free(lexer);
    free(parser);

    return spr;
}

void spr_write_to_file(spr_T* spr, const char* filename)
{
    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    // width  height      r  g   b     delay  animated
    // 3       6         255 255 255   1.6    1
    char* info_template = "info %d %d %d %d %d %12.6f %d\n";
    char* info = calloc(300, sizeof(char));
    sprintf(info, info_template, spr->width, spr->height, spr->r, spr->g, spr->b, spr->delay, spr->animated);

    buffer = realloc(buffer, (strlen(buffer) + strlen(info) + 1) * sizeof(char));
    strcat(buffer, info);
    free(info);

    for (int i = 0; i < spr->frames_size; i++)
    {
        spr_frame_T* frame = spr->frames[i];

        buffer = realloc(buffer, (strlen(buffer) + strlen("frame\n") + 1) * sizeof(char));
        strcat(buffer, "frame\n");

        for (int y = 0; y < frame->pixel_rows_size; y++)
        {
            spr_pixel_row_T* pixel_row = frame->pixel_rows[y];

            char* pixel_row_str = calloc(1, sizeof(char));
            pixel_row_str[0] = '\0';

            for (int x = 0; x < spr->width; x++)
            {
                spr_pixel_T* pixel = pixel_row->pixels[x];
                // 255,255,255,255; 255,255,255,255; 255,255,255,255
                char* pixel_template = "%d,%d,%d,%d";
                char* pixel_str = calloc(300, sizeof(char));
                sprintf(pixel_str, pixel_template, pixel->r, pixel->g, pixel->b, pixel->a);

                if (x < spr->width - 1)
                {
                    pixel_str = realloc(pixel_str, (strlen(pixel_str) + 3) * sizeof(char));
                    strcat(pixel_str, "; ");
                }

                pixel_row_str = realloc(pixel_row_str, (strlen(pixel_row_str) + strlen(pixel_str) + 2) * sizeof(char));
                strcat(pixel_row_str, pixel_str);
                free(pixel_str);
            }

            pixel_row_str = realloc(pixel_row_str, (strlen(pixel_row_str) + 2) * sizeof(char));
            strcat(pixel_row_str, "\n");

            buffer = realloc(buffer, (strlen(buffer) + strlen(pixel_row_str) + 1) * sizeof(char));
            strcat(buffer, pixel_row_str);

            free(pixel_row_str);
        }
    }

    write_file(filename, buffer);

    free(buffer);
}
