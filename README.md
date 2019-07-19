# .spr file format
> This is the official implementation for the `.spr` file format.

## The Contents Of The .spr
> Here is an example:
```
info 3 6 255 255 255 1.6 1
frame
255,255,255,255; 255,255,255,255; 255,255,255,255
255,255,255,255; 255,255,255,255; 255,255,255,255
255,255,255,255; 255,255,255,255; 255,255,255,255
255,255,255,255; 255,255,255,255; 255,255,255,255
255,255,255,255; 255,255,255,255; 255,255,255,255
255,255,255,255; 255,255,255,255; 255,255,255,255
frame
255,255,255,255; 0,0,0,0; 255,255,255,255
255,255,255,255; 0,0,0,0; 255,255,255,255
255,255,255,255; 0,0,0,0; 255,255,255,255
255,255,255,255; 0,0,0,0; 255,255,255,255
255,255,255,255; 0,0,0,0; 255,255,255,255
255,255,255,255; 0,0,0,0; 255,255,255,255
```
> This sprite has the width of `3` pixels and the height of `6` pixels.
> The color key of this sprite is the RGB values `255`, `255`, `255` (white).
> This sprite has a delay of `1.6` seconds between every frame.
> The last number in the header is the `animated` boolean.
> This sprite has `2` frames.

### A better description
> Here is a better description of the content style:
```
info <width(int)> <height(int)> <r(int)l> <g(int)> <b(int)> <delay(float)> <animated(int)>
frame
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
frame
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
pixel[r, g, b, a]; pixel[r, g, b, a]; pixel[r, g, b, a]
```

## Reading a .spr file
> Here is an example in C on how to read a .spr file:
```C
#include <spr/spr.h>


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

// dont forget to free it
spr_free(spr);
```

## Writing a .spr file
> Here is an example in C on how to write a .spr file:
```C
#include <spr/spr.h>


spr_T* spr = init_spr(
    16, // width
    16, // height
    255, // red
    255, // green
    255, // blue
    1.6, // delay
    1, // animated
    frames, // frames
    frames_size // frames_size
)

spr_write_to_file(spr, "tmp.spr");

// dont forget to free it
spr_free(spr);
```
