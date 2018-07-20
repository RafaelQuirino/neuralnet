#include "../include/neuralnet.h"



neuralnet_t* nn_new (int topology[], int tsize, double (*func)())
{
	// Obs.: "func" is a pointer to a function
	// that generates some value (like gaussian random numbers...).
	// If it is set to NULL, all weights are going to be set to 1.0. 

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

	// Building weight matrices...
	newnet->W = NULL;
	newnet->W = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));

	if (!newnet->W)
	{
		ut_errmsg (
			"Couldnot allocate memory for the weight matrices.",
			__FILE__, line, 1
		);
	}

	for (i = 0; i < tsize-1; i++) 
	{
		newnet->W[i] = vec_new(topology[i],topology[i+1]);
		if (func == NULL)
			vec_set_all(newnet->W[i], 1.0);
		else
			vec_set_all_func(newnet->W[i], func);
	}

	// Building bias matrices (1 x n vectors)...
	newnet->B = NULL;
	newnet->B = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));

	if (!newnet->B)
	{
		ut_errmsg (
			"Couldnot allocate memory for the bias matrices.",
			__FILE__, line, 1
		);
	}

	for (i = 0; i < tsize-1; i++) 
	{
		newnet->B[i] = vec_new(1,topology[i+1]);
		if (func == NULL)
			vec_set_all(newnet->B[i], 1.0);
		else
			vec_set_all_func(newnet->B[i], func);
	}

	// Building activity (Z) and activation (A) vecrices...
	newnet->Z = NULL;
	newnet->Z = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));

	if (!newnet->Z)
	{
		ut_errmsg (
			"Couldnot allocate memory for the activity matrices.",
			__FILE__, line, 1
		);
	}

	newnet->A = NULL;
	newnet->A = (vec_t**) malloc((tsize-1) * sizeof(vec_t*));

	if (!newnet->A)
	{
		ut_errmsg (
			"Could not allocate memory for the activation matrices.",
			__FILE__, line, 1
		);
	}

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

	for (i = 0; i < tsize-1; i++) 
	{
		newnet->VdW[i] = vec_new(topology[i],topology[i+1]);
		vec_set_all(newnet->VdW[i], 0.0);
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
		newnet->VdB[i] = vec_new(1, topology[i+1]);
		vec_set_all(newnet->VdB[i], 0.0);
	}

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

	for (i = 0; i < (tsize-1)-1; i++)
	{
		// newnet->activations[i] = NN_IDENTITY_ACTIVATION;
		// newnet->activations[i] = NN_RELU_ACTIVATION;
		newnet->activations[i] = NN_SIGMOID_ACTIVATION;
		// newnet->activations[i] = NN_HYPERBOLIC_TANGENT_ACTIVATION;
	}
	// Output layer's activation
	newnet->activations[(tsize-1)-1] = NN_SIGMOID_ACTIVATION;

	// newnet->cost_function = NN_SQUARE_ERROR;
	newnet->cost_function = NN_HALF_SQUARE_ERROR;
	// newnet->cost_function = NN_MEAN_SQUARE_ERROR;
	// newnet->cost_function = NN_CROSS_ENTROPY;

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
	}

	vec_free(&(*nn)->yHat);

	free((*nn)->W);
	free((*nn)->B);
	free((*nn)->Z);
	free((*nn)->A);
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

	   - (#neurons_layer1 * #neurons_layer2) 64 bit doubles : 1st weight matrix
	   - (              1 * #neurons_layer2) 64 bit doubles : 1st bias   vector
	   - (#neurons_layer2 * #neurons_layer3) 64 bit doubles : 2st weight matrix
	   - (              1 * #neurons_layer3) 64 bit doubles : 2st bias   vector
	   ...
	   - (#neurons_layer(N-1) * #neurons_layerN) 64 bit double : (N-1)th weight matrix
	   - (                  1 * #neurons_layerN) 64 bit double : (N-1)th bias   vector
*/
void nn_export (neuralnet_t* nn, const char* fname)
{
	// Variables
	int i, line;
	size_t res;

	// If i want to change fname later...
	const char* namebuff = fname;

	FILE* fp = fopen(namebuff, "wb");

	// First, fwrite the number of layers
	res = fwrite(&nn->nlayers, sizeof(int), 1, fp);
	line = __LINE__ - 1;
	
	if (res != 1) 
	{
		ut_errmsg (
			"Couldnot fwrite nn->laers into file.",
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

	// Then, for each layer, fwrite the weights linearly, row-wise, and biases
	for (i = 0; i < nn->nlayers-1; i++)
	{
		res = fwrite(nn->W[i]->vec, sizeof(double), nn->W[i]->size, fp);
		line = __LINE__ - 1;
		
		if (res != nn->W[i]->size) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fwrite nn->W[%d]->vec into file.", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}

		res = fwrite(nn->B[i]->vec, sizeof(double), nn->B[i]->size, fp);
		line = __LINE__ - 1;

		if (res != nn->B[i]->size) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fwrite nn->B[%d]->vec into file.", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}		
	}

	fclose(fp);
}



neuralnet_t* nn_import (const char* fname)
{
	// Variables
	int i, numlayers, line;
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

	// Create new neural_net_t*
	neuralnet_t* nn = nn_new(topology, numlayers, NULL);

	for (i = 0; i < numlayers-1; i++)
	{
		// Read weights
		int numweights = topology[i] * topology[i+1];
		res = fread(nn->W[i]->vec, sizeof(double), numweights, fp);
		line = __LINE__ - 1;

		if (res != numweights) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fread weights of W[%d].", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}

		// Read biases
		int numbiases = 1 * topology[i+1];
		res = fread(nn->B[i]->vec, sizeof(double), numbiases, fp);
		line = __LINE__ - 1;

		if (res != numbiases) 
		{
			char msgbuf[128];
			sprintf(msgbuf, "Couldnot fread biases of B[%d].", i);
			ut_errmsg (
				msgbuf,
				__FILE__, line, 1
			);
		}		
	}

	fclose(fp);

	return nn;
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

		// Last, make A[i] = fact(Z[i]),
		// where fact is the activation function
		// nn->A[i] = vec_apply_out(nn->Z[i], nn_sigmoid);
		nn->A[i] = vec_apply_out(
			nn->Z[i], 
			nn_get_activation(nn->activations[i])
		);

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

		// Last, make Z[i] = fact(Z[i]),
		// where fact is the activation function
		// vec_apply_to (nn->Z[i], nn->Z[i], nn_sigmoid);
		vec_apply_to (
			nn->Z[i], nn->Z[i], 
			nn_get_activation(nn->activations[i])
		);

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



// vec_type_t nn_cost_func (neuralnet_t* nn, vec_t* X, vec_t* Y)
double nn_cost_func (
	neuralnet_t* nn, vec_t* y, vec_t* yHat,
	int funcflag
)
{
	// vec_t* yHat = nn_forward(nn,X);
	
	// // (Y - yHat) ^ 2
	// vec_t* Y_yHat = vec_get_diff(Y,yHat);
	// vec_free(&yHat);
	// vec_apply(Y_yHat, vec_square_op);
	// double cost = 0.5 * vec_inner_sum(Y_yHat);
	// vec_free(&Y_yHat);

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
	else if (funcflag == NN_CROSS_ENTROPY)
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
		vec_free(&ylnyHat);
	}
	
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
	// J = -1/n * Sum()
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
	// // (y - yHat) ^ 2
	// vec_t* y_yHat_tmp = vec_get_diff(Y,nn->yHat);
	// vec_apply(y_yHat_tmp, vec_square_op);
	// double cost = 0.5 * vec_inner_sum(y_yHat_tmp);
	// vec_free(&y_yHat_tmp);

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
	vec_t* sigPrime_z = NULL;
	vec_t* delta      = NULL;
	vec_t* Wt         = NULL;
	
	
	
	// COST FUNCTION DERIVATIVE ---------------------------
	// First error signal, -(y - yHat),
	// derivative of half squared error
	// vec_t* _y_yHat = vec_get_diff(Y,nn->yHat);
	// vec_mult_scalar(_y_yHat,-1);

	// Cost function derivative goes here
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
		// Sigmoid prime of Z f'act(Z[i])
		// sigPrime_z = vec_apply_out(nn->Z[i], nn_sigmoid_prime);
		sigPrime_z = vec_apply_out(
			nn->Z[i], nn_get_activation_prime(nn->activations[i])
		);
		if (i == 0)
			act = Xt; // First layer's activation is the data itself
		else
			act = vec_transposed(nn->A[i-1]);
		//---------------------------------------------------------------------

		//---------------------------------------------------------------------
		// Calculating and saving dJdW and dJdB
		//---------------------------------------------------------------------
		delta = vec_get_mult_elwise(errsign, sigPrime_z);
		vec_free(&sigPrime_z);
		vec_free(&errsign);

		// Weights gradient
		dJdWs[i] = vec_get_dot(act, delta);

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
	double momentum = 0.9;

	// For each iteration of backpropagation
	for (i = 0; i < num_iterations; i++) 
	{
		// Calculate gradients
		double cost = nn_cost_func_prime(nn, X, Y, &dJdW, &dJdB);
		printf("iteration: %d, cost: %g\n", i+1, cost);		

		// For each layer in the neural net
		for (j = 0; j < nn->nlayers-1; j++)
		{
			//=================================================================
			// FIRST WAY, WITHOUT OPTIMIZATION
			//=================================================================
			if (FALSE)
			{
				// Updating weights for current layer
				vec_mult_scalar(dJdW[j], learning_rate);
				vec_sub(nn->W[j], dJdW[j], nn->W[j]);
				vec_free(&dJdW[j]);

				// Updating biases for current layer
				vec_mult_scalar(dJdB[j], learning_rate);
				vec_sub(nn->B[j], dJdB[j], nn->B[j]);
				vec_free(&dJdB[j]);
			}
			//=================================================================

			//=================================================================
			// SECOND WAY, WITH MOMENTUM LEARNING
			//=================================================================
			if (FALSE)
			{
				// Updating weights and biases for current layer
				// newVdW = momentum*VdW + (1-momentum)*dJdW
				// newVdB = momentum*VdB + (1-momentum)*dJdB
				// newW   = W - learning_rate*newVdW
				// newB   = B - learning_rate*newVdB

				vec_mult_scalar(nn->VdW[j], momentum);
				vec_mult_scalar(dJdW[j], 1-momentum);
				vec_add(nn->VdW[j], dJdW[j], nn->VdW[j]);
				vec_t* lrateVdW = vec_get_scalar_prod(nn->VdW[j], learning_rate);
				vec_sub(nn->W[j], lrateVdW, nn->W[j]);
				vec_free(&lrateVdW);
				vec_free(&dJdW[j]);

				vec_mult_scalar(nn->VdB[j], momentum);
				vec_mult_scalar(dJdB[j], 1-momentum);
				vec_add(nn->VdB[j], dJdB[j], nn->VdB[j]);
				vec_t* lrateVdB = vec_get_scalar_prod(nn->VdB[j], learning_rate);
				vec_sub(nn->B[j], lrateVdB, nn->B[j]);
				vec_free(&lrateVdB);
				vec_free(&dJdB[j]);
			}
			//=================================================================

			//=================================================================
			// THIRD WAY, WITH RMS LEARNING
			//=================================================================
			if (TRUE)
			{
				//------------------
				// Updating weights
				// -----------------                      element wise square...
				// newVdW = momentum*VdW + (1-momentum)*(dJdW^2)
				// newW = W - learning_rate*(dJdW/sqrt(newVdW))
				vec_mult_scalar(nn->VdW[j], momentum);
				vec_t* dJdWsquared = vec_clone(dJdW[j]);
				vec_apply(dJdWsquared, vec_square_op);
				vec_mult_scalar(dJdWsquared, 1-momentum);
				vec_t* newVdW = vec_get_sum(nn->VdW[j], dJdWsquared);
				vec_copy(nn->VdW[j], newVdW);
				vec_apply(newVdW, vec_sqrt_op);
				vec_div_elwise(dJdW[j], newVdW, dJdW[j]);
				vec_mult_scalar(dJdW[j], learning_rate);
				vec_sub(nn->W[j], dJdW[j], nn->W[j]);

				//Freeing memory
				vec_free(&dJdWsquared);
				vec_free(&newVdW);
				vec_free(&dJdW[j]);

				//-----------------
				// Updating biases
				// ----------------                       element wise square...
				// newVdB = momentum*VdB + (1-momentum)*(dJdB^2)
				// newB = B - learning_rate*(dJdB/sqrt(newVdB))
				vec_mult_scalar(nn->VdB[j], momentum);
				vec_t* dJdBsquared = vec_clone(dJdB[j]);
				vec_apply(dJdBsquared, vec_square_op);
				vec_mult_scalar(dJdBsquared, 1-momentum);
				vec_t* newVdB = vec_get_sum(nn->VdB[j], dJdBsquared);
				vec_copy(nn->VdB[j], newVdB);
				vec_apply(newVdB, vec_sqrt_op);
				vec_div_elwise(dJdB[j], newVdB, dJdB[j]);
				vec_mult_scalar(dJdB[j], learning_rate);
				vec_sub(nn->B[j], dJdB[j], nn->B[j]);

				//Freeing memory
				vec_free(&dJdBsquared);
				vec_free(&newVdB);
				vec_free(&dJdB[j]);
			}
			//=================================================================

		} // for (j = 0; j < nn->nlayers-1; j++)
	}
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
	double momentum = 0.9;

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
		vec_t* sigPrime_z = NULL;
		vec_t* delta      = NULL;
		vec_t* Wt         = NULL;

	 	// Backpropagation loop
		for (j = nn->nlayers-2; j >= 0; j--) 
		{
			//-----------------------------------------------------------------
			// Prepare matrices 
			//-----------------------------------------------------------------
			// Sigmoid prime of Z f'act(Z[i])
			// sigPrime_z = vec_apply_out(nn->Z[j], nn_sigmoid_prime);
			sigPrime_z = vec_apply_out(
				nn->Z[j], 
				nn_get_activation_prime(nn->activations[j])
			);
			if (j == 0)
				act = Xt; // First layer's activation is the data itself
			else
				act = vec_transposed(nn->A[j-1]);
			//-----------------------------------------------------------------

			//-----------------------------------------------------------------
			// Calculating and saving dJdW and dJdB
			//-----------------------------------------------------------------
			delta = vec_get_mult_elwise(errsign, sigPrime_z);

			vec_free(&sigPrime_z);
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

			//-----------------------------------------------------------------
			// RMS LEARNING
			//-----------------------------------------------------------------
			//------------------
			// Updating weights
			// -----------------                      element wise square...
			// newVdW = momentum*VdW + (1-momentum)*(dJdW^2)
			// newW = W - learning_rate*(dJdW/sqrt(newVdW))
			vec_mult_scalar(nn->VdW[j], momentum);

			vec_t* dJdWsquared = vec_clone(dJdW);
			vec_apply(dJdWsquared, vec_square_op);
			vec_mult_scalar(dJdWsquared, 1-momentum);

			vec_t* newVdW = vec_get_sum(nn->VdW[j], dJdWsquared);
			vec_copy(nn->VdW[j], newVdW);
			vec_apply(newVdW, vec_sqrt_op);

			vec_div_elwise(dJdW, newVdW, dJdW);
			vec_mult_scalar(dJdW, learning_rate);
			vec_sub(nn->W[j], dJdW, nn->W[j]);

			//Freeing memory
			vec_free(&dJdWsquared);
			vec_free(&newVdW);
			vec_free(&dJdW);

			//-----------------
			// Updating biases
			// ----------------                       element wise square...
			// newVdB = momentum*VdB + (1-momentum)*(dJdB^2)
			// newB = B - learning_rate*(dJdB/sqrt(newVdB))
			vec_mult_scalar(nn->VdB[j], momentum);

			vec_t* dJdBsquared = vec_clone(dJdB);
			vec_apply(dJdBsquared, vec_square_op);
			vec_mult_scalar(dJdBsquared, 1-momentum);

			vec_t* newVdB = vec_get_sum(nn->VdB[j], dJdBsquared);
			vec_copy(nn->VdB[j], newVdB);
			vec_apply(newVdB, vec_sqrt_op);

			vec_div_elwise(dJdB, newVdB, dJdB);
			vec_mult_scalar(dJdB, learning_rate);
			vec_sub(nn->B[j], dJdB, nn->B[j]);

			//Freeing memory
			vec_free(&dJdBsquared);
			vec_free(&newVdB);
			vec_free(&dJdB);
			//-----------------------------------------------------------------

		} // for (j = nn->nlayers-2; j >= 0; j--)
		
		// Freeing memory
		vec_free(&errsign);

	} // for (i = 0; i < num_iterations; i++)

} // void nn_backpropagation_2



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
const double RELU_THRESHOLD = 1;

vec_type_t nn_relu (vec_type_t k)
{
	double x = (double) k;
	double y = fmax(0.0,x);

	// Trying to clip...
	if (y > RELU_THRESHOLD)
		y = RELU_THRESHOLD;

	return (vec_type_t) y;
}

vec_type_t nn_relu_prime (vec_type_t k)
{
	double x = (double) k;
	double y = k > 0 ? 1 : 0;

	// Trying to clip...
	if (y > RELU_THRESHOLD)
		y = RELU_THRESHOLD;
	
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
	double x = (double) k;
	double y = exp(-x) / pow(1+exp(-x),2);
	return (vec_type_t) y;
}
//---------------------------------------------------------



// HYPERBOLIC TANGENT ACTIVATION --------------------------
vec_type_t nn_hyperbolic_tangent (vec_type_t k)
{
	double x = (double) k;
	double y = (exp(x)-exp(-x))/(exp(x)+exp(-x));
	return (vec_type_t) y;
}

vec_type_t nn_hyperbolic_tangent_prime (vec_type_t k)
{
	double x = (double) k;
	double y = 1 - pow(tanh(x),2);
	return (vec_type_t) y;
}
//---------------------------------------------------------