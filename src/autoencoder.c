#include "../include/autoencoder.h"

autoencoder_t* ae_new (int input_size)
{
    autoencoder_t* ae;

    int topology[3] = {input_size,input_size/2,input_size};
    ae = nn_new(topology,3);
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



void ae_train (autoencoder_t* ae, dataset_t* dataset)
{

}



vec_t* ae_test (autoencoder_t* ae, dataset_t* dataset)
{
    return NULL;
}