#include "../include/linalg.h"
#include "../include/util.h"



//=========================================
// COMMON, MAINTAINANCE, USEFUL FUNCTIONS
//=========================================

vec_t* vec_new (int m, int n)
{
    int line = __LINE__ - 2;

    if (m <= 0 || n <= 0) 
    {
        ut_errmsg (
            "Dimensions cannot be negative.",
            __FILE__, line, 1
        );
    }

    vec_t* vec = 0;
    vec = (vec_t*) malloc (sizeof(vec_t));
    line = __LINE__ - 1;
    if (!vec)
    {
        ut_errmsg (
            "Cannot allocate memory for the vec_t struct.",
            __FILE__, line, 1
        );
    }

    vec->vec = 0;
    vec->vec = (vec_type_t*) malloc((m*n) * sizeof(vec_type_t));
    line = __LINE__ - 1;
    if (!vec)
    {
        ut_errmsg (
            "Cannot allocate memory for the vec_t array.",
            __FILE__, line, 1
        );
    }

    memset(vec->vec, 0, m*n*sizeof(vec_type_t));
    vec->m = m; vec->rows = m;
    vec->n = n; vec->columns = n;
    vec->size = m*n; vec->length = m*n;

    return vec;
}

vec_t* vec_new_arr (vec_type_t** arr, int m, int n)
{
    int line = __LINE__ - 2;

    if (m <= 0 || n <= 0) 
    {
        ut_errmsg (
            "Dimensions cannot be negative.",
            __FILE__, line, 0
        );
        return NULL;
    }

    int i, j;

    vec_t* vec = 0;
    vec = (vec_t*) malloc(sizeof(vec_t));
    line = __LINE__ - 1;
    if (!vec)
    {
        ut_errmsg (
            "Cannot allocate memory for the vec_t struct.",
            __FILE__, line, 1
        );
    }

    vec->vec = 0;
    vec->vec = (vec_type_t*) malloc((m*n) * sizeof(vec_type_t));
    line = __LINE__ - 1;
    if (!vec)
    {
        ut_errmsg (
            "Cannot allocate memory for the vec_t struct.",
            __FILE__, line, 1
        );
    }
    
    vec->m = m; vec->rows = m;
    vec->n = n; vec->columns = n;
    vec->size = m*n;

    for (i = 0; i < m; i++) 
        for (j = 0; j < n; j++) 
            vec_set(vec, i, j, arr[i][j]);

    return vec;
}

vec_type_t** vec_to_arr (vec_t* vec)
{
    int i, j, line = __LINE__ - 2;
    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    vec_type_t** arr = 0;
    arr = (vec_type_t**) malloc(vec->m * sizeof(vec_type_t*));
    line = __LINE__ - 1;
    if (!arr)
    {
        ut_errmsg (
            "Cannnot allocate memory for the array.",
            __FILE__, line, 1
        );
    }

    for (i = 0; i < vec->m; i++)
        arr[i] = (vec_type_t*) malloc(vec->n * sizeof(vec_type_t));

    for (i = 0; i < vec->m; i++)
        for (j = 0; j < vec->n; j++)
            arr[i][j] = vec_get(vec,i,j);

    return arr;
}

void vec_free (vec_t** vecptr)
{
    if ((*vecptr) == NULL)
        return;

    free((*vecptr)->vec);
    free((*vecptr));
    *vecptr = NULL;
}

vec_type_t vec_get (vec_t* vec, int i, int j)
{
    int line = __LINE__ - 2;
    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    return vec->vec[i*vec->n + j];
}

void vec_set (vec_t* vec, int i, int j, vec_type_t k)
{
    int line = __LINE__ - 2;
    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    vec->vec[i*vec->n + j] = k;
}

void vec_set_all (vec_t* vec, vec_type_t k)
{
    int line = __LINE__ - 2;
    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    
    for (i = 0; i < vec->m; i++)
        for (j = 0; j < vec->n; j++)
            vec_set(vec, i, j, k);
}

