#ifndef NETWORK_INFORMATION_H
#define NETWORK_INFORMATION_H

#include "darknet.h"

void print_network_informations(network *net);
void print_layer_n_mul(layer l, int layer_id);
void print_layer_n_weights(layer l);
void print_layer_conv_info(layer l, int layer_id);
void print_layer_fc_info(layer l, int layer_id);

#endif
