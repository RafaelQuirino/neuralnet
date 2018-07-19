#ifdef __cplusplus
extern "C" {
#endif
#ifndef _NEURALNET_H_
#define _NEURALNET_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"
#include "../include/data.h"
#include "../include/linalg.h"



//---------------------------------------------------------
// Defining types weight initialization
//---------------------------------------------------------
#define NN_WINIT_RANDOM                0
#define NN_WINIT_GAUSSIAN              1
#define NN_WINIT_VARIANCE_CALLIBRATION 2
#define NN_BINIT_ONE                   3
#define NN_BINIT_ZERO                  4
#define NN_BINIT_SMALL                 5
#define NN_BINIT_RANDOM                6
#define NN_BINIT_GAUSSIAN              7
//---------------------------------------------------------

//---------------------------------------------------------
// Defining types of loss and cost functions
//---------------------------------------------------------
#define NN_SQUARE_ERROR              0
#define NN_CROSS_ENTROPY             1
#define NN_MEAN_SQUARE_ERROR         2
#define NN_HALF_SQUARE_ERROR         3
#define NN_BINAR_CROSS_ENTROPY       4
#define NN_LOGISTIC_REGRESSION       5
#define NN_HALF_MEAN_SQUARE_ERROR    6
#define NN_NEGATIVE_LOG_LIKELIHOOD   7
#define NN_CATEGORICAL_CROSS_ENTROPY 8
//---------------------------------------------------------

//---------------------------------------------------------
// Defining types of activation functions
//---------------------------------------------------------
#define NN_IDENTITY_ACTIVATION           0
#define NN_RELU_ACTIVATION               1
#define NN_SIGMOID_ACTIVATION            2
#define NN_SOFTMAX_ACTIVATION            3
#define NN_HYPERBOLIC_TANGENT_ACTIVATION 4
//---------------------------------------------------------

//---------------------------------------------------------
// Defining types of regularization
//---------------------------------------------------------
#define NN_NO_REGULARIZATION        0
#define NN_L1_REGULARIZATION        1
#define NN_L2_REGULARIZATION        2
#define NN_DROPOUT_REGULARIZATION   3
#define NN_FROBENIUS_REGULARIZATION 4
//---------------------------------------------------------



typedef struct 
{
    // Network topology
    // (1st layer is only input, no activation or dropout)
    int*   topology;    // Each layer's # of hidden units
    int*   activations; // Each layer's activation function
    float* keep_probs;  // Each layer's chance of dropout
    int    nlayers;     // # of layers

    //-----------------------------------------------------
    // Arrays of vec_t* (matrices from linear_algebra.h)
    //-----------------------------------------------------
    // W  : represents the weight matrices (synapses)
    // B  : represents the bias vectors
    // VdW: represents the "velocities" for weights 
    // VdB: represents the "velocities" for biases 
    vec_t **W, **VdW;
    vec_t **B, **VdB;

    // Arrays of activities (Z) and activations (A),
    // where activities are the sums of stimuli in each 
    // neuron and activations are the result of applying 
    // the activation function to these stimuli
    vec_t **Z, **A;

    // Output
    vec_t *yHat;
    //-------------------------------------------------------

    // Hyper-parameters
    float learning_rate;
    float momentum;
    float keep_prob;
    int   cost_function;

    // Flags
    int regularization;
    int input_normalization;

} neuralnet_t;



// Common "CRUD" functions
neuralnet_t* nn_new    (int topology[], int tsize, double (*func)());
void         nn_free   (neuralnet_t** nn);
void         nn_export (neuralnet_t* nn, const char* fname);
neuralnet_t* nn_import (const char* fname);

// Configuring neuralnet's mechanism parameters
void nn_set_cost_function    (neuralnet_t* nn, int cost_func_code);
void nn_set_layer_activation (neuralnet_t* nn, int layeridx, int act_func_code);

// Main interface
void nn_train (neuralnet_t* nn, dataset_t* data);
void nn_test  (neuralnet_t* nn, dataset_t* data);

// Run the network, saving all activities (Z) and activation (A) matrices
// for the backpropagation to work.
vec_t* nn_forward (neuralnet_t* nn, vec_t* data);

// Do not save activities (Z) and activations (A)
// (Just to get neuralnet's output, not running backpropagation)
vec_t* nn_feed_forward (neuralnet_t* nn, vec_t* data);

vec_type_t nn_cost_func (neuralnet_t* nn, vec_t* X, vec_t* Y);

vec_type_t nn_cost_func_prime (
	neuralnet_t* nn, vec_t* X, vec_t* Y,
	vec_t*** dJdWs_out, vec_t*** dJdBs_out
);

vec_type_t nn_activation_func ();

vec_type_t nn_activation_func_prime ();

void nn_backpropagation (
	neuralnet_t* nn, vec_t* X, vec_t* Y, 
	int num_iterations,
	vec_type_t learning_rate
);

// Memory efficient version.
// Calculate gradients from last to first layer,
// use it to update weights and free them before
// going one layer back (thus saving memory)
void nn_backpropagation_2 (
	neuralnet_t* nn, vec_t* X, vec_t* Y, 
	int num_iterations,
	vec_type_t learning_rate
);



// Specific math functions --------------------------------
vec_type_t nn_sigmoid       (vec_type_t k);
vec_type_t nn_sigmoid_prime (vec_type_t k);

vec_type_t nn_relu       (vec_type_t k);
vec_type_t nn_relu_prime (vec_type_t k);
//---------------------------------------------------------



#endif // _NEURALNET_H_
#ifdef __cplusplus
}
#endif