void vec_set_all_func (vec_t* vec, double(*funcptr)())
{
    int line = __LINE__ - 2;
    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = 0; i < vec->m; i++)
        for (j = 0; j < vec->n; j++)
            vec_set(vec, i, j, (vec_type_t) funcptr());
}

//TODO
void vec_get_row (vec_t* vec, int row)
{

}

void vec_set_row (vec_t* vec, int row, vec_type_t k)
{
    int line = __LINE__ - 2;

    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    if (row < 0 || row >= vec->m)
    {
        ut_errmsg (
            "Invalid row.",
            __FILE__, line, 1
        );
    }

    int j;
    for (j = 0; j < vec->n; j++)
        vec_set(vec, row, j, k);
}

//TODO
void vec_get_column (vec_t* vec, int column)
{

}

//TODO
void vec_set_column (vec_t* vec, int column, vec_type_t k)
{

}

//TODO
void vec_push_row (vec_t* vec, vec_type_t* arr, int arrsize)
{

}

//TODO
void vec_pop_row (vec_t* vec, vec_type_t* arr)
{

}

//TODO
void vec_push_column (vec_t* vec, vec_type_t* arr, int arrsize)
{

}

//TODO
void vec_pop_column (vec_t* vec, vec_type_t* arr)
{

}

void vec_copy (vec_t* dest, vec_t* src)
{
    int line = __LINE__ - 2;

    if (dest == NULL || src == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    if (dest->m != src->m || dest->n != src->n) 
    {
        ut_errmsg (
            "Dimensions must be equal.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = 0; i < dest->m; i++) 
    {
        for (j = 0; j < dest->n; j++) 
        {
            vec_type_t elem = vec_get(src, i, j);
            vec_set(dest, i, j, elem);
        }
    }
}

void vec_copy_portion (vec_t* dest, vec_t* src, int offset, int size)
{
    int line = __LINE__ - 2;

    if (dest == NULL || src == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    if (dest->m != src->m || dest->n != src->n) 
    {
        ut_errmsg (
            "Dimensions must be equal.",
            __FILE__, line, 1
        );
    }

    if (offset < 0 || offset >= dest->m || size < 0 || offset+size-1 >= dest->m)
    {
        ut_errmsg (
            "Either offset and/or size are wrong.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = offset; i < offset+size; i++) 
    {
        for (j = 0; j < src->n; j++) 
        {
            vec_type_t elem = vec_get(src, i, j);
            vec_set(dest, i, j, elem);
        }
    }
}

vec_t* vec_clone (vec_t* src)
{
    int line = __LINE__ - 2;
    if (src == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;

    vec_t* dest = vec_new(src->m, src->n);

    for (i = 0; i < dest->m; i++) 
    {
        for (j = 0; j < dest->n; j++) 
        {
            vec_type_t elem = vec_get(src, i, j);
            vec_set(dest, i, j, elem);
        }
    }

    return dest;
}

vec_t* vec_clone_portion (vec_t* src, int offset, int size)
{
    int line = __LINE__ - 2;
    
    if (src == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    if (offset < 0 || offset >= src->m || size < 0 || offset+size-1 >= src->m)
    {
        ut_errmsg (
            "Either offset and/or size are wrong.",
            __FILE__, line, 1
        );
    }

    int i, j, curr_row = 0;

    vec_t* dest = vec_new(size, src->n);

    for (i = offset; i < offset+size; i++) 
    {
        for (j = 0; j < src->n; j++) 
        {
            vec_type_t elem = vec_get(src, i, j);
            vec_set(dest, curr_row, j, elem);
        }

        curr_row += 1;
    }

    return dest;
}

vec_t* vec_clone_portion_circ (vec_t* src, int offset, int size)
{
    int line = __LINE__ - 2;
    
    if (src == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    if (offset < 0 || size < 0)
    {
        ut_errmsg (
            "Either offset and/or size are wrong.",
            __FILE__, line, 1
        );
    }

    int i, j, curr_row = 0;
    int real_offset = offset >= src->m ? offset % src->m : offset;
    int curr_offset = real_offset;

    vec_t* dest = vec_new(size, src->n);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < src->n; j++)
        {
            vec_type_t elem = vec_get(src, curr_offset, j);
            vec_set(dest, curr_row, j, elem);
        }

        curr_offset = (curr_offset + 1) % src->m;
        curr_row += 1;
    }

    return dest;
}

void vec_print (vec_t* vec)
{
    int line = __LINE__ - 2;
    if (vec == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = 0; i < vec->m; i++) 
    {
        for (j = 0; j < vec->n; j++) 
        {
            printf("%.3g\t\t", vec->vec[i*vec->n + j]);
        }
        printf("\n");
    }

    printf("\n");
}

void vec_print_portion (vec_t* vec, int portion, int side)
{
    int line = __LINE__ - 2;
    if (vec == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;

    int m = portion < vec->m ? portion : vec->m;
    int n = portion < vec->n ? portion : vec->n;

    if (side == 0) 
    {
        for (i = 0; i < m; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                printf("%.4f\t\t", vec->vec[i*vec->n + j]);
            }
            printf("\n");
        }
    }
    else 
    {
        for (i = m; i > 0; i--)
        {
            for (j = n; j > 0; j--)
            {
                printf("%.4f\t\t", vec_get(vec,vec->m-i,vec->n-j));
            }
            printf("\n");
        }
    }
}

void vec_print_bitmap (vec_t* bitmap, int row,  int rows, int columns)
{
    int line = __LINE__ - 2;
    if (bitmap == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    printf("+");
    for (j = 0; j < columns; j++)
        printf("-");
    printf("+\n");
    for (i = 0; i < rows; i++) 
    {
        printf("|");
        for (j = 0; j < columns; j++)
        {
            vec_type_t el = vec_get(bitmap,row,i*columns+j);
            if (el >= 0 && el < 0.2)
                printf(" ");
            else if (el >= 0.2 && el < 0.4)
                printf(".");
            else if (el >= 0.4 && el < 0.6)
                printf("'");
            else if  (el >= 0.6 && el < 0.8)
                printf("*");
            else
                printf("#");
        }
        printf("|\n");
    }
    printf("+");
    for (j = 0; j < columns; j++)
        printf("-");
    printf("+\n");
}



//=====================================
// REDUCTIONS
//=====================================

vec_type_t vec_inner_sum (vec_t* A)
{
    int line = __LINE__ - 2;
    if (A == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    vec_type_t sum = 0;
    
    for (i = 0; i < A->m; i++) 
    {
        for (j = 0; j < A->n; j++) 
        {
            sum += vec_get(A, i, j);
        }
    }

    return sum;
};

vec_t* vec_get_rows_sums (vec_t* A)
{
    int line = __LINE__ - 2;
    if (A == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    vec_type_t sum;

    vec_t* newvec = vec_new(A->m, 1);

    for (i = 0; i < A->m; i++) 
    {
        sum = 0;
        for (j = 0; j < A->n; j++)
            sum += vec_get(A, i, j);
        vec_set(newvec, i, 0, sum);
    }

    return newvec;
}

vec_t* vec_get_columns_sums (vec_t* A)
{
    int line = __LINE__ - 2;
    if (A == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    vec_type_t sum;

    vec_t* newvec = vec_new(1, A->n);

    for (j = 0; j < A->n; j++) 
    {
        sum = 0;
        for (i = 0; i < A->m; i++)
            sum += vec_get(A, i, j);
        vec_set(newvec, 0, j, sum);
    }

    return newvec;
}



//=====================================
// SELF OPERATIONS
//=====================================

vec_t* vec_transposed (vec_t* A)
{
    int line = __LINE__ - 2;
    if (A == NULL) 
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    vec_t* At = vec_new(A->n, A->m);
    for (i = 0; i < A->m; i++)
        for (j = 0; j < A->n; j++)
            vec_set(At, j, i, vec_get(A, i, j));

    return At;
}



//=========================================================
// ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION,
// EXPONENTIATION, NTH ROOT AND LOG BASE N...
//=========================================================

void vec_add_scalar (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(A,i,j, vec_get(A,i,j) + scalar);
}

//TODO
vec_t* vec_get_scalar_sum  (vec_t* A, vec_type_t scalar)
{
    return NULL;
}

void vec_sub_scalar (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(A,i,j, vec_get(A,i,j) - scalar);
}

//TODO
vec_t* vec_get_scalar_diff (vec_t* A, vec_type_t scalar)
{
    return NULL;
}

void vec_mult_scalar (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    int i, j;
    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(A,i,j, vec_get(A,i,j) * scalar);
}

vec_t* vec_get_scalar_prod (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    int i, j;
    vec_t* newvec = vec_new(A->m,A->n);

    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(newvec,i,j, vec_get(A,i,j) * scalar);

    return newvec;
}

void vec_div_scalar (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    int i, j;
    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(A,i,j, vec_get(A,i,j) / scalar);
}

//TODO
vec_t* vec_get_scalar_div  (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    return NULL;
}

void vec_exp_scalar (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    int i, j;
    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(A,i,j, pow(vec_get(A,i,j), scalar));
}

//TODO
vec_t* vec_get_scalar_exp  (vec_t* A, vec_type_t scalar)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    return NULL;
}

//TODO
void vec_nth_root (vec_t* A, vec_type_t scalar_root)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }

}

//TODO
void vec_log_base_n (vec_t* A, vec_type_t scalar_base)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
}

void vec_sqrt (vec_t* A)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Pointer vec_t* is NULL.",
            __FILE__, line, 1
        );
    }
    
    int i, j;
    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(A,i,j, sqrt(vec_get(A,i,j)));
}



//======================================
// DOT PRODUCT (MATRIX MULTIPLICATION)
//======================================

void vec_dot (vec_t* A, vec_t* B, vec_t* AB)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Matrices are NULL.",
            __FILE__, line, 1
        );
    }

    if (A->n != B->m) 
    {
        ut_errmsg (
            "Matrices are not compatible for dot product.",
            __FILE__, line, 1
        );
    }

    int i, j, k;

    // // Naive algorithm
    // //-----------------
    // for (i = 0; i < A->m; i++) 
    // {
    //     for (j = 0; j < B->n; j++) 
    //     {
    //         double sum = 0.0;
    //         for (k = 0; k < A->n; k++) 
    //         {
    //             sum += vec_get(A,i,k) * vec_get(B,k,j);
    //         }
    //         vec_set(AB,i,j,sum);
    //     }
    // }

    int M = A->m,
        N = B->n,
        K = A->n;

    int alpha = 1,
        beta  = 1,
        lda   = K,
        ldb   = N,
        ldc   = N;

    if (VEC_TYPE == VEC_FLOAT)
    {
        cblas_sgemm ( // Specific for single precision
            CblasRowMajor, CblasNoTrans, CblasNoTrans,
            M, N, K,
            (float) alpha, 
            (float*) A->vec, lda,
            (float*) B->vec, ldb, 
            (float) beta,
            (float*) AB->vec, ldc 
        );
    }
    else if (VEC_TYPE == VEC_DOUBLE)
    {
        cblas_dgemm ( // Specific for double precision
            CblasRowMajor, CblasNoTrans, CblasNoTrans,
            M, N, K,
            (double) alpha, 
            (double*) A->vec, lda,
            (double*) B->vec, ldb, 
            (double) beta,
            (double*) AB->vec, ldc 
        );
    }

    // "Correcting" Not A Number situations
    for (i = 0; i < AB->m*AB->n; i++)
        if (isnan(AB->vec[i]))
            AB->vec[i] = (vec_type_t) 0;
}

void vec_mult (vec_t* A, vec_t* B, vec_t* AB)
{
    vec_dot(A,B,AB);
}

vec_t* vec_get_dot (vec_t* A, vec_t* B)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Matrices are NULL.",
            __FILE__, line, 1
        );
    }

    if (A->n != B->m) 
    {
        ut_errmsg (
            "Matrices are not compatible for dot product.",
            __FILE__, line, 1
        );
    }

    int i, j, k;

    vec_t* AB = vec_new(A->m, B->n);
    vec_set_all(AB, 0.0);

    // // Naive algorithm
    // //-----------------
    // for (i = 0; i < A->m; i++) 
    // {
    //     for (j = 0; j < B->n; j++) 
    //     {
    //         double sum = 0.0;
    //         for (k = 0; k < A->n; k++) 
    //         {
    //             double product = vec_get(A,i,k) * vec_get(B,k,j);
    //             if (isnan(sum + product))
    //                 sum += 0.0;
    //             else
    //                 sum += product;
    //         }
    //         vec_set(AB,i,j,sum);
    //     }
    // }
    
    int M = A->m,
        N = B->n,
        K = A->n;

    int alpha = 1,
        beta  = 1,
        lda   = K,
        ldb   = N,
        ldc   = N;

    if (VEC_TYPE == VEC_FLOAT)
    {
        cblas_sgemm ( // Specific for single precision
            CblasRowMajor, CblasNoTrans, CblasNoTrans,
            M, N, K,
            (float) alpha, 
            (float*) A->vec, lda,
            (float*) B->vec, ldb, 
            (float) beta,
            (float*) AB->vec, ldc 
        );
    }
    else if (VEC_TYPE == VEC_DOUBLE)
    {
        cblas_dgemm ( // Specific for double precision
            CblasRowMajor, CblasNoTrans, CblasNoTrans,
            M, N, K,
            (double) alpha, 
            (double*) A->vec, lda,
            (double*) B->vec, ldb, 
            (double) beta,
            (double*) AB->vec, ldc 
        );
    }
    
    // "Correcting" Not A Number situations
    for (i = 0; i < AB->m*AB->n; i++)
        if (isnan(AB->vec[i]))
            AB->vec[i] = (vec_type_t) 0.0;

    return AB;
}

