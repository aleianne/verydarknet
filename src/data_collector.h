#ifndef DATA_COLLECTOR_H
#define DATA_COLLECTOT_H 

#include "darknet.h"

#define LABEL_N 10
#define BIT_N 8

static int sa0_label_freq[BIT_N][LABEL_N];
static int sa1_label_freq[BIT_N][LABEL_N];

void print_network_informations(network *net);
void print_layer_n_mul(layer l, int layer_id);
void print_layer_n_weights(layer l);
void print_layer_conv_info(layer l, int layer_id);
void print_layer_fc_info(layer l, int layer_id);
void print_prediction_results(float *pred, int n);
void init_stuck_at_data_struct();
void update_sa_freq(int st_type, int bit, int label);

void write_golded_prediction_file();

#endif
