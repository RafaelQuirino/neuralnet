#include "../include/autoencoder.h"

autoencoder_t* ae_new (int input_size)
{
    autoencoder_t* ae;

    int x = input_size;
    int topology[5] = {x, x*(3.0/4.0), x/2, x*(3.0/4.0), x};
    ae = nn_new(topology,5);

    nn_set_layer_activation(ae, 0, NN_SIGMOID_ACTIVATION);
    nn_set_layer_activation(ae, 1, NN_SIGMOID_ACTIVATION);
    nn_set_output_activation(ae, NN_SIGMOID_ACTIVATION);
    nn_set_cost_function(ae, NN_SQUARE_ERROR);

    return ae;
}



autoencoder_t* ae_new_top (int* topology, int tsize)
{
    autoencoder_t* ae;

    ae = nn_new(topology,tsize);

    nn_set_layer_activation(ae, 0, NN_SIGMOID_ACTIVATION);
    nn_set_layer_activation(ae, 1, NN_SIGMOID_ACTIVATION);
    nn_set_output_activation(ae, NN_SIGMOID_ACTIVATION);
    nn_set_cost_function(ae, NN_SQUARE_ERROR);

    return ae;
}



void ae_free (autoencoder_t** ae)
{
    nn_free(ae);
}



void ae_add_layer (autoencoder_t* ae, int lsize)
{

}



void ae_add_hidden_layer (autoencoder_t* ae, int lsize)
{
    int pos1 = ae->nlayers/2;
    int pos2 = pos1 + 1;

    vec_t* Wx = vec_new(ae->topology[pos1],lsize);
    vec_t* Wy = vec_new(lsize,ae->topology[pos2]);

    
}



void ae_remove_layer (autoencoder_t* ae, int lsize)
{

}



vec_t* ae_encode (autoencoder_t* ae, vec_t* vecs)
{
    int i, j;
    int encoder_size = (ae->nlayers/2)+1;

    int topology[encoder_size];
    for (i = 0; i < encoder_size; i++)
    {
        topology[i] = ae->topology[i];
    }

    neuralnet_t* nn = nn_new(topology,encoder_size);
    nn_set_layers_activation(nn, NN_SIGMOID_ACTIVATION);
    nn_set_cost_function(nn, NN_SQUARE_ERROR);

    for (i = 0; i < encoder_size-1; i++)
    {
        vec_copy(nn->W[i], ae->W[i]);
        vec_copy(nn->B[i], ae->B[i]);
    }
    // Don't need this...
    // nn->rms_rate            = ae->rms_rate;
    // nn->momentum_rate       = ae->momentum_rate;
    // nn->learning_rate       = ae->learning_rate;
    // nn->regularization_rate = ae->regularization_rate;
 
    vec_t* output = nn_feed_forward(nn,vecs);
    nn_free(&nn);

    return output;
}



vec_t* ae_decode (autoencoder_t* ae, vec_t* vecs)
{
    return NULL;
}



void ae_train (autoencoder_t* ae, dataset_t* dataset)
{

}



vec_t* ae_test (autoencoder_t* ae, dataset_t* dataset)
{
    return NULL;
}