vec_t* vec_get_mult (vec_t* A, vec_t* B)
{
    return vec_get_dot(A,B);
}



//======================================
// ELEMENT-WISE MATRIX OPERATIONS
//======================================

void vec_add (vec_t* A, vec_t* B, vec_t* AplusB)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(AplusB,i,j, vec_get(A,i,j) + vec_get(B,i,j));
}

vec_t* vec_get_sum (vec_t* A, vec_t* B)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;

    vec_t* AplusB = vec_new(A->m,A->n);

    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(AplusB,i,j, vec_get(A,i,j) + vec_get(B,i,j));

    return AplusB;
}

void vec_sub (vec_t* A, vec_t* B, vec_t* AminusB)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(AminusB,i,j, vec_get(A,i,j) - vec_get(B,i,j));
}

vec_t* vec_get_diff (vec_t* A, vec_t* B)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;

    vec_t* AminusB = vec_new(A->m,A->n);

    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(AminusB,i,j, vec_get(A,i,j) - vec_get(B,i,j));

    return AminusB;
}

void vec_mult_elwise (vec_t* A, vec_t* B, vec_t* AB)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;

    for (i = 0; i < A->m; i++) 
        for (j = 0; j < A->n; j++) 
            vec_set(AB,i,j, vec_get(A,i,j) * vec_get(B,i,j));
}

