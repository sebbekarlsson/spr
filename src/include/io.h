#ifndef SPR_IO_H
#define SPR_IO_H
#include <stdio.h>
#include <stdlib.h>


char* spr_read_file(char* filename);

void write_file(const char* filename, char* contents);
#endif
