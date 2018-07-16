#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/io.h"
#include "../include/util.h"
#include "../include/text.h"
#include "../include/linalg.h"



int main (int argc, char** argv)
{
    // Arguments ------------------------------------------
    if (argc != 2)
    {
        fprintf(stdout, "usage: bin path_to_txt_file\n");
        exit(1);
    }
    
    char* file = argv[1];
    //-----------------------------------------------------



    vec_t* A = vec_new(1,5);
    vec_t* B = vec_new(5,4);
    vec_set_all(A,1);
    vec_set_all(B,3);
    vec_print(A);
    vec_print(B);

    vec_t* AB = vec_get_dot(A,B);
    vec_print(AB);

    vec_set_row(B, 0, 1);
    vec_set_row(B, 1, 2);
    vec_set_row(B, 2, 3);
    vec_set_row(B, 3, 4);
    vec_set_row(B, 4, 5);
    vec_print(B);

    vec_t* C = vec_clone_portion(B, 1, 3);
    vec_print(C);



    int num_lines;
    utext_t* ulines = txt_get_ulines(file, &num_lines);
    txt_print_ulines(ulines, num_lines);

    //



    return 0;
}