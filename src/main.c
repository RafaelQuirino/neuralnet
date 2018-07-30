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



#define NET_TRAIN_MODE     0
#define NET_RUN_MODE       1
#define AE_TRAIN_MODE      2
#define AE_RUN_MODE        3
#define NET_AE_TRAIN_MODE  4
#define NET_AE_RUN_MODE    5



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

void print_config (neuralnet_t* nn)
{
    fprintf(stderr, "           rms rate : %g\n", nn->rms_rate);
    fprintf(stderr, "      momentum rate : %g\n", nn->momentum_rate);
    fprintf(stderr, "      learning rate : %g\n", nn->learning_rate);
    fprintf(stderr, "regularization rate : %g\n", nn->regularization_rate);
}



int main (int argc, char** argv)
{
    // vec_t* A = vec_new(3,3);
    // vec_set_all(A,2);
    // vec_print(A);
    // vec_type_t sum = vec_inner_sum(A);
    // printf("%g\n", sum);
    // exit(0);

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
    
    char* data       = argv[1];
    char* labels     = argv[2];
    char* net_file   = argv[3];
    char* ae_file    = argv[4];
    int   iterations = atoi(argv[5]);
    int   use_net    = atoi(argv[6]);
    int   mode       = atoi(argv[7]);

    if (strcmp(labels,"NULL") == 0)
        labels = NULL;
    //-----------------------------------------------------



    // Variables
    int i, j;
    int num_layers = 5;
    unsigned long mem, memaux, memtotal = 0;
    float nnregrate = 0.1;
    float aeregrate = 0.0001;



    fprintf(stderr, "Getting data...\n");
    dataset_t* dataset = dat_get_dataset_from_representation_1(data, labels);
    fprintf(stderr, "Number of samples: %d\n", dataset->size);
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
		// print_config(nn);
        mem = nn_get_mem(nn);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        fprintf(stderr, "Preparing data...\n");
        dat_shuffle(dataset);
        fprintf(stderr, "Done.\n\n");

        nn->regularization_rate = nnregrate;
		print_config(nn);
        nn_backpropagation_sgd (
            nn, net_file, dataset, iterations
        );

		// Export nn and free memory
		nn_export(nn, net_file);
		nn_free(&nn);

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
	}

	else if (mode == NET_RUN_MODE)
	{
		fprintf(stderr, "\nNET_RUN_MODE\n\n");

		// Build neural_net_t*
		neuralnet_t* nn = nn_import(net_file);
        fprintf(stderr, "Loaded neural network.\n");
		print_topology(nn);
		print_config(nn);
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

            // int topology[3] = {1120,840,1120};
            // ae = ae_new_top(topology,3);
        }
        else
        {
            ae = nn_import(ae_file);
        }

        fprintf(stderr, "Loaded autoencoder.\n");
        print_topology(ae);
		print_config(ae);
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
        // nn_backpropagation (
        //     ae, dataset->X, dataset->Y, iterations, 0.001
        // );
        // nn_backpropagation_sgd (
        //     ae, ae_file, dataset, iterations
        // );
        ae_train(ae, ae_file, dataset, iterations);

		// Export nn and free memory
		nn_export(ae, ae_file);
		nn_free(&ae);

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }

    else if (mode == AE_RUN_MODE)
    {
        fprintf(stderr, "\nAE_RUN_MODE\n\n");

        dat_print_text(dataset->X);
        // exit(0);

		// Build neural_net_t*
		neuralnet_t* ae = nn_import(ae_file);
        fprintf(stderr, "Loaded neural network.\n");
		print_topology(ae);
		print_config(ae);
        mem = nn_get_mem(ae);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

		// Feed data forward
        fprintf(stderr, "Running autoencoder...\n");
		vec_t* output = nn_feed_forward(ae, dataset->X);
        mem = vec_get_mem(output);
        memtotal += mem;
        fprintf(stderr, "Output size: %g MB\n\n", (double)mem/(1024.0*1024.0));

        dat_print_text(output);

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }

    else if (mode == NET_AE_TRAIN_MODE)
    {
        fprintf(stderr, "\nNET_AE_TRAIN_MODE\n\n");

        neuralnet_t*   nn;
        autoencoder_t* ae;

        ae = nn_import(ae_file);
        fprintf(stderr, "Loaded autoencoder.\n");
        print_topology(ae);
		print_config(ae);
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

            // // int topology[4] = {560,256,256,2};
            // int topology[5] = {320,256,256,256,2};

			nn = nn_new(topology, num_layers);
            fprintf(stderr, "Created new neural network.\n");
		}
		else
		{
			nn = nn_import(net_file);
            fprintf(stderr, "Loaded neural network.\n");
		}

        print_topology(nn);
		print_config(nn);
        mem = nn_get_mem(nn);
        memtotal += mem;
        fprintf(stderr, "neural network memory: %g MB\n\n", (double)mem/(1024.0*1024.0));

        fprintf(stderr, "Preparing data...\n");
        dat_shuffle(dataset);
        fprintf(stderr, "Done.\n\n");

        nn_backpropagation_sgd (
            nn, net_file, dataset, iterations
        );

		// Export nn and free memory
		nn_export(nn, net_file);
		nn_free(&nn);

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }

    else if (mode == NET_AE_RUN_MODE)
    {
        fprintf(stderr, "\nNET_AE_RUN_MODE\n\n");

        neuralnet_t*   nn;
        autoencoder_t* ae;

        ae = nn_import(ae_file);
        fprintf(stderr, "Loaded autoencoder.\n");
        print_topology(ae);
		print_config(ae);
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
		print_config(nn);
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

        fprintf(stderr, "\ntotal memory: %g MB\n\n", (double)memtotal/(1024.0*1024.0));
    }



    return 0;
}