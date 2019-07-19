#include "include/spr.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
    spr_T* spr = spr_load_from_file("shards/blink.spr");

    printf(
        "width: %d, height: %d, r: %d, g: %d, b: %d, delay: %12.6f, animated: %d\n",
        spr->width,
        spr->height,
        spr->r,
        spr->g,
        spr->b,
        spr->delay,
        spr->animated
    );

    for (int i = 0; i < spr->frames_size; i++)
    {
        spr_frame_T* frame = spr->frames[i];

        for (int y = 0; y < frame->pixel_rows_size; y++)
        {
            spr_pixel_row_T* pixel_row = frame->pixel_rows[y];

            for (int x = 0; x < spr->width; x++)
            {
                spr_pixel_T* pixel = pixel_row->pixels[x];

                printf(
                    "r: %d, g: %d b: %d, a: %d\n",
                    pixel->r,
                    pixel->g,
                    pixel->b,
                    pixel->a
                );
            }
        }
    }

    spr_write_to_file(spr, "tmp.spr");

    return 0;
}
