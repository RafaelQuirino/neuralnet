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



#define NET_TRAIN_MODE 0
#define NET_TEST_MODE  1



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



    // // Testing linalg.h
    // vec_t* A = vec_new(1,5);
    // vec_t* B = vec_new(5,4);
    // vec_set_all(A,1);
    // vec_set_all(B,3);
    // vec_print(A);
    // vec_print(B);

    // vec_t* AB = vec_get_dot(A,B);
    // vec_print(AB);

    // vec_set_row(B, 0, 1);
    // vec_set_row(B, 1, 2);
    // vec_set_row(B, 2, 3);
    // vec_set_row(B, 3, 4);
    // vec_set_row(B, 4, 5);
    // vec_print(B);

    // vec_t* C = vec_clone_portion(B, 1, 3);
    // vec_print(C);

    // vec_t* D = vec_clone_portion_circ(B, 2, 9);
    // vec_print(D);



    dataset_t* dataset = dat_get_dataset_from_representation_1(file, labels);


    //-------------------------------------------------------------------------
	// Testing neural_net.h
	//-------------------------------------------------------------------------
    int num_layers = 5;
    vec_type_t learning_rate = 0.0001; //will be overwriten...

	if (mode == NET_TRAIN_MODE)
	{
		fprintf(stderr, "\nNET_TRAIN_MODE\n\n");

		neuralnet_t* nn;
		int layer_size = dataset->X->n;

		// Build neuralnet_t* --------------------------------------
		if (use_net == 0)
		{
			int topology[num_layers];
			for (i = 0; i < num_layers; i++)
            {
				topology[i] = i == num_layers-1 ? 2 : layer_size;
            }
            // topology[1] = 512;
            // topology[2] = 256;
            // topology[3] = 128;

			nn = nn_new(topology, num_layers);
		}
		else
		{
			nn = nn_import(net_file);
		}

		// Print topology
		fprintf(stderr, "\nnlayers : %d\ntopology: _", nn->nlayers);
		for (i = 0; i < nn->nlayers; i++)
			fprintf(stderr, "%d_", nn->topology[i]);
		fprintf(stderr, "\n\n");
		//-----------------------------------------------------------

		// Run backpropagation
		// --------------------
		// nn_backpropagation(
        //     nn, dataset->X, dataset->Y, iterations, learning_rate
        // );

        dat_shuffle(dataset);
        nn_backpropagation_sgd(
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

		// Print topology
		fprintf(stderr, "\nnlayers : %d\ntopology: _", nn->nlayers);
		for (i = 0; i < nn->nlayers; i++)
			fprintf(stderr, "%d_", nn->topology[i]);
		fprintf(stderr, "\n\n");

		// Feed data forward
		vec_t* output = nn_feed_forward(nn, dataset->X);

        // Print estimations for class 1
		for (i = 0; i < output->m; i++) {
			printf("%g", vec_get(output,i,0));
			printf("\n");
		}
	}
	//-------------------------------------------------------------------------



    return 0;
}