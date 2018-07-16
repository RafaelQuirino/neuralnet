#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/io.h"
#include "../include/util.h"
#include "../include/text.h"
#include "../include/data.h"
#include "../include/linalg.h"



int main (int argc, char** argv)
{
    // Arguments ------------------------------------------
    if (argc != 3)
    {
        fprintf(stdout, "usage: bin path_to_txt_file ");
        fprintf(stdout, "path_to_labels_file\n");
        exit(1);
    }
    
    char* file   = argv[1];
    char* labels = argv[2];
    //-----------------------------------------------------



    // Variables
    int i, j;



    // Testing linalg.h
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



    // Testing text.h
    int num_lines, num_lines_2;
    utext_t* ulines  = txt_get_ulines(file, &num_lines);
    utext_t* ulabels = txt_get_ulines(labels, &num_lines_2);

    // Testing data.h
    vec_type_t** line_vecs   = dat_get_lines_representation_1(ulines, num_lines, 128);
    vec_type_t** line_labels = dat_get_lines_label_1(ulabels, num_lines);

    for (i = 0; i < num_lines; i++) {
        for (j = 0; j < 2; j++)
            printf("[%g]", line_labels[i][j]);
        printf(": ");
        for (j = 0; j < 12; j++)
            printf("[%g]", line_vecs[i][j]);
        printf("\n\n");
    }


    return 0;
}