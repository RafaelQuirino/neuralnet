#include "../include/neuralnet.h"



neuralnet_t* nn_new (int topology[], int tsize)
{
	// Variables
	int i, line;

	// New neural net being created
	neuralnet_t* newnet = NULL;
	newnet = (neuralnet_t*) malloc(sizeof(neuralnet_t));
	line = __LINE__ - 1;
	if (!newnet)
	{
		ut_errmsg (
			"Couldnot allocate memory for the neuralnet_t.",
			__FILE__, line, 1
		);
	}
	
	// Building topology...
	newnet->topology = NULL;
	newnet->topology = (int*) malloc(tsize * sizeof(int));
	line = __LINE__ - 1;
	if (!newnet->topology)
	{
		ut_errmsg (
			"Couldnot allocate memory for the topology.",
			__FILE__, line, 1
		);
	}
	for (i = 0; i < tsize; i++)
		newnet->topology[i] = topology[i];
	newnet->nlayers = tsize;

	// Building weight matrices and bias vectors
	newnet->W = NULL;
	newnet->W = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	line = __LINE__ - 1;
	if (!newnet->W)
	{
		ut_errmsg (
			"Couldnot allocate memory for the weight matrices.",
			__FILE__, line, 1
		);
	}
	newnet->B = NULL; // (1 x n vectors)...
	newnet->B = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	line = __LINE__ - 1;
	if (!newnet->B)
	{
		ut_errmsg (
			"Couldnot allocate memory for the bias matrices.",
			__FILE__, line, 1
		);
	}
	nn_initialize_weights(newnet);
	for (i = 0; i < tsize-1; i++) 
	{
		// newnet->W[i] = vec_new(topology[i],topology[i+1]);
		// vec_set_all_func(newnet->W[i], ut_gaussian_rand);

		newnet->B[i] = vec_new(1,topology[i+1]);
		vec_set_all_func(newnet->B[i], ut_gaussian_rand);
	}

	// Building activity (Z) and activation (A) matrices...
	newnet->Z = NULL;
	newnet->Z = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	line = __LINE__ - 1;
	if (!newnet->Z)
	{
		ut_errmsg (
			"Couldnot allocate memory for the activity matrices.",
			__FILE__, line, 1
		);
	}
	newnet->A = NULL;
	newnet->A = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	line = __LINE__ - 1;
	if (!newnet->A)
	{
		ut_errmsg (
			"Could not allocate memory for the activation matrices.",
			__FILE__, line, 1
		);
	}
	// Initializing activity and activation matrices
	for (i = 0; i < tsize-1; i++) 
	{
		newnet->Z[i] = NULL;
		newnet->A[i] = NULL;		
	}

	// Building "velocities" for momentum learning
	newnet->VdW = NULL;
	newnet->VdW = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	if (!newnet->VdW)
	{
		ut_errmsg (
			"Couldnot allocate memory for the momentum weight velocities.",
			__FILE__, line, 1
		);
	}
	newnet->VdB = NULL;
	newnet->VdB = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	if (!newnet->VdB)
	{
		ut_errmsg (
			"Couldnot allocate memory for the momentum bias velocities.",
			__FILE__, line, 1
		);
	}
	for (i = 0; i < tsize-1; i++) 
	{
		newnet->VdW[i] = vec_new(topology[i],topology[i+1]);
		vec_set_all(newnet->VdW[i], 0);

		newnet->VdB[i] = vec_new(1, topology[i+1]);
		vec_set_all(newnet->VdB[i], 0);
	}

	// Building "sums" for RMS and ADAM learning
	newnet->SdW = NULL;
	newnet->SdW = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	if (!newnet->SdW)
	{
		ut_errmsg (
			"Couldnot allocate memory for SdW matrices.",
			__FILE__, line, 1
		);
	}
	newnet->SdB = NULL;
	newnet->SdB = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));
	if (!newnet->SdB)
	{
		ut_errmsg (
			"Couldnot allocate memory for SdB matrices.",
			__FILE__, line, 1
		);
	}
	for (i = 0; i < tsize-1; i++) 
	{
		newnet->SdW[i] = vec_new(topology[i],topology[i+1]);
		vec_set_all(newnet->SdW[i], 0);

		newnet->SdB[i] = vec_new(1, topology[i+1]);
		vec_set_all(newnet->SdB[i], 0);
	}

	// Initializing net's output
	newnet->yHat = NULL;

	// Creating activation functions
	newnet->activations = NULL;
	newnet->activations = (int*) malloc ((tsize-1) * sizeof(int));
	line = __LINE__ - 1;
	if (!newnet->activations)
	{
		ut_errmsg (
			"Couldnot allocate memory for the activations.",
			__FILE__, line, 1
		);
	}

	for (i = 0; i < (tsize-1); i++)
	{
		newnet->activations[i] = NN_SIGMOID_ACTIVATION;
	}

	// Configurations
	//================
	newnet->cost_function     = NN_SQUARE_ERROR;
	newnet->output_activation = NN_SOFTMAX_ACTIVATION;
	newnet->regularization    = NN_L2_REGULARIZATION;
	newnet->optimization      = NN_ADAM_OPTIMIZATION;
	
	newnet->rms_rate            = 0.9;
	newnet->momentum_rate       = 0.9;
	newnet->learning_rate       = 0.001;
	newnet->regularization_rate = 0.0001;

	// Setting output layer's activation
	//===================================
	newnet->activations[(tsize-1)-1] = newnet->output_activation;

	return 	newnet;
}



void nn_free (neuralnet_t **nn)
{
	if ((*nn) == NULL)
		return;

	int i;

	for (i = 0; i < (*nn)->nlayers-1; i++) 
	{
		vec_free(&(*nn)->W[i]);
		vec_free(&(*nn)->B[i]);
		vec_free(&(*nn)->Z[i]);
		vec_free(&(*nn)->A[i]);
		vec_free(&(*nn)->VdW[i]);
		vec_free(&(*nn)->VdB[i]);
		vec_free(&(*nn)->SdW[i]);
		vec_free(&(*nn)->SdB[i]);
	}

	vec_free(&(*nn)->yHat);

	free((*nn)->W);
	free((*nn)->B);
	free((*nn)->Z);
	free((*nn)->A);
	free((*nn)->VdW);
	free((*nn)->VdB);
	free((*nn)->SdW);
	free((*nn)->SdB);
	free((*nn)->topology);
	free((*nn));

	(*nn) = NULL;
}



