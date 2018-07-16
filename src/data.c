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



dataset_t* dat_import (const char* features, const char* labels)
{
    return NULL;
}



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



vec_type_t** dat_get_lines_representation_1 (
    utext_t* lines, int num_lines, int limit
)
{

    return NULL;
}