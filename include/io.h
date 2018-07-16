#ifdef __cplusplus
extern "C" {
#endif
#ifndef _IO_H_
#define _IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char*  io_read_text (const char* file_path);
char** io_cut_lines (char* text, int* num_lines);
char** io_get_lines (const char* file_path, int* num_lines);



#endif // _IO_H_
#ifdef __cplusplus
}
#endif