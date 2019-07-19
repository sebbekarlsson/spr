#include "include/io.h"


/**
 * Reads the content of a file
 *
 * @param char* filename
 *
 * @return char*
 */
char* read_file(char* filename)
{
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = calloc (length, length);

        if (buffer)
            fread (buffer, 1, length, f);

        fclose (f);
    }

    return buffer;
}

void write_file(const char* filename, char* contents)
{
    FILE * fp;
    
    fp = fopen(filename, "w");

    fprintf(fp, contents);

    fclose (fp);
}
