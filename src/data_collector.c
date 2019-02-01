#include "data_collector.h"

void print_network_informations(network *net) {
     int i;
    layer l;

    printf("NETWORK INFORMATIONS\n");
    for (i = 0; i < net->n; ++i) {
        l = net->layers[i];
        print_layer_n_mul(l, i);
        print_layer_n_weights(l);
    }
    printf("\n");
}


void print_layer_n_mul(layer l, int layer_id) {
    if (l.type == CONVOLUTIONAL) {
        print_layer_conv_info(l, layer_id); 
    } else if (l.type == CONNECTED) {
        print_layer_fc_info(l, layer_id);
    } else {
        // do nothing;
    }  
}

void print_layer_n_weights(layer l) {
    if (l.type == CONVOLUTIONAL) {
        printf("WEIGHTS (total): %d\n", l.nweights);
    } else if (l.type == CONNECTED) {
        int n_weights = l.outputs * l.inputs;
        printf("WEIGHTS (total): %d\n", n_weights);
    }
}

void print_layer_conv_info(layer l, int layer_id) {
    int n_iteration = l.outputs * l.size * l.size * l.c;
    int n_filters = l.n;
    int n_weights = l.nweights / l.n;
    int n_output_neurons = l.out_w * l.out_h;
    printf("layer convolutional n: %d; MUL: %d, FILTERS: %d, WEIGHTS (per filter): %d, OUTPUTS: %d ", layer_id, n_iteration, n_filters, n_weights, n_output_neurons);
}

void print_layer_fc_info(layer l, int layer_id) {
    int n_iteration = l.outputs * l.inputs;
    printf("layer fully connected n: %d; MUL: %d, INPUTS %d, OUTPUTS %d, BATCHES %d ", layer_id, n_iteration, l.inputs, l.outputs, l.batch);
}

void print_prediction_results(float *pred, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("prediction score for label %d: %f\n", i, pred[i]);
    }
}

void init_stuck_at_data_struct() {
    int i, j;
    for (i = 0; i < BIT_N; i++) {
        for (j = 0; j < LABEL_N; i++) {
            sa0_label_freq[i][j] = 0;
            sa1_label_freq[i][j] = 0;
        }
    }
}

void update_sa_freq(int st_type, int bit,  int label) {
    if (label < 0 || label > LABEL_N) {
        return;
    }
    if (st_type == 1) {
        sa0_label_freq[bit][label]++;
    } if (st_type == 0) {
        sa1_label_freq[bit][label]++;
    }
}