/*
	neural_net_t file format
	========================

	We define a file, with extension ".nn", in order to 
	export and import neural nets. We define nn_export
	and nn_import functions below.

	* ( We still have to choose between little and big endian as
	    the standard bit format. )
	* ( We also have to create some wrapper functions for 
	    fread and fwrite, to deal with different machines. )

	=> The "file.nn" format
	   ---------------------

	   - 32 bit integer: number of layers (nlayers)
	   - for each of these layers
	     - 32 bit integer: number of neurons in layer
	   
	   Now we have a topology array [#neurons_layer1, #neurons_layer2, ... , #neurons_layerN]

	   - for each of these layers but the first (input has no activation)
	     - 32 bit integer: NN_*_ACTIVATION codes for each layer (but the first)

	   Now we have the activation codes for each layer, but the first

	   - (#neurons_layer1 * #neurons_layer2) 64 bit doubles : 1st weight matrix
	   - (              1 * #neurons_layer2) 64 bit doubles : 1st bias   vector
	   - (#neurons_layer2 * #neurons_layer3) 64 bit doubles : 2st weight matrix
	   - (              1 * #neurons_layer3) 64 bit doubles : 2st bias   vector
	   .
	   .
	   .
	   - (#neurons_layer(N-1) * #neurons_layerN) 64 bit double : (N-1)th weight matrix
	   - (                  1 * #neurons_layerN) 64 bit double : (N-1)th bias   vector

	   Now we have all the weights and biases

	   - 32 bit integer: cost_function     code
	   - 32 bit integer: output_activation code
	   - 32 bit integer: regularization    code
	   - 32 bit integer: optimization      code	   
*/
void nn_export (neuralnet_t* nn, const char* fname)
{
	// Variables
	int i, j, line;
	size_t res;

	// If i want to change fname later...
	const char* namebuff = fname;

	// Open the file for writing
	FILE* fp = fopen(namebuff, "wb");

	// First, fwrite the number of layers
	res = fwrite(&nn->nlayers, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	
	if (res != 1) 
	{
		ut_errmsg (
			"Couldnot fwrite nn->layers into file.",
			__FILE__, line, 1
		);
	}

	// Then, for each layer, fwrite the number of neurons
	for (i = 0; i < nn->nlayers; i++) 
	{
		res = fwrite(&nn->topology[i], sizeof(int), 1, fp);
		line = __LINE__ - 1;

		if (res != 1) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fwrite nn->topology[%d] into file.", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}
	}

	// Now, for each layer but the first, fwrite its activation code
	for (i = 0; i < nn->nlayers-1; i++) 
	{
		res = fwrite(&nn->activations[i], sizeof(int), 1, fp);
		line = __LINE__ - 1;

		if (res != 1) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fwrite nn->activations[%d] into file.", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}
	}

	// Then, for each layer, fwrite the weights linearly, row-wise, and biases
	for (i = 0; i < nn->nlayers-1; i++)
	{
		for (j = 0; j < nn->W[i]->size; j++)
		{
			double val = (double) nn->W[i]->vec[j];
			res = fwrite(&val, sizeof(double), 1, fp);
			line = __LINE__ - 1;
			if (res != 1) 
			{
				char msgbuf[128];
				sprintf(msgbuf, "Couldnot fwrite nn->W[%d]->vec[%d] into file.", i,j);
				ut_errmsg (
					msgbuf,
					__FILE__, line, 1
				);
			}
		}

		for (j = 0; j < nn->B[i]->size; j++)
		{
			double val = (double) nn->B[i]->vec[j];
			res = fwrite(&val, sizeof(double), 1, fp);
			line = __LINE__ - 1;
			if (res != 1) 
			{
				char msgbuf[128];
				sprintf(msgbuf, "Couldnot fwrite nn->B[%d]->vec[%d] into file.", i,j);
				ut_errmsg (
					msgbuf,
					__FILE__, line, 1
				);
			}
		}		
	} // for (i = 0; i < nn->nlayers-1; i++)

	res = fwrite(&nn->cost_function, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
		ut_errmsg (
			"Couldnot fwrite cost_function code into file.",
			__FILE__, line, 1
		);
	}

	res = fwrite(&nn->output_activation, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
		ut_errmsg (
			"Couldnot fwrite output_activation code into file.",
			__FILE__, line, 1
		);
	}

	res = fwrite(&nn->regularization, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
		ut_errmsg (
			"Couldnot fwrite regularization code into file.",
			__FILE__, line, 1
		);
	}

	res = fwrite(&nn->optimization, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
		ut_errmsg (
			"Couldnot fwrite optimization code into file.",
			__FILE__, line, 1
		);
	}

	fclose(fp);
}



neuralnet_t* nn_import (const char* fname)
{
	// Variables
	int i, j, numlayers, line;
	size_t res;

	FILE* fp = fopen(fname, "rb");

	res = fread(&numlayers, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
        ut_errmsg (
			"Couldn't fread number of layers file.",
			__FILE__, line, 1
		);
	}
	
	int topology[numlayers];
	for (i = 0; i < numlayers; i++)
	{
		res = fread(&topology[i], sizeof(int), 1, fp);
		line = __LINE__ - 1;

		if (res != 1) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fread number of neurons of layer %d.", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}
	}

	// Create new neuralnet_t*
	neuralnet_t* nn = nn_new(topology, numlayers);

	for (i = 0; i < numlayers-1; i++)
	{
		res = fread(&nn->activations[i], sizeof(int), 1, fp);
		line = __LINE__ - 1;

		if (res != 1) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fread activation code of layer %d.", i+1);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}
	}

	for (i = 0; i < numlayers-1; i++)
	{
		// Read weights
		int numweights = topology[i] * topology[i+1];
		for (j = 0; j < numweights; j++)
		{
			double val;
			res = fread(&val, sizeof(double), 1, fp);
			line = __LINE__ - 1;
			if (res != 1) 
			{
				char msgbuf[128];
				sprintf(msgbuf, "Couldnot fread weight %d of layer %d.", j,i);
				ut_errmsg (
					msgbuf,
					__FILE__, line, 1
				);
			}
			nn->W[i]->vec[j] = (vec_type_t) val;
		}

		// Read biases
		int numbiases = 1 * topology[i+1];
		for (j = 0; j < numbiases; j++)
		{
			double val;
			res = fread(&val, sizeof(double), 1, fp);
			line = __LINE__ - 1;
			if (res != 1) 
			{
				char msgbuf[128];
				sprintf(msgbuf, "Couldnot fread bias %d of layer %d.", j,i);
				ut_errmsg (
					msgbuf,
					__FILE__, line, 1
				);
			}
			nn->B[i]->vec[j] = (vec_type_t) val;
		}
	}

	res = fread(&nn->cost_function, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
        ut_errmsg (
			"Couldn't fread cost_function code.",
			__FILE__, line, 1
		);
	}

	res = fread(&nn->output_activation, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
        ut_errmsg (
			"Couldn't fread output_activation code.",
			__FILE__, line, 1
		);
	}

	res = fread(&nn->regularization, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
        ut_errmsg (
			"Couldn't fread regularization code.",
			__FILE__, line, 1
		);
	}

	res = fread(&nn->optimization, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	if (res != 1) 
	{
        ut_errmsg (
			"Couldn't fread optimization code.",
			__FILE__, line, 1
		);
	}

	fclose(fp);

	return nn;
}



void nn_initialize_weights (neuralnet_t* nn)
{
	int i, j, k;

	for (k = 0; k < nn->nlayers-1; k++) 
	{
		nn->W[k] = vec_new(nn->topology[k],nn->topology[k+1]);

		for (i = 0; i < nn->W[k]->m; i++)
		{
			for (j = 0; j < nn->W[k]->n; j++)
			{
				vec_type_t val = (vec_type_t) ut_gaussian_rand();
				float init_term = sqrt(1.0/(float)nn->topology[k]);
				val *= (vec_type_t) init_term;
				vec_set(nn->W[k],i,j,val);
			}
		}
	}
}



void nn_set_cost_function (neuralnet_t* nn, int cost_func_code)
{

}



void nn_set_output_function (neuralnet_t* nn, int output_func_code)
{

}



void nn_set_layer_activation (neuralnet_t* nn, int layeridx, int act_func_code)
{

}



// Run the network, saving all activities (Z) and activation (A) matrices
// for the backpropagation to work.
vec_t* nn_forward (neuralnet_t* nn, vec_t* data)
{
	int line = __LINE__ - 2;
	if(data->n != nn->W[0]->m) 
	{
		char msgbuf[128];
		sprintf(msgbuf, "vec_t* data is not in the right format (%dx%d).",
			data->m, nn->W[0]->m
		);
		ut_errmsg (
			msgbuf,
			__FILE__, line, 1
		);
	}

	// Variables
	int i, row;

	// First, free all previous Z[i] and A[i] memory
	for (i = 0; i < nn->nlayers-1; i++) {
		vec_free(&nn->Z[i]);
		vec_free(&nn->A[i]);
	}

	// Then, initialize the current layer input
	// to be the data (input for first layer)
	vec_t* layerInput = data;

	//------------------------------------------------
	// Now, for each layer but the first...
	// (input layer is not really a layer)
	// Obs.: Z[0] represent activities in layer 1...
	//------------------------------------------------
	for (i = 0; i < nn->nlayers-1; i++) 
	{	
		//-------------------------------------
		// Calculate Z[i] and A[i] = fact(Z[i])
		//-------------------------------------

		// First, Z[i] = layerInput * nn->W[i]
		nn->Z[i] = vec_get_dot(layerInput, nn->W[i]);
		
		// Then add bias, Z[i][row] = Z[i][row] + B[i], for each row
		for (row = 0; row < nn->Z[i]->m; row++)
			vec_sum_row(nn->Z[i], row, nn->B[i]->vec);

		// Last, make A[i] = f(Z[i]),
		// where f is the activation function
		nn->A[i] = nn_apply_activation(nn, i);

		// Update the layer input to be
		// the activation of the previous layer
		layerInput = nn->A[i];
	}

	// Return a clone to the last 
	// activation layer matrix (the output)
	vec_t* result_vec = vec_clone(nn->A[nn->nlayers-2]);

	return result_vec;
}



// Do not save activities (Z) and activations (A)
// (Just to get neural_net's output, not running backpropagation)
vec_t* nn_feed_forward (neuralnet_t* nn, vec_t* data)
{
	int line = __LINE__ - 2;
	if(data->n != nn->W[0]->m) 
	{
		char msgbuf[128];
		sprintf(msgbuf, "vec_t* data is not in the right format (nx%d).", nn->topology[0]);
		ut_errmsg (
			msgbuf,
			__FILE__, line, 1
		);
	}

	// Variables
	int i, row;

	// First, free all previous Z[i]
	for (i = 0; i < nn->nlayers-1; i++)
		vec_free(&nn->Z[i]);

	// Then, initialize the current layer input
	// to be the data (input for first layer)
	vec_t* layerInput = data;

	//-----------------------------------------------------
	// Now, for each layer but the first...
	// (input layer is not really a layer).
	// We're going to use nn->Z[i] as an auxiliar
	// memoty to run the data through the neural net.
	//-----------------------------------------------------
	for (i = 0; i < nn->nlayers-1; i++) 
	{
		// First, Z[i] = layerInput * nn->W[i]
		nn->Z[i] = vec_get_dot (layerInput, nn->W[i]);
		
		// Then add bias, Z[i][row] = Z[i][row] + B[i], for each row
		for (row = 0; row < nn->Z[i]->m; row++)
			vec_sum_row (nn->Z[i], row, nn->B[i]->vec);

		// Last, make Z[i] = f(Z[i]),
		// where f is the activation function
		nn->Z[i] = nn_apply_activation(nn, i);

		// Update the layer input to be
		// the activation of the previous layer
		layerInput = nn->Z[i];

		// Freeing last layer's memory
		if (i > 0) {
			vec_free(&nn->Z[i-1]);
		}
	}

	// Return a clone to the last 
	// activation layer matrix (the output)
	vec_t* result_vec = vec_clone(nn->Z[nn->nlayers-2]);
	vec_free(&nn->Z[nn->nlayers-2]);

	return result_vec;
}



double nn_cost_func (
	neuralnet_t* nn, vec_t* y, vec_t* yHat,
	int funcflag
)
{
	double cost = 0.0;

	if (funcflag == NN_SQUARE_ERROR)
	{
		vec_t* y_yHat = vec_get_diff(y,yHat);
		vec_apply(y_yHat, vec_square_op);
		cost = (1.0) * vec_inner_sum(y_yHat);
		vec_free(&y_yHat);
	}
	else if (funcflag == NN_HALF_SQUARE_ERROR)
	{
		vec_t* y_yHat = vec_get_diff(y,yHat);
		vec_apply(y_yHat, vec_square_op);
		cost = (1.0/2.0) * vec_inner_sum(y_yHat);
		vec_free(&y_yHat);
	}
	else if (funcflag == NN_MEAN_SQUARE_ERROR)
	{
		vec_t* y_yHat = vec_get_diff(y,yHat);
		vec_apply(y_yHat, vec_square_op);
		cost = (1.0/(vec_type_t)y->m) * vec_inner_sum(y_yHat);
		vec_free(&y_yHat);
	}
	else if (funcflag == NN_HALF_MEAN_SQUARE_ERROR)
	{
		vec_t* y_yHat = vec_get_diff(y,yHat);
		vec_apply(y_yHat, vec_square_op);
		cost = (1.0/(2.0*(vec_type_t)y->m)) * vec_inner_sum(y_yHat);
		vec_free(&y_yHat);
	}
	// ***   PROBABLY WRONG   ***
	else if (funcflag == NN_CROSS_ENTROPY)
	{
		// ylnyHat = y * ln(yHat)
		vec_t* ylnyHat = vec_apply_out(yHat, vec_log_op);
		vec_mult_elwise(y, ylnyHat, ylnyHat);
		// cost = (-1.0/(vec_type_t)y->m) * vec_inner_sum(ylnyHat);
		cost = (-1.0) * vec_inner_sum(ylnyHat);
		vec_free(&ylnyHat);
	}
	// ***   THIS IS WRONG   ***
	// J = -1/n * Sum{y/yHat + (1-y)/(1-yHat)}
	else if (funcflag == NN_BINARY_CROSS_ENTROPY)
	{
		// (1-y)
		vec_t* aux1 = vec_get_scalar_prod(y,-1);
		vec_add_scalar(aux1,1);
		
		// (1-yHat)
		vec_t* aux2 = vec_get_scalar_prod(nn->yHat,-1);
		vec_add_scalar(aux2,1);

		// aux1 := (1-y)ln(1-yHat)
		vec_apply(aux2,vec_log_op);
		vec_mult_elwise(aux1, aux2, aux1);

		// yln(yHat)
		vec_free(&aux2);
		aux2 = vec_apply_out(yHat,vec_log_op);
		vec_t* ylnyHat = vec_get_mult_elwise(y,aux2);

		// Sum of yln(yHat) + (1-y)ln(1-yHat)
		vec_add(ylnyHat,aux1,aux1);
		cost = (-1.0/(vec_type_t)y->m) * vec_inner_sum(aux1);
		
		vec_free(&aux1);
		vec_free(&aux2);
		vec_free(&ylnyHat);
	}

	cost += (double) nn_regularization_term(nn);
	
	return cost;
}



vec_t* nn_cost_func_gradient (
    neuralnet_t* nn, vec_t* y, vec_t* yHat,
	int funcflag
)
{
	vec_t* grads = vec_new(y->m,y->n);

	if (funcflag == NN_SQUARE_ERROR)
	{
		vec_sub(y, nn->yHat, grads);
		vec_mult_scalar(grads,-2);
	}
	else if (funcflag == NN_HALF_SQUARE_ERROR)
	{
		vec_sub(y, nn->yHat, grads);
		vec_mult_scalar(grads,-1);
	}
	else if (funcflag == NN_MEAN_SQUARE_ERROR)
	{
		vec_sub(y, nn->yHat, grads);
		vec_mult_scalar(grads,-(2/(vec_type_t)y->m));
	}
	else if (funcflag == NN_HALF_MEAN_SQUARE_ERROR)
	{
		vec_sub(y, nn->yHat, grads);
		vec_mult_scalar(grads,-(1/(vec_type_t)y->m));
	}
	// ***   PROBABLY WRONG   ***
	else if (funcflag == NN_CROSS_ENTROPY)
	{
		grads = vec_clone(y);
		vec_mult_scalar(grads,-1);
		vec_div_elwise(grads, yHat, grads);
	}
	// ***   THIS IS WRONG   ***
	// J = -1/n * Sum{y/yHat + (1-y)/(1-yHat)}
	else if (funcflag == NN_BINARY_CROSS_ENTROPY)
	{
		// aux1 := (1-y)/(1-yHat)
		vec_t* aux1 = vec_get_scalar_prod(y,-1);
		vec_t* aux2 = vec_get_scalar_prod(nn->yHat,-1);
		vec_add_scalar(aux1,1);
		vec_add_scalar(aux2,1);
		vec_div_elwise(aux1, aux2, aux1);

		// grads := y/yHat
		vec_div_elwise(y, nn->yHat, grads);

		// grads := -1/n * {y/yHat + (1-y)/(1-yHat)}
		vec_add(grads, aux1, grads);
		vec_mult_scalar(grads,-1*(1.0/(vec_type_t)y->m));

		vec_free(&aux1);
		vec_free(&aux2);
	}

	return grads;
}



vec_type_t nn_cost_func_prime (
	neuralnet_t* nn, vec_t* X, vec_t* Y,
	vec_t*** dJdWs_out, vec_t*** dJdBs_out
)
{
	// Variables
	int i;

	// First of all, forward the data
	// and get the net's output (yHat)
	vec_free(&nn->yHat);
	nn->yHat = nn_forward(nn, X);

	//-----------------------------------------------------
	// CALCULATE THE COST FOR OUTPUT
	//-----------------------------------------------------
	double cost = nn_cost_func(
		nn,Y,nn->yHat,
		nn->cost_function
	);
	//-----------------------------------------------------

	//==============================================
	// CALCULATE GRADIENTS FROM LAST TO FIRST LAYER
	//==============================================

	//-----------------
	// Initializations
	//-----------------

	// dJdW outputs
	vec_t** dJdWs = (vec_t**) malloc((nn->nlayers-1) * sizeof(vec_t*));

	// dJdB outputs
	vec_t** dJdBs = (vec_t**) malloc((nn->nlayers-1) * sizeof(vec_t*));

	// First activation matrix (Xt)
	vec_t* Xt = vec_transposed(X);
	
	// Loop variables
	vec_t* act        = NULL;
	vec_t* grads_z    = NULL;
	vec_t* delta      = NULL;
	vec_t* Wt         = NULL;
	vec_t* Waux       = NULL;
	
	
	
	// COST FUNCTION DERIVATIVE ---------------------------
	// vec_t* errsign = _y_yHat;
	vec_t* errsign = nn_cost_func_gradient(
		nn,Y,nn->yHat,
		nn->cost_function
	);
	//-----------------------------------------------------



	// Backpropagation loop
	for (i = nn->nlayers-2; i >= 0; i--) 
	{
		//---------------------------------------------------------------------
		// Prepare matrices 
		//---------------------------------------------------------------------
		grads_z = nn_apply_activation_prime(nn, i);
		
		if (i == 0)
			act = Xt; // First layer's activation is the data itself
		else
			act = vec_transposed(nn->A[i-1]);
		//---------------------------------------------------------------------

		//---------------------------------------------------------------------
		// Calculating and saving dJdW and dJdB
		//---------------------------------------------------------------------
		delta = vec_get_mult_elwise(errsign, grads_z);
		vec_free(&grads_z);
		vec_free(&errsign);

		// Weights gradient
		dJdWs[i] = vec_get_dot(act, delta);

		// Regularization
		if (nn->regularization == NN_L2_REGULARIZATION)
		{
			Waux = vec_get_scalar_prod(nn->W[i], nn->regularization_rate);
			vec_add(dJdWs[i], Waux, dJdWs[i]);
			vec_free(&Waux);
		}

		// Bias gradient, reference below...
		// "https://datascience.stackexchange.com/questions/20139/
		//  gradients-for-bias-terms-in-backpropagation/20142#20142"
		dJdBs[i] = vec_get_columns_sums(delta);
		
		vec_free(&act);
		//---------------------------------------------------------------------

		//---------------------------------------------------------------------
		// Post iteration calculations (errsign for the next iteration)
		//---------------------------------------------------------------------
		Wt = vec_transposed(nn->W[i]);
		errsign = vec_get_dot(delta,Wt);

		vec_free(&Wt);
		vec_free(&delta);
		//---------------------------------------------------------------------
		
	} // for (i = nn->nlayers-2; i >= 0; i--)
	
	// Freeing memory
	vec_free(&errsign);

	// Outputting results
	*dJdWs_out = dJdWs;
	*dJdBs_out = dJdBs;

	return cost;
}



void nn_backpropagation (
	neuralnet_t* nn, vec_t* X, vec_t* Y, 
	int num_iterations,
	vec_type_t learning_rate
)
{
	// Variables
	int i, j;
	vec_t **dJdW, **dJdB;

	// Parameters
	double momentum = nn->momentum_rate;

	// For each iteration of backpropagation
	for (i = 0; i < num_iterations; i++) 
	{
		// Calculate gradients
		double cost = nn_cost_func_prime(nn, X, Y, &dJdW, &dJdB);
		printf("iteration: %d, cost: %g\n", i+1, cost);		

		// For each layer in the neural net
		for (j = 0; j < nn->nlayers-1; j++)
		{
			nn_optimization(
				nn, dJdW[j], dJdB[j],
				learning_rate, j, i, nn->optimization
			);

			//Freeing memory
			vec_free(&dJdW[j]);
			vec_free(&dJdB[j]);

		} // for (j = 0; j < nn->nlayers-1; j++)
	}
}



void nn_backpropagation_sgd (
	neuralnet_t* nn,
	dataset_t* dataset, 
	int num_iterations,
	vec_type_t learning_rate
)
{
	// Variables
	int i, j;
	vec_t **dJdW, **dJdB;
	double costsum  = 0.0;
	double costmean = 0.0;

	// Parameters
	double momentum = nn->momentum_rate;
	learning_rate   = nn->learning_rate; //overwriting...

	// For each iteration of backpropagation
	for (i = 0; i < num_iterations; i++) 
	{
		minibatch_t* batch = dat_next_minibatch(dataset);

		// Calculate gradients
		double cost = nn_cost_func_prime(nn, batch->X, batch->Y, &dJdW, &dJdB);
		costsum += cost;
		costmean = costsum / (double) (i+1);

		dat_free_minibatch(&batch);
		
		printf(
			"\repoch: %d, batch: %d, iteration: %d, cost: %g, mean: %g",
			dataset->current_epoch, dataset->current_batch,
			dataset->current_iteration, cost, costmean
		);
		fflush(stdout);		

		// For each layer in the neural net
		for (j = 0; j < nn->nlayers-1; j++)
		{
			nn_optimization(
				nn, dJdW[j], dJdB[j],
				learning_rate, j, i, nn->optimization
			);

			//Freeing memory
			vec_free(&dJdW[j]);
			vec_free(&dJdB[j]);

		} // for (j = 0; j < nn->nlayers-1; j++)
	}
	printf("\n");
}



// This version of nn_backpropagation includes all code from
// nn_cost_func_prime function into the algorithm, in order to
// avoid calculating all gradients from all layers before the
// weights update. Now we only have to calculate the gradients
// for the current layer being processed, thus saving memory.
void nn_backpropagation_mem (
	neuralnet_t* nn, vec_t* X, vec_t* Y, 
	int num_iterations,
	vec_type_t learning_rate
)
{
	// Variables
	int i, j, k;

	// Parameters
	double momentum = nn->momentum_rate;

	// For each iteration of backpropagation
	for (i = 0; i < num_iterations; i++) 
	{
		// First of all, forward the data
		// and get the net's output (yHat)
		// (also nn->Z[i] and nn->A[i] matrices)
		vec_free(&nn->yHat);
		nn->yHat = nn_forward(nn,X);

		//-----------------------------------------------------
		// CALCULATE THE COST FOR OUTPUT
		//-----------------------------------------------------
		// (y - yHat) ^ 2
		vec_t* y_yHat_tmp = vec_get_diff(Y,nn->yHat);

		vec_apply(y_yHat_tmp, vec_square_op);
		double cost = 0.5 * vec_inner_sum(y_yHat_tmp);
		vec_free(&y_yHat_tmp);
		printf("iteration: %d, cost: %g\n", i+1, cost);
		//-----------------------------------------------------

		//==============================================
		// CALCULATE GRADIENTS FROM LAST TO FIRST LAYER
		//==============================================

		//-----------------
		// Initializations
		//-----------------
		vec_t* dJdW;
		vec_t* dJdB;

		// First error signal, -(y - yHat)
		vec_t* _y_yHat = vec_get_diff(Y,nn->yHat);
		vec_mult_scalar(_y_yHat,-1.0);
		vec_t* errsign = _y_yHat;

		// First activation matrix (Xt)
		vec_t* Xt = vec_transposed(X);
		
		// Loop variables
		vec_t* act        = NULL;
		vec_t* grads_z    = NULL;
		vec_t* delta      = NULL;
		vec_t* Wt         = NULL;

	 	// Backpropagation loop
		for (j = nn->nlayers-2; j >= 0; j--) 
		{
			//-----------------------------------------------------------------
			// Prepare matrices 
			//-----------------------------------------------------------------
			grads_z = nn_apply_activation_prime(nn, i);

			if (j == 0)
				act = Xt; // First layer's activation is the data itself
			else
				act = vec_transposed(nn->A[j-1]);
			//-----------------------------------------------------------------

			//-----------------------------------------------------------------
			// Calculating and saving dJdW and dJdB
			//-----------------------------------------------------------------
			delta = vec_get_mult_elwise(errsign, grads_z);

			vec_free(&grads_z);
			vec_free(&errsign);

			// Weights gradient
			dJdW = vec_get_dot(act, delta);

			// Bias gradient, reference below...
			// "https://datascience.stackexchange.com/questions/20139/
			//  gradients-for-bias-terms-in-backpropagation/20142#20142"
			dJdB = vec_get_columns_sums(delta);
			
			vec_free(&act);
			//-----------------------------------------------------------------

			//-----------------------------------------------------------------
			// Post iteration calculations (errsign for the next iteration)
			//-----------------------------------------------------------------
			Wt = vec_transposed(nn->W[j]);
			errsign = vec_get_dot(delta,Wt);

			vec_free(&Wt);
			vec_free(&delta);
			//-----------------------------------------------------------------

			nn_optimization(
				nn, dJdW, dJdB,
				learning_rate, j, i, nn->optimization
			);

			//Freeing memory
			vec_free(&dJdW);
			vec_free(&dJdB);
			//-----------------------------------------------------------------

		} // for (j = nn->nlayers-2; j >= 0; j--)
		
		// Freeing memory
		vec_free(&errsign);

	} // for (i = 0; i < num_iterations; i++)

} // void nn_backpropagation_2



void nn_optimization (
	neuralnet_t* nn, vec_t* dJdW, vec_t* dJdB,
	double learning_rate, int layer, int iteration,
	int optimization_code
)
{
	int line = __LINE__ - 6;

	if (nn == NULL || dJdW == NULL || dJdB == NULL)
	{
		ut_errmsg (
			"Some pointer is NULL.",
			__FILE__, line, 1
		);
	}

	if (layer < 0 || layer >= nn->nlayers-1)
	{
		ut_errmsg (
			"Invalid index for layer.",
			__FILE__, line, 1
		);
	}

	if (optimization_code == NN_NO_OPTIMIZATION)
	{
		// Updating weights for current layer
		vec_mult_scalar(dJdW, learning_rate);
		vec_sub(nn->W[layer], dJdW, nn->W[layer]);

		// Updating biases for current layer
		vec_mult_scalar(dJdB, learning_rate);
		vec_sub(nn->B[layer], dJdB, nn->B[layer]);
	}
	else if (optimization_code == NN_MOMENTUM_OPTIMIZATION)
	{
		vec_t *alphaVdW, *alphaVdB;

		// Updating weights and biases for current layer
		//-----------------------------------------------
		// newVdW = beta1*VdW + (1-beta1)*dJdW
		// newVdB = beta1*VdB + (1-beta1)*dJdB
		// newW   = W - alpha*newVdW
		// newB   = B - alpha*newVdB

		vec_mult_scalar(nn->VdW[layer], nn->momentum_rate);
		vec_mult_scalar(dJdW, 1-nn->momentum_rate);
		vec_add(nn->VdW[layer], dJdW, nn->VdW[layer]);
		alphaVdW = vec_get_scalar_prod(nn->VdW[layer], learning_rate);
		vec_sub(nn->W[layer], alphaVdW, nn->W[layer]);

		vec_free(&alphaVdW);

		vec_mult_scalar(nn->VdB[layer], nn->momentum_rate);
		vec_mult_scalar(dJdB, 1-nn->momentum_rate);
		vec_add(nn->VdB[layer], dJdB, nn->VdB[layer]);
		alphaVdB = vec_get_scalar_prod(nn->VdB[layer], learning_rate);
		vec_sub(nn->B[layer], alphaVdB, nn->B[layer]);

		vec_free(&alphaVdB);
	}
	else if (optimization_code == NN_RMS_OPTIMIZATION)
	{
		vec_t *dJdWsquared, *dJdBsquared;
		vec_t *newSdW, *newSdB; 

		//------------------
		// Updating weights
		// -----------------                      element wise square...
		// newSdW = beta2*SdW + (1-beta2)*(dJdW^2)
		// newW = W - alpha*(dJdW/(sqrt(newSdW)+epsilon))
		vec_mult_scalar(nn->SdW[layer], nn->rms_rate);
		dJdWsquared = vec_apply_out(dJdW, vec_square_op);
		vec_mult_scalar(dJdWsquared, 1-nn->rms_rate);
		newSdW = vec_get_sum(nn->SdW[layer], dJdWsquared);
		vec_copy(nn->SdW[layer], newSdW);
		vec_apply(newSdW, vec_sqrt_op);
		vec_add_scalar(newSdW, NN_EPSILON);
		vec_div_elwise(dJdW, newSdW, dJdW);
		vec_mult_scalar(dJdW, learning_rate);
		vec_sub(nn->W[layer], dJdW, nn->W[layer]);

		//Freeing memory
		vec_free(&dJdWsquared);
		vec_free(&newSdW);

		//-----------------
		// Updating biases
		// ----------------                       element wise square...
		// newSdB = beta2*SdB + (1-beta2)*(dJdB^2)
		// newB = B - alpha*(dJdB/(sqrt(newSdB)+epsilon))
		vec_mult_scalar(nn->SdB[layer], nn->rms_rate);
		dJdBsquared = vec_apply_out(dJdB, vec_square_op);
		vec_mult_scalar(dJdBsquared, 1-nn->rms_rate);
		newSdB = vec_get_sum(nn->SdB[layer], dJdBsquared);
		vec_copy(nn->SdB[layer], newSdB);
		vec_apply(newSdB, vec_sqrt_op);
		vec_add_scalar(newSdB, NN_EPSILON);
		vec_div_elwise(dJdB, newSdB, dJdB);
		vec_mult_scalar(dJdB, learning_rate);
		vec_sub(nn->B[layer], dJdB, nn->B[layer]);

		//Freeing memory
		vec_free(&dJdBsquared);
		vec_free(&newSdB);
	}
	else if (optimization_code == NN_ADAM_OPTIMIZATION)
	{
		vec_t *dJdWsquared, *dJdBsquared;
		vec_t *newSdW, *newSdB;
		int t = iteration;

		float alpha = learning_rate;
		float beta1 = nn->momentum_rate;
		float beta2 = nn->rms_rate;

		//------------------
		// Updating weights
		// -----------------
		// newVdW = beta1*VdW + (1-beta1)*dJdW
		// newSdW = beta2*SdW + (1-beta2)*(dJdW^2)
		// Bias correction (VdW := VdW/(1-beta1^t)), SdW...
		// W = W - alpha*(VdW/(sqrt(SdW)+epsilon))

		// Calculate dJdW^2 before losing dJdW...
		dJdWsquared = vec_apply_out(dJdW, vec_square_op);

		vec_mult_scalar(nn->VdW[layer], beta1);
		vec_mult_scalar(dJdW, 1.0-beta1); // lost dJdW...
		vec_add(nn->VdW[layer], dJdW, nn->VdW[layer]);

		vec_mult_scalar(nn->SdW[layer], beta2);
		vec_mult_scalar(dJdWsquared, 1.0-beta2);
		newSdW = vec_get_sum(nn->SdW[layer], dJdWsquared);
		vec_copy(nn->SdW[layer], newSdW);

		// NOT WORKING...
		// vec_mult_scalar(nn->VdW[layer], (vec_type_t) 1.0/(1.0-pow(beta1,t)));
		// vec_mult_scalar(nn->SdW[layer], (vec_type_t) 1.0/(1.0-pow(beta2,t)));

		// We'll use dJdW as auxiliar matrix
		vec_apply(newSdW, vec_sqrt_op);
		vec_add_scalar(newSdW, NN_EPSILON);
		vec_div_elwise(nn->VdW[layer], newSdW, dJdW);
		vec_mult_scalar(dJdW, alpha);
		vec_sub(nn->W[layer], dJdW, nn->W[layer]);

		vec_free(&dJdWsquared);
		vec_free(&newSdW);

		//------------------
		// Updating biases
		// -----------------
		// newVdB = beta1*VdB + (1-beta1)*dJdB
		// newSdB = beta2*SdB + (1-beta2)*(dJdB^2)
		// Bias correction (VdB := VdB/(1-beta1^t)), SdB...
		// B = B - alpha*(VdB/(sqrt(SdB)+epsilon))

		dJdBsquared = vec_apply_out(dJdB, vec_square_op);

		vec_mult_scalar(nn->VdB[layer], beta1);
		vec_mult_scalar(dJdB, 1.0-beta1);
		vec_add(nn->VdB[layer], dJdB, nn->VdB[layer]);

		vec_mult_scalar(nn->SdB[layer], beta2);
		dJdBsquared = vec_apply_out(dJdB, vec_square_op);
		vec_mult_scalar(dJdBsquared, 1.0-beta2);
		newSdB = vec_get_sum(nn->SdB[layer], dJdBsquared);
		vec_copy(nn->SdB[layer], newSdB);

		// NOT WORKING...
		// vec_mult_scalar(nn->VdB[layer], (vec_type_t) 1.0/(1.0-pow(beta1,t)));
		// vec_mult_scalar(nn->SdB[layer], (vec_type_t) 1.0/(1.0-pow(beta2,t)));

		// We'll use dJdW as auxiliar matrix
		vec_apply(newSdB, vec_sqrt_op);
		vec_add_scalar(newSdB, NN_EPSILON);
		vec_div_elwise(nn->VdB[layer], newSdB, dJdB);
		vec_mult_scalar(dJdB, alpha);
		vec_sub(nn->B[layer], dJdB, nn->B[layer]);

		vec_free(&dJdBsquared);
		vec_free(&newSdB);
	}
}



vec_type_t nn_activation_func (vec_type_t k, activation_t func, int flag)
{
	vec_type_t result;

	if (flag == NN_IDENTITY_ACTIVATION)
		result = nn_identity(k);

	else if (flag == NN_RELU_ACTIVATION)
		result = nn_relu(k);
	
	else if (flag == NN_SIGMOID_ACTIVATION)
		result = nn_sigmoid(k);
	
	else if (flag == NN_HYPERBOLIC_TANGENT_ACTIVATION)
		result = nn_hyperbolic_tangent(k);

	return (vec_type_t) result;
}

vec_type_t nn_activation_func_prime (vec_type_t k, activation_t func, int flag)
{
	vec_type_t result;

	if (flag == NN_IDENTITY_ACTIVATION)
		result = nn_identity_prime(k);
	
	else if (flag == NN_RELU_ACTIVATION)
		result = nn_relu_prime(k);
	
	else if (flag == NN_SIGMOID_ACTIVATION)
		result = nn_sigmoid_prime(k);
	
	else if (flag == NN_HYPERBOLIC_TANGENT_ACTIVATION)
		result = nn_hyperbolic_tangent_prime(k);

	return (vec_type_t) result;
}



activation_t nn_get_activation (int flag)
{
	activation_t activation;

	if (flag == NN_IDENTITY_ACTIVATION)
		activation = nn_identity;
	
	else if (flag == NN_RELU_ACTIVATION)
		activation = nn_relu;
	
	else if (flag == NN_SIGMOID_ACTIVATION)
		activation = nn_sigmoid;
	
	else if (flag == NN_HYPERBOLIC_TANGENT_ACTIVATION)
		activation = nn_hyperbolic_tangent;

	return activation;
}

activation_t nn_get_activation_prime (int flag)
{
	activation_t activation_prime;

	if (flag == NN_IDENTITY_ACTIVATION)
		activation_prime = nn_identity_prime;
	
	else if (flag == NN_RELU_ACTIVATION)
		activation_prime = nn_relu_prime;
	
	else if (flag == NN_SIGMOID_ACTIVATION)
		activation_prime = nn_sigmoid_prime;
	
	else if (flag == NN_HYPERBOLIC_TANGENT_ACTIVATION)
		activation_prime = nn_hyperbolic_tangent_prime;

	return activation_prime;
}



vec_t* nn_apply_activation (
	neuralnet_t* nn, int layer
)
{
	int line = __LINE__ - 2;

	if (nn == NULL)
	{
		ut_errmsg (
			"The neuralnet_t pointer is NULL.",
			__FILE__, line, 1
		);
	}

	if (layer < 0 || layer >= nn->nlayers-1)
	{
		ut_errmsg (
			"Invalid index for layer.",
			__FILE__, line, 1
		);
	}

	if (nn->Z[layer] == NULL)
	{
		ut_errmsg (
			"nn->Z[layer] is NULL.",
			__FILE__, line, 1
		);
	}

	vec_t* output;
	int activation_code = nn->activations[layer];

	if (activation_code == NN_SOFTMAX_ACTIVATION)
	{
		output = nn_softmax_of_layer(nn,layer);
	}
	else 
	{
		output = vec_new(nn->Z[layer]->m,nn->Z[layer]->n);
		vec_apply_to(
			output, nn->Z[layer],
			nn_get_activation(nn->activations[layer])
		);
	}

	return output;
}

vec_t* nn_apply_activation_prime (
	neuralnet_t* nn, int layer
)
{
	int line = __LINE__ - 4;

	if (nn == NULL)
	{
		ut_errmsg (
			"The neuralnet_t pointer is NULL.",
			__FILE__, line, 1
		);
	}

	if (layer < 0 || layer >= nn->nlayers-1)
	{
		ut_errmsg (
			"Invalid index for layer.",
			__FILE__, line, 1
		);
	}

	if (nn->Z[layer] == NULL)
	{
		ut_errmsg (
			"nn->Z[layer] is NULL.",
			__FILE__, line, 1
		);
	}

	vec_t* output;
	int activation_code = nn->activations[layer];

	if (activation_code == NN_SOFTMAX_ACTIVATION)
	{
		output = nn_softmax_prime_of_layer(nn,layer);
	}
	else 
	{
		output = vec_new(nn->Z[layer]->m,nn->Z[layer]->n);
		vec_apply_to(
			output, nn->Z[layer],
			nn_get_activation_prime(nn->activations[layer])
		);
	}

	return output;
}



// IDENTITY ACTIVATION ------------------------------------
vec_type_t nn_identity (vec_type_t k)
{
	double x = (double) k;
	return fmax(0.0,x);
}

vec_type_t nn_identity_prime (vec_type_t k)
{
	double x = (double) k;
	double y = 1.0;
	return (vec_type_t) y;
}
//---------------------------------------------------------



// ReLU ACTIVATION ----------------------------------------
const double RELU_THRESHOLD = 10;

vec_type_t nn_relu (vec_type_t k)
{
	double x = (double) k;
	double y = k > 0 ? k : 0;

	// Trying to clip...
	// if (y > RELU_THRESHOLD)
	// 	y = RELU_THRESHOLD;

	return (vec_type_t) y;
}

vec_type_t nn_relu_prime (vec_type_t k)
{
	double x = (double) k;
	double y = k > 0 ? 1 : 0;
	
	return (vec_type_t) y;
}
//---------------------------------------------------------



// SIGMOID ACTIVATION -------------------------------------
vec_type_t nn_sigmoid (vec_type_t k)
{
	double x = (double) k;
	double y = 1/(1+exp(-x));
	return (vec_type_t) y;
}

vec_type_t nn_sigmoid_prime (vec_type_t k)
{
	// double x = (double) k;
	// double y = exp(-x) / pow(1+exp(-x),2);

	// A little bit more efficient...
	vec_type_t sig = nn_sigmoid(k);
	vec_type_t y   = sig * (1.0-sig);
	
	return (vec_type_t) y;
}
//---------------------------------------------------------



// HYPERBOLIC TANGENT ACTIVATION --------------------------
vec_type_t nn_hyperbolic_tangent (vec_type_t k)
{
	double x = (double) k;
	// double y = (exp(x)-exp(-x))/(exp(x)+exp(-x));

	double expx    = exp(x);
	double expnegx = exp(-x);
	
	// A little bit more efficient...
	double y = (expx - expnegx) / (expx + expnegx);

	return (vec_type_t) y;
}

vec_type_t nn_hyperbolic_tangent_prime (vec_type_t k)
{
	double x = (double) k;
	double y = 1 - pow(tanh(x),2);
	return (vec_type_t) y;
}
//---------------------------------------------------------



vec_t* nn_softmax_of_layer (
    neuralnet_t* nn, int layer
)
{
	int line = __LINE__ - 4;

	if (nn == NULL)
	{
		ut_errmsg (
			"The neuralnet_t pointer is NULL.",
			__FILE__, line, 1
		);
	}

	if (layer < 0 || layer >= nn->nlayers-1)
	{
		ut_errmsg (
			"Invalid index for layer.",
			__FILE__, line, 1
		);
	}

	if (nn->Z[layer] == NULL)
	{
		ut_errmsg (
			"nn->Z[layer] is NULL.",
			__FILE__, line, 1
		);
	}

	int i, j;

	// Reference: "https://deepnotes.io/softmax-crossentropy"
	//-------------------------------------------------------
	// def stable_softmax(X):
    // 	 exps = np.exp(X - np.max(X))
    // 	 return exps / np.sum(exps)
	//-------------------------------------------------------

	// TODO

	// We will create a vector with exp sums...
	vec_t* output = vec_clone(nn->Z[layer]);
	vec_apply(output,vec_exp_op);
	vec_t* sums = vec_get_rows_sums(output);

	for (i = 0; i < output->m; i++)
	{
		for (j = 0; j < output->n; j++)
		{
			vec_type_t elem = vec_get(output, i,j);
			vec_type_t sum  = vec_get(sums, i,0);
			vec_set(output, i,j, elem/sum);
		}
	}

	vec_free(&sums);

	return output;
}

vec_t* nn_softmax_prime_of_layer (
    neuralnet_t* nn, int layer
)
{
	int line = __LINE__ - 4;

	if (nn == NULL)
	{
		ut_errmsg (
			"The neuralnet_t pointer is NULL.",
			__FILE__, line, 1
		);
	}

	if (layer < 0 || layer >= nn->nlayers-1)
	{
		ut_errmsg (
			"Invalid index for layer.",
			__FILE__, line, 1
		);
	}

	if (nn->Z[layer] == NULL)
	{
		ut_errmsg (
			"nn->Z[layer] is NULL.",
			__FILE__, line, 1
		);
	}

	int i, j;

	// We will create a vector with exp sums...
	vec_t* output = vec_clone(nn->Z[layer]);
	vec_apply(output,vec_exp_op);
	vec_t* sums = vec_get_rows_sums(output);

	for (i = 0; i < output->m; i++)
	{
		for (j = 0; j < output->n; j++)
		{
			vec_type_t elem  = vec_get(output, i,j);
			vec_type_t sum   = vec_get(sums, i,0);
			vec_type_t deriv = (elem * (sum-elem))/(sum*sum);
			vec_set(output, i,j, deriv);
		}
	}

	vec_free(&sums);

	return output;
}



vec_type_t nn_regularization_term (neuralnet_t* nn)
{
	int i;
	vec_type_t sum    = 0.0;
	vec_type_t result = 0.0;
	vec_t* wsquared   = NULL;

	if (nn->regularization == NN_L2_REGULARIZATION)
	{
		for (i = 0; i < nn->nlayers-1; i++)
		{
			wsquared = vec_apply_out(nn->W[i], vec_square_op);
			sum += vec_inner_sum(wsquared);
			vec_free(&wsquared);
		}

		sum *= nn->regularization_rate;
	}

	return result;
}
