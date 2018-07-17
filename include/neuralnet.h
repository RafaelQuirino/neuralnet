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
#include "../include/linalg.h"



typedef struct 
{
    // Network topology
    int* topology;
    int nlayers;

    //-----------------------------------------------------
    // Arrays of vec_t* (matrices from linear_algebra.h)
    //-----------------------------------------------------
    // W  : represents the weight matrices (synapses)
    // B  : represents the bias vectors
    // VdW: represents the "velocities" for weights 
    // VdB: represents the "velocities" for biases 
    vec_t **W, **VdW;
    vec_t **B, **VdB; // Actually, Arrays of vectors
                      // (vec_t's of the type 1 x n)

    // Arrays of activities (Z) and activations (A),
    // where activities are the sums of stimuli in each 
    // neuron and activations are the result of applying 
    // the activation function to these stimuli
    vec_t **Z, **A;

    // Output
    vec_t *yHat;
    //-------------------------------------------------------

} neuralnet_t;



neuralnet_t* nn_new    (int topology[], int tsize, vec_type_t (*func)());
void         nn_free   (neuralnet_t** nn);
void         nn_export (neuralnet_t* nn, const char* fname);
neuralnet_t* nn_import (const char* fname);

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
//---------------------------------------------------------



#endif // _NEURALNET_H_
#ifdef __cplusplus
}
#endif
