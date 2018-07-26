#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/io.h"
#include "../include/util.h"
#include "../include/text.h"
#include "../include/data.h"
#include "../include/linalg.h"
#include "../include/neuralnet.h"
#include "../include/autoencoder.h"



#define NET_TRAIN_MODE 0
#define NET_TEST_MODE  1
#define AE_TRAIN_MODE  2
#define AE_TEST_MODE   3



// HELPER FUNCTIONS

void print_topology (neuralnet_t* nn)
{
    // Print topology
    int i;
    fprintf(stderr, "\nnlayers : %d\ntopology: _", nn->nlayers);
    for (i = 0; i < nn->nlayers; i++)
        fprintf(stderr, "%d_", nn->topology[i]);
    fprintf(stderr, "\n\n");
}



int main (int argc, char** argv)
{
    // Arguments ------------------------------------------
    if (argc != 7)
    {
        fprintf(stdout, "usage: bin path_to_txt_file ");
        fprintf(stdout, "path_to_labels_file ");
        fprintf(stdout, "net_file_name ");        
        fprintf(stdout, "#_iterations ");
        fprintf(stdout, "use_net ");
        fprintf(stdout, "mode \n");
        exit(1);
    }
    
    char* file       = argv[1];
    char* labels     = argv[2];
    char* net_file   = argv[3];
    int   iterations = atoi(argv[4]);
    int   use_net    = atoi(argv[5]);
    int   mode       = atoi(argv[6]);
    //-----------------------------------------------------



    // Variables
    int i, j;
    int num_layers;
    vec_type_t learning_rate;



    dataset_t* dataset = dat_get_dataset_from_representation_1(file, labels);

    //-------------------------------------------------------------------------
	// Testing neural_net.h
	//-------------------------------------------------------------------------
    num_layers = 4;
    learning_rate = 0.001; //will be overwriten...

	if (mode == NET_TRAIN_MODE)
	{
		fprintf(stderr, "\nNET_TRAIN_MODE\n\n");

		neuralnet_t* nn;

		// Build neuralnet_t* --------------------------------------
		if (use_net == 0)
		{
			int topology[num_layers];
			for (i = 0; i < num_layers; i++)
            {
				topology[i] = i == num_layers-1 ? 2 : dataset->X->n;
            }

			nn = nn_new(topology, num_layers);
		}
		else
		{
			nn = nn_import(net_file);
		}

		print_topology(nn);
		//-----------------------------------------------------------

		// Run backpropagation
		// --------------------
        dat_shuffle(dataset);
        nn_backpropagation_sgd (
            nn, dataset, iterations, learning_rate
        );

		// Export nn and free memory
		nn_export(nn, net_file);
		nn_free(&nn);
	}

	else if (mode == NET_TEST_MODE)
	{
		fprintf(stderr, "\nNET_TEST_MODE\n\n");

		// Build neural_net_t*
		neuralnet_t* nn = nn_import(net_file);

		print_topology(nn);

		// Feed data forward
		vec_t* output = nn_feed_forward(nn, dataset->X);

        // Print estimations
		for (i = 0; i < output->m; i++) 
        {
			printf("%g %g\n", vec_get(output,i,0), vec_get(output,i,1));
		}
	}
	//-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
	// Testing neural_net.h
	//-------------------------------------------------------------------------
    else if (mode == AE_TRAIN_MODE)
    {
        fprintf(stderr, "\nAE_TRAIN_MODE\n\n");

        neuralnet_t* ae;

        if (use_net == 0)
		{
            ae = ae_new(dataset->X->n);
        }
        else
        {
            ae = nn_import(net_file);
        }

        print_topology(ae);

        // Prepare data
        vec_free(&dataset->Y);
        dataset->Y = vec_clone(dataset->X);

        // Train
        dat_shuffle(dataset);
        nn_backpropagation_sgd (
            ae, dataset, iterations, learning_rate
        );

		// Export nn and free memory
		nn_export(ae, net_file);
		nn_free(&ae);
    }
	//-------------------------------------------------------------------------



    return 0;
}