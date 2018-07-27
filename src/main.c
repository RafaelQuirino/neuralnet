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



#define NET_TRAIN_MODE      0
#define NET_TEST_MODE       1
#define AE_TRAIN_MODE       2
#define AE_TEST_MODE        3
#define NET_AE_TRAIN_MODE   4
#define NET_AE_TEST_MODE    5



// HELPER FUNCTIONS

void print_topology (neuralnet_t* nn)
{
    // Print topology
    int i;
    fprintf(stderr, "topology: _");
    for (i = 0; i < nn->nlayers; i++)
        fprintf(stderr, "%d_", nn->topology[i]);
    fprintf(stderr, "\n");
}



int main (int argc, char** argv)
{
    // Arguments ------------------------------------------
    if (argc != 8)
    {
        fprintf(stdout, "usage: bin path_to_txt_file ");
        fprintf(stdout, "path_to_labels_file ");
        fprintf(stdout, "net_file_name ");        
        fprintf(stdout, "ae_file_name ");        
        fprintf(stdout, "#_iterations ");
        fprintf(stdout, "use_net ");
        fprintf(stdout, "mode \n");
        exit(1);
    }
    
    char* file       = argv[1];
    char* labels     = argv[2];
    char* net_file   = argv[3];
    char* ae_file    = argv[4];
    int   iterations = atoi(argv[5]);
    int   use_net    = atoi(argv[6]);
    int   mode       = atoi(argv[7]);
    //-----------------------------------------------------



    // Variables
    int i, j;
    int num_layers = 4;
    unsigned long mem, memaux, memtotal = 0;



    fprintf(stderr, "Getting data...\n");
    dataset_t* dataset = dat_get_dataset_from_representation_1(file, labels);
    mem = dat_get_mem(dataset);
    memtotal += mem;
    fprintf(stderr, "dataset memory: %g MB\n", (double)mem/(1024.0*1024.0));

	if (mode == NET_TRAIN_MODE)
	{
		fprintf(stderr, "\nNET_TRAIN_MODE\n\n");

		neuralnet_t* nn;

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

        fprintf(stderr, "Loaded neural network.\n");
		print_topology(nn);
        mem = nn_get_mem(nn);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        fprintf(stderr, "Preparing data...\n");
        dat_shuffle(dataset);
        fprintf(stderr, "Done.\n\n");

        nn_backpropagation_sgd (
            nn, dataset, iterations
        );

		// Export nn and free memory
		nn_export(nn, net_file);
		nn_free(&nn);

        fprintf(stderr, "total memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
	}

	else if (mode == NET_TEST_MODE)
	{
		fprintf(stderr, "\nNET_TEST_MODE\n\n");

		// Build neural_net_t*
		neuralnet_t* nn = nn_import(net_file);
        fprintf(stderr, "Loaded neural network.\n");
		print_topology(nn);
        mem = nn_get_mem(nn);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

		// Feed data forward
        fprintf(stderr, "Running neural network...\n");
		vec_t* output = nn_feed_forward(nn, dataset->X);
        mem = vec_get_mem(output);
        memtotal += mem;
        fprintf(stderr, "Output size: %g MB\n\n", (double)mem/(1024.0*1024.0));

        // Print estimations
		for (i = 0; i < output->m; i++) 
        {
			printf("%g %g\n", vec_get(output,i,0), vec_get(output,i,1));
		}

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
	}

    else if (mode == AE_TRAIN_MODE)
    {
        fprintf(stderr, "\nAE_TRAIN_MODE\n\n");

        autoencoder_t* ae;

        if (use_net == 0)
		{
            ae = ae_new(dataset->X->n);
        }
        else
        {
            ae = nn_import(ae_file);
        }

        fprintf(stderr, "Loaded autoencoder.\n");
        print_topology(ae);
        mem = nn_get_mem(ae);
        memtotal += mem;
        fprintf(stderr, "autoencoder memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        // Prepare data
        fprintf(stderr, "Preparing data...\n");
        memaux = vec_get_mem(dataset->Y);
        vec_free(&dataset->Y);
        memtotal -= memaux;
        dataset->Y = vec_clone(dataset->X);
        dat_add_noise(dataset->X);
        dat_shuffle(dataset);
        mem = dat_get_mem(dataset);
        memtotal += vec_get_mem(dataset->Y);
        fprintf(stderr, "new dataset memory: %g MB\n\n", (double)mem/(1024.0*1024.0));
        fprintf(stderr, "Done.\n\n");

        // Train
        nn_backpropagation_sgd (
            ae, dataset, iterations
        );

		// Export nn and free memory
		nn_export(ae, ae_file);
		nn_free(&ae);

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }

    else if (mode == AE_TEST_MODE)
    {
        
    }

    else if (mode == NET_AE_TRAIN_MODE)
    {
        fprintf(stderr, "\nNET_AE_TRAIN_MODE\n\n");

        neuralnet_t*   nn;
        autoencoder_t* ae;

        ae = nn_import(ae_file);
        fprintf(stderr, "Loaded autoencoder.\n");
        print_topology(ae);
        mem = nn_get_mem(ae);
        memtotal += mem;
        fprintf(stderr, "autoencoder memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        // Encoding data with the autoencoder
        fprintf(stderr, "Encoding input data...\n");
        vec_t* vec_codes = ae_encode(ae, dataset->X);
        memaux = vec_get_mem(dataset->X);
        vec_free(&dataset->X);
        memtotal -= memaux;
        dataset->X = vec_codes;
        memtotal += vec_get_mem(dataset->X);
        fprintf(stderr, "Done.\n\n");

		if (use_net == 0)
		{
			int topology[num_layers];
			for (i = 0; i < num_layers; i++)
            {
				topology[i] = i == num_layers-1 ? 2 : dataset->X->n;
            }

			nn = nn_new(topology, num_layers);
            fprintf(stderr, "Created new neural network.\n");
		}
		else
		{
			nn = nn_import(net_file);
            fprintf(stderr, "Loaded neural network.\n");
		}

        print_topology(nn);
        mem = nn_get_mem(nn);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        fprintf(stderr, "Preparing data...\n");
        dat_shuffle(dataset);
        fprintf(stderr, "Done.\n\n");

        nn_backpropagation_sgd (
            nn, dataset, iterations
        );

		// Export nn and free memory
		nn_export(nn, net_file);
		nn_free(&nn);

        fprintf(stderr, "total memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }

    else if (mode == NET_AE_TEST_MODE)
    {
        fprintf(stderr, "\nNET_AE_TEST_MODE\n\n");

        neuralnet_t*   nn;
        autoencoder_t* ae;

        ae = nn_import(ae_file);
        fprintf(stderr, "Loaded autoencoder.\n");
        print_topology(ae);
        mem = nn_get_mem(ae);
        memtotal += mem;
        fprintf(stderr, "autoencoder memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        // Encoding data with the autoencoder
        fprintf(stderr, "Encoding input data...\n");
        vec_t* vec_codes = ae_encode(ae, dataset->X);
        memaux = vec_get_mem(dataset->X);
        vec_free(&dataset->X);
        memtotal -= memaux;
        dataset->X = vec_codes;
        memtotal += vec_get_mem(dataset->X);
        fprintf(stderr, "Done.\n\n");

		// Build neural_net_t*
		nn = nn_import(net_file);
        fprintf(stderr, "Loaded neural network.\n");
		print_topology(nn);
        mem = nn_get_mem(nn);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

		// Feed data forward
        fprintf(stderr, "Running neural network...\n");
		vec_t* output = nn_feed_forward(nn, dataset->X);
        fprintf(stderr, "Done.\n\n");

        // Print estimations
		for (i = 0; i < output->m; i++) 
        {
			printf("%g %g\n", vec_get(output,i,0), vec_get(output,i,1));
		}

        fprintf(stderr, "total memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }



    return 0;
}