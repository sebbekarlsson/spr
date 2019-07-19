#include "include/spr.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
    spr_T* spr = spr_load_from_file("shards/blink.spr");

    printf(
        "width: %12.6f, height: %12.6f, r: %12.6f, g: %12.6f, b: %12.6f, delay: %12.6f, animated: %12.6f\n",
        spr->info[0],
        spr->info[1],
        spr->info[2],
        spr->info[3],
        spr->info[4],
        spr->info[5],
        spr->info[6]
    );
    

    for (int i = 0; i < spr->frames_size; i++)
    {
        spr_AST_T* frame = spr->frames[i];

        for (int y = 0; y < frame->frame_value_pixel_rows_size; y++)
        {
            spr_AST_T* pixel_row = frame->frame_value_pixel_rows[y];

            for (int x = 0; x < (int) spr->info[0]; x++)
            {
                spr_AST_T* pixel = pixel_row->pixel_row_value[x];

                printf(
                    "r: %d, g: %d b: %d, a: %d\n",
                    pixel->pixel_value[0],
                    pixel->pixel_value[1],
                    pixel->pixel_value[2],
                    pixel->pixel_value[3]
                );
            }
        }
    }

    return 0;
}
