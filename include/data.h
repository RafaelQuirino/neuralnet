#ifdef __cplusplus
extern "C" {
#endif
#ifndef _DATA_H_
#define _DATA_H_

#include <limits.h>
#include "../include/text.h"
#include "../include/linalg.h"



static const vec_type_t NORM_0     = (vec_type_t) 0.0; //0.000001;
static const vec_type_t NORM_1     = (vec_type_t) 1.0; //0.999999;
static const vec_type_t NORM_UCHAR = (vec_type_t) UCHAR_MAX;



typedef struct
{
    vec_t *X; // features
    vec_t *Y; // labels

    int size;
    int batch_size;
    int row_offset;
    int current_batch;
    int current_epoch;

    int repeat_flag;
    int reshuffle_flag;

} dataset_t;



// Abstract data interface ----------------------------------------------------
dataset_t* dat_new    ();
void       dat_free   (dataset_t** data);
void       dat_export (dataset_t* data, const char* fname);
dataset_t* dat_import (const char* fname);

dataset_t* dat_import_array (
    vec_type_t** features_arr, vec_type_t** labels_arr,
    int features_num_rows, int features_num_columns,
    int labels_num_rows,   int labels_num_columns
);

void dat_import_features       (const char* features_file);
void dat_import_labels         (const char* labels_file);
void dat_import_features_array (vec_type_t** features_arr, int rows, int columns);
void dat_import_labels_array   (vec_type_t** labels_arr, int rows, int columns);

void   dat_shuffle    (dataset_t* data);
void   dat_repeat     (dataset_t* data);
vec_t* dat_next_batch (dataset_t* data);
//-----------------------------------------------------------------------------

// Specific data representations interface ------------------------------------
vec_type_t** dat_get_lines_label_1 (
    utext_t* lines, int num_lines, int output_size
);

vec_type_t** dat_get_lines_representation_1 (
    utext_t* lines, int num_lines, int max_chars
);

dataset_t* dat_get_dataset_from_representation_1(
    const char* linesfile, const char* labelsfile
);
//-----------------------------------------------------------------------------



#endif // _DATA_H_
#ifdef __cplusplus
}
#endif