vec_t* vec_get_mult_elwise (vec_t* A, vec_t* B)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j, k;

    vec_t* AB = vec_new(A->m,A->n);

    for (i = 0; i < A->m; i++) 
    {
        for (j = 0; j < A->n; j++) 
        {
            vec_type_t bij = vec_get(B,i,j);
            vec_set(AB,i,j, vec_get(A,i,j) * bij);
        }
    }

    return AB;
}

void vec_div_elwise (vec_t* A, vec_t* B, vec_t* AdivB)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j, k;

    for (i = 0; i < A->m; i++) 
    {
        for (j = 0; j < A->n; j++) 
        {
            vec_type_t bij = vec_get(B,i,j);
            vec_type_t den = bij == 0.0 ? 0.00001 : bij; // Dont divide by 0
            vec_set(AdivB,i,j, vec_get(A,i,j) / vec_get(B,i,j));
        }
    }
}

vec_t* vec_get_div_elwise (vec_t* A, vec_t* B)
{
    int line = __LINE__ - 2;

    if (A == NULL || B == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (A->m != B->m || A->n != B->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;

    vec_t* AdivB = vec_new(A->m, A->n);

    for (i = 0; i < A->m; i++) 
    {
        for (j = 0; j < A->n; j++) 
        {
            vec_type_t bij = vec_get(B,i,j);
            vec_type_t den = bij == 0.0 ? 0.00001 : bij; // Dont divide by 0
            vec_set(AdivB, i, j, vec_get(A,i,j) / den);
        }
    }

    return AdivB;
}

void vec_sum_row (vec_t* A, int row, vec_type_t* arr)
{
    int line = __LINE__ - 2;
    if (row < 0 || row >= A->m) 
    {
        ut_errmsg (
            "Invalid row.",
            __FILE__, line, 1
        );
    }

    int j;

    for (j = 0; j < A->n; j++)
        vec_set(A,row,j, vec_get(A,row,j) + arr[j]);
}



//=========================================================
// APPLY FUNCTION ELEMENT-WISE
//=========================================================

void vec_apply (vec_t* A, vec_elwise_op op)
{
    int line = __LINE__ - 2;
    if (A== NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    for (i = 0; i < A->m; i++) 
    {
        for (j = 0; j < A->n; j++) 
        {
            vec_type_t elem = vec_get(A,i,j);
            vec_type_t result = op(elem);
            vec_set(A,i,j,result);
        }
    }
}

void vec_apply_to (vec_t* dst, vec_t* src, vec_elwise_op op)
{
    int line = __LINE__ - 2;

    if (dst == NULL || src == NULL)
    {
        ut_errmsg (
            "Some matrice is NULL.",
            __FILE__, line, 1
        );
    }

    if (dst->m != src->m || dst->n != src->n) 
    {
        ut_errmsg (
            "Matrices are not of the same order.",
            __FILE__, line, 1
        );
    }

    int i, j;
    for (i = 0; i < src->m; i++) 
    {
        for (j = 0; j < src->n; j++) 
        {
            vec_type_t elem = vec_get(src,i,j);
            vec_type_t result = op(elem);
            vec_set(dst,i,j,result);
        }
    }
}

vec_t* vec_apply_out (vec_t* A, vec_elwise_op op)
{
    int line = __LINE__ - 2;
    if (A == NULL)
    {
        ut_errmsg (
            "Matrice is NULL.",
            __FILE__, line, 1
        );
    }

    int i, j;
    vec_t* out = vec_new(A->m,A->n);

    for (i = 0; i < A->m; i++) 
    {
        for (j = 0; j < A->n; j++) 
        {
            vec_type_t elem = vec_get(A,i,j);
            vec_type_t result = op(elem);
            vec_set(out,i,j,result);
        }
    }

    return out;
}

vec_type_t vec_square_op (vec_type_t elem)
{
    return elem * elem;
}

vec_type_t vec_sqrt_op (vec_type_t elem)
{
    return sqrt(elem);
}

vec_type_t vec_log_op (vec_type_t elem)
{
    return log(elem);
}