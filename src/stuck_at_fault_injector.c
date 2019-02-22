#include "stuck_at_fault_injector.h"

void inject_stuck_at_fault_into_layers(network *net, int layer_n, int bit, int type, int fault_location) {
    // create a new stuck-at data structure
    stuck_at_fault *new_st_fault = malloc(sizeof(stuck_at_fault));
    new_st_fault->bit = bit;
    new_st_fault->type = type;
    new_st_fault->fault_location = fault_location;

    if (layer_n < 0) {
        if (net->layers[layer_n].type == CONVOLUTIONAL || net->layers[layer_n].type == CONNECTED) {
            inject_stuck_at_fault(net, layer_n, new_st_fault);
        } else {
            fprintf(stderr, "impossible to insert fault into layer %d because it is not a convolution or fully connected layer", layer_n);
        }
    } else {
        int i;
        for (i = 0; i < net->n; i++) {
            if (net->layers[i].type == CONVOLUTIONAL || net->layers[i].type == CONNECTED) {
                inject_stuck_at_fault(net, i, new_st_fault);
            }
        }   
    }
}

void inject_stuck_at_fault(network *net, int layer_n, stuck_at_fault *fault) {
    layer *l = &net->layers[layer_n];
    l->f_model = STUCK_AT_1;
    l->fault = fault;
}

float compute_faulty_multiplication(float value, int bit_position, int type) {
    int tmp, mask;
    memcpy(&tmp, &value, sizeof(float));
    mask = 0x01;
    mask <<= bit_position;
    if (type == 1) {
        tmp |= mask;
    } else {
        mask = ~mask;
        tmp &= mask;
    }
    memcpy(&value, &tmp, sizeof(float));
    return value;
}

void remove_stuck_at_fault(network *net, int layer_n) {
    if (layer_n < 0) {
        // if layer number is less than 0 we must delete the fault from all the layers
        int i;
        for (i = 0; i < net->n; i++) {
            if (net->layers[i].type == CONVOLUTIONAL || net->layers[i].type == CONNECTED) {
                // remove the fault from the network
                layer *l = &net->layers[i];
                free(l->fault);
                l->f_model = NO_FAULT;
                l->fault = NULL;
            }
        }
    } else {
        layer *l = &net->layers[layer_n];
        if (l->type == CONVOLUTIONAL || l->type == CONNECTED) {
            free(l->fault);
            l->f_model = NO_FAULT;
            l->fault = NULL; 
        } else {
            // only for debug 
            fprintf(stderr, "the layer %d is neither convolutional or fully connected", layer_n);
        }
    }
    
}
