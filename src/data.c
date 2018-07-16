#include "../include/data.h"
#include "../include/util.h"



dataset_t* dat_new ()
{
    dataset_t* data = NULL;
    data = (dataset_t*) malloc (sizeof(dataset_t));

    if (data == NULL)
    {
        ut_errmsg (
            "Could not allocate memory for the dataset.",
            __FILE__, __LINE__ - 6, 1
        );
    }

    // Initialize vectors to be NULL
    data->X = NULL;
    data->Y = NULL;

    // Some default values
    data->batch_size     = 1024;
    data->row_offset     = 0;
    data->current_batch  = 0;
    data->current_epoch  = 0;
    data->repeat_flag    = 1;
    data->reshuffle_flag = 0;
    
    return data;
}



void dat_free (dataset_t** data)
{
    vec_free(&(*data)->X);
    vec_free(&(*data)->Y);
    free(*data);
}



void dat_export (dataset_t* data, const char* fname)
{

}



dataset_t* dat_import (const char* fname)
{
    return NULL;
}



//-----------------------------------------------------------------------------
// Different types of exporting/importing datasets
//-----------------------------------------------------------------------------
dataset_t* dat_import_array (
    vec_type_t** features_arr, vec_type_t** labels_arr,
    int features_num_rows, int features_num_columns,
    int labels_num_rows,   int labels_num_columns
)
{
    return NULL;
}



void dat_import_features (const char* features_file)
{

}


void dat_import_labels (const char* labels_file)
{

}



void dat_import_features_array (vec_type_t** features_arr, int rows, int columns)
{

}



void dat_import_labels_array (vec_type_t** labels_arr, int rows, int columns)
{

}
//-----------------------------------------------------------------------------



void dat_shuffle (dataset_t* data)
{

}



void dat_repeat (dataset_t* data)
{

}



vec_t* dat_next_batch (dataset_t* data)
{
    return NULL;
}



//=============================================================================



vec_type_t** dat_get_lines_label_1 (
    utext_t* lines, int num_lines
)
{
    int i, line, output_size = 2;

    vec_type_t** vecs = NULL;
    vecs = (vec_type_t**) malloc (num_lines * sizeof(vec_type_t*));
    line = __LINE__ - 1;

    if (!vecs)
    {
        ut_errmsg(
            "Couldnot allocate memory for the vectors.",
            __FILE__, line, 1
        );
    }

    for (i = 0; i < num_lines; i++)
    {
        vecs[i] = NULL;
        vecs[i] = (vec_type_t*) malloc (output_size * sizeof(vec_type_t));
        line = __LINE__ - 1;

        if (!vecs[i])
        {
            char msgbuf[128];
            sprintf(msgbuf, "Couldnot allocate memory for vector %d.", i);
            ut_errmsg(
                msgbuf,
                __FILE__, line, 1
            );
        }
    }

    for (i = 0; i < num_lines; i++)
    {
        if (lines[i][0] == '0')
        {
            vecs[i][0] = NORM_0;
            vecs[i][1] = NORM_1;
        }
        else
        {
            vecs[i][0] = NORM_1;
            vecs[i][1] = NORM_0;
        }
    }

    return vecs;
}



vec_type_t** dat_get_lines_representation_1 (
    utext_t* lines, int num_lines, int max_chars
)
{
    int i, j, line;
    
    vec_type_t** vecs = NULL;
    vecs = (vec_type_t**) malloc (num_lines * sizeof(vec_type_t*));
    line = __LINE__ - 1;

    if (!vecs)
    {
        ut_errmsg(
            "Couldnot allocate memory for the vectors.",
            __FILE__, line, 1
        );
    }

    for (i = 0; i < num_lines; i++)
    {
        vecs[i] = NULL;
        vecs[i] = (vec_type_t*) malloc (max_chars * sizeof(vec_type_t));
        line = __LINE__ - 1;

        if (!vecs[i])
        {
            char msgbuf[128];
            sprintf(msgbuf, "Couldnot allocate memory for vector %d.", i);
            ut_errmsg(
                msgbuf,
                __FILE__, line, 1
            );
        }
    }

    // 
    for (i = 0; i < num_lines; i++)
    {
        int len = strlen((const char*) lines[i]);

        for (j = 0; j < max_chars; j++)
        {
            vec_type_t val = 0;
            if (j < len)
                val = ((vec_type_t) lines[i][j]) / NORM_UCHAR;
            vecs[i][j] = val;
        }
    }

    return vecs;
}