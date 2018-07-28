#ifdef __cplusplus
extern "C" {
#endif
#ifndef _AUTOENCODER_H_
#define _AUTOENCODER_H_

#include "../include/data.h"
#include "../include/linalg.h"
#include "../include/neuralnet.h"

// typedef struct {
//     neuralnet_t* nn;
// } autoencoder_t;

typedef neuralnet_t autoencoder_t;

autoencoder_t* ae_new     (int input_size);
autoencoder_t* ae_new_top (int* topology, int tsize);
void           ae_free    (autoencoder_t** ae);

void ae_add_layer        (autoencoder_t* ae, int lsize);
void ae_hidden_add_layer (autoencoder_t* ae, int lsize);
void ae_remove_layer     (autoencoder_t* ae, int lsize);

void   ae_train (autoencoder_t* ae, dataset_t* dataset);
vec_t* ae_test  (autoencoder_t* ae, dataset_t* dataset);

vec_t* ae_encode (autoencoder_t* ae, vec_t* vecs);
vec_t* ae_decode (autoencoder_t* ae, vec_t* vecs);

#endif // _AUTOENCODER_H_
#ifdef __cplusplus
}
#endif
