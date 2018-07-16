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



    dataset_t* dataset = dat_get_dataset_from_representation_1(file, labels);

    for (i = 0; i < dataset->size; i++) {
        for (j = 0; j < dataset->Y->columns; j++)
            printf("[%g]", vec_get(dataset->Y, i, j));
        printf(": ");
        for (j = 0; j < 12; j++)
        // for (j = 127; j >= 115; j--)
            printf("[%g]", vec_get(dataset->X, i, j));
        printf("\n\n");
    }


    return 0;
}