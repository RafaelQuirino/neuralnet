#include "../include/data.h"
#include "../include/util.h"



dataset_t* dat_new ()
{
    dataset_t* data = NULL;
    data = (dataset_t*) malloc (sizeof(dataset_t));
    int line = __LINE__ - 1;

    if (data == NULL)
    {
        ut_errmsg (
            "Could not allocate memory for the dataset.",
            __FILE__, line, 1
        );
    }

    // Initialize vectors to be NULL
    data->X = NULL;
    data->Y = NULL;

    // Some default values
    data->size               = 0;
    data->batch_size         = 128;
    data->row_offset         = 0;
    data->current_batch      = 0;
    data->current_epoch      = 0;
    data->current_iteration  = 0;
    data->repeat_flag        = 1;
    data->reshuffle_flag     = 0;
    
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



void dat_normalize (dataset_t* data)
{
    int i, j;

    vec_t* median = vec_get_columns_sums(data->X);
    vec_mult_scalar(median, 1.0/(vec_type_t)data->X->m);

    for (i = 0; i < data->X->m; i++)
    {
        for (j = 0; j < data->X->n; j++)
        {
            vec_type_t elem = vec_get(data->X, i,j);
            vec_type_t mu   = vec_get(median, 0, j);
            elem -= mu;
            vec_set(data->X, i,j, elem);
        }
    }

    vec_t* xsquared = vec_apply_out(data->X, vec_square_op);
    vec_t* variance = vec_get_columns_sums(xsquared);
    vec_mult_scalar(variance, 1.0/(vec_type_t)data->X->m);    
    vec_free(&xsquared);

    for (i = 0; i < data->X->m; i++)
    {
        for (j = 0; j < data->X->n; j++)
        {
            vec_type_t elem  = vec_get(data->X, i,j);
            vec_type_t sigma = vec_get(variance, 0, j);
            elem /= sigma;
            vec_set(data->X, i,j, elem);
        }
    }
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
    int i, n = data->X->m;

    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
 
        // Swap arr[i] with the element at random index
        // swap(&arr[i], &arr[j]);
        vec_swap_rows(data->X, i, j);
        vec_swap_rows(data->Y, i, j);
    }
}



void dat_repeat (dataset_t* data)
{

}



minibatch_t* dat_next_minibatch (dataset_t* data)
{
    int line;

    minibatch_t* minibatch = NULL;
    minibatch = (minibatch_t*) malloc (sizeof(minibatch_t));
    line = __LINE__ - 1;

    if (!minibatch)
    {
        ut_errmsg(
            "Couldnot allocate memory for the minibatch.",
            __FILE__, line, 1
        );
    }

    minibatch->X = vec_clone_portion_circ(
        data->X, data->row_offset, data->batch_size
    );

    minibatch->Y = vec_clone_portion_circ(
        data->Y, data->row_offset, data->batch_size
    );

    minibatch->size = data->batch_size;

    data->current_batch     += 1;
    data->current_iteration += 1;
    if ((data->row_offset + data->batch_size) >= data->size)
    {
        data->current_batch = 0;
        data->current_epoch += 1;
        dat_shuffle(data);
    }
    data->row_offset = (data->row_offset + data->batch_size) % data->size;

    return minibatch;
}



void dat_free_minibatch (minibatch_t** minibatch)
{
    vec_free(&((*minibatch)->X));
    vec_free(&((*minibatch)->Y));
    free(*minibatch);
}



//=============================================================================



vec_type_t** dat_get_lines_label_1 (
    utext_t* lines, int num_lines, int output_size
)
{
    int i, line;

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

    // Normalizing each unsigned char
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



dataset_t* dat_get_dataset_from_representation_1 (
    const char* linesfile, const char* labelsfile
)
{
    int line;
    int max_chars = 160*7;//128*5;
    int output_size = 2;

    dataset_t* dataset = dat_new();

    int num_lines, num_lines_2;
    utext_t* lines  = txt_get_ulines(linesfile, &num_lines);
    // txt_print_ulines(lines,num_lines);
    // exit(0);
    utext_t* labels = txt_get_ulines(labelsfile, &num_lines_2);
    line = __LINE__ - 1;

    // printf("%d, %d\n", num_lines, num_lines_2);

    if (num_lines != num_lines_2)
    if (abs(num_lines - num_lines_2) > 1) // Hardcoded solution. Needs fixing...
    {
        ut_errmsg(
            "Text and label files doesn't match.",
            __FILE__, line, 1
        );
    }
    
    dataset->X = vec_new_arr(
        dat_get_lines_representation_1(lines, num_lines, max_chars),
        num_lines, max_chars
    );

    dataset->Y = vec_new_arr(
        dat_get_lines_label_1(labels, num_lines, output_size),
        num_lines, output_size
    );
    
    dataset->size = num_lines;

    txt_free_ulines(&lines, num_lines);
    txt_free_ulines(&labels, num_lines);

    // dat_normalize(dataset);
    // dat_shuffle(dataset);

    return dataset;
}



void dat_print_vec_lines (vec_t* data)
{
    int i, j;

    for (i = 0; i < data->m; i++)
    {
        for (j = 0; j < data->n; j++)
        {
            printf("%c", (char) vec_get(data, i, j));
        }
        printf("\n");
    }
}