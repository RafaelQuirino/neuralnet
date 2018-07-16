#ifdef __cplusplus
extern "C" {
#endif
#ifndef _LINALG_H_
#define _LINALG_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cblas.h>
//#include <clBLAS.h>



typedef double vec_type_t;

typedef struct 
{
    vec_type_t *vec;
    int m, rows;
    int n, columns;
    int size, length; //m*n

} vec_t;



/*
	Linear Algebra functions.
    --------------------------

    - For now, we only have 2D vectors, but we plan to
      implement generic n-dimensional vectors as well.

    - We're going to use cBLAS library in the algorithms of
      several functions.

    - We're going to implement GPU versions of the algorithms
      using clBLAS and maybe also cuBLAS libraries.
      
*/



//=============================================================================
// COMMON, MAINTAINANCE, USEFUL FUNCTIONS
//=============================================================================
vec_t*       vec_new     (int m, int n);
vec_t*       vec_new_arr (vec_type_t** arr, int m, int n);
vec_type_t** vec_to_arr  (vec_t* vec);
void         vec_free    (vec_t** vecptr);
vec_type_t   vec_get     (vec_t* vec, int i, int j);
void         vec_set     (vec_t* vec, int i, int j, vec_type_t k);
void         vec_set_all (vec_t* vec, vec_type_t k);

// For use with functions that generate some value like random numbers
void vec_set_all_func (vec_t* vec, vec_type_t(*funcptr)(void));

//-----------------------------------------------------------------------------
// TODO
//-----------------------------------------------------------------------------
void vec_get_row         (vec_t* vec, int row);
void vec_set_row         (vec_t* vec, int row, vec_type_t k);
void vec_get_column      (vec_t* vec, int column);
void vec_set_column      (vec_t* vec, int column, vec_type_t k);
void vec_push_row        (vec_t* vec, vec_type_t* arr, int arrsize);
void vec_pop_row         (vec_t* vec, vec_type_t* arr);
void vec_push_column     (vec_t* vec, vec_type_t* arr, int arrsize);
void vec_pop_column      (vec_t* vec, vec_type_t* arr);
//-----------------------------------------------------------------------------

void   vec_copy          (vec_t* dest, vec_t* src);
void   vec_copy_portion  (vec_t* dest, vec_t* src, int offset, int size);
vec_t* vec_clone         (vec_t* vec);
vec_t* vec_clone_portion (vec_t* vec, int offset, int size);
void   vec_print         (vec_t* vec);
void   vec_print_portion (vec_t* vec, int portion, int side);
void   vec_print_bitmap  (vec_t* bitmap, int row, int rows, int columns);

//=============================================================================
// REDUCTIONS
//=============================================================================
vec_type_t vec_inner_sum        (vec_t* A);
vec_t*     vec_get_rows_sums    (vec_t* A);
vec_t*     vec_get_columns_sums (vec_t* A);

//=============================================================================
// SELF OPERATIONS
//=============================================================================
vec_t* vec_transposed (vec_t* A);

//=============================================================================
// ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION,
// EXPONENTIATION, NTH ROOT AND LOG BY A SCALAR...
//=============================================================================
void   vec_add_scalar      (vec_t* A, vec_type_t scalar);
vec_t* vec_get_scalar_sum  (vec_t* A, vec_type_t scalar);
void   vec_sub_scalar      (vec_t* A, vec_type_t scalar);
vec_t* vec_get_scalar_diff (vec_t* A, vec_type_t scalar);
void   vec_mult_scalar     (vec_t* A, vec_type_t scalar);
vec_t* vec_get_scalar_prod (vec_t* A, vec_type_t scalar);
void   vec_div_scalar      (vec_t* A, vec_type_t scalar);
vec_t* vec_get_scalar_div  (vec_t* A, vec_type_t scalar);
void   vec_exp_scalar      (vec_t* A, vec_type_t scalar);
vec_t* vec_get_scalar_exp  (vec_t* A, vec_type_t scalar);
// TODO ---------------------------------------------------
void   vec_nth_root        (vec_t* A, vec_type_t scalar_root);
void   vec_log_base_n      (vec_t* A, vec_type_t scalar_base);
//---------------------------------------------------------
void vec_sqrt (vec_t* A); // Useful, therefore here...

//=============================================================================
// DOT PRODUCT (MATRIX MULTIPLICATION)
//=============================================================================
// Aliases ------------------------------------------------
void vec_dot  (vec_t* A, vec_t* B, vec_t* AB);
void vec_mult (vec_t* A, vec_t* B, vec_t* AB);
//---------------------------------------------------------
// Aliases ------------------------------------------------
vec_t* vec_get_dot  (vec_t* A, vec_t* B);
vec_t* vec_get_mult (vec_t* A, vec_t* B);
//---------------------------------------------------------

//=============================================================================
// ELEMENT-WISE MATRIX OPERATIONS
//=============================================================================
void   vec_add             (vec_t* A, vec_t* B, vec_t* AplusB);
vec_t* vec_get_sum         (vec_t* A, vec_t* B);
void   vec_sub             (vec_t* A, vec_t* B, vec_t* AminusB);
vec_t* vec_get_diff        (vec_t* A, vec_t* B);
void   vec_mult_elwise     (vec_t* A, vec_t* B, vec_t* AB);
vec_t* vec_get_mult_elwise (vec_t* A, vec_t* B);
void   vec_div_elwise      (vec_t* A, vec_t* B, vec_t* AdivB);
vec_t* vec_get_div_elwise  (vec_t* A, vec_t* B);
void   vec_sum_row         (vec_t* A, int row, vec_type_t* arr);

//=============================================================================
// APPLY FUNCTION ELEMENT-WISE
//=============================================================================
typedef vec_type_t (*vec_elwise_op)(vec_type_t);

// vec apply funcs
void        vec_apply     (vec_t* A, vec_elwise_op op);
void        vec_apply_to  (vec_t* dst, vec_t* src, vec_elwise_op op);
vec_t*      vec_apply_out (vec_t* A, vec_elwise_op op);

// vec ops
vec_type_t 	vec_square_op (vec_type_t elem);
vec_type_t 	vec_sqrt_op   (vec_type_t elem);

//=============================================================================



#endif // _LINALG_H_
#ifdef __cplusplus
}
#endif
