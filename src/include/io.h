#ifndef IO_H
#define IO_H
#include <stdio.h>
#include <stdlib.h>


char* read_file(char* filename);

void write_file(const char* filename, char* contents);
#endif
