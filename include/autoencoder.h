#ifdef __cplusplus
extern "C" {
#endif
#ifndef _AUTOENCODER_H_
#define _AUTOENCODER_H_

#include "../include/neuralnet.h"

typedef struct {

    neuralnet_t* nn;

} autoencoder_t;

autoencoder_t* ae_new ();

#endif // _AUTOENCODER_H_
#ifdef __cplusplus
}
#endif
