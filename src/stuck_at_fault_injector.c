#include "stuck_at_fault_injector.h"

void inject_stuck_at_fault_into_layers(network *net, int layer_n, int bit, int type) {
    // create a new stuck-at data structure
    stuck_at_fault *new_st_fault = malloc(sizeof(stuck_at_fault));
    new_st_fault->bit = bit;
    new_st_fault->type = type;

    if (layer_n < net->n) {
        inject_stuck_at_fault(net, layer_n, new_st_fault);
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
    // set the faulty flag to 1 and inject the fault
    l->f_model = STUCK_AT;
    l->fault = fault;
}

float compute_faulty_multiplication(float value, int position, int type) {
    int tmp, mask;
    memcpy(&tmp, &value, sizeof(float));
    if (type == 1) {
        //printf("DEBUG: the value before the stuck at is %d\n", tmp);
        mask = 0x01;
        mask <<= position;
        tmp |= mask;
        //printf("DEBUG: the value after the stuck at is %d\n", tmp);
    } else {
        //printf("the stuck at position is %d\n", position);
        //printf("DEBUG: the value before the stuck at is %d\n", tmp);
        mask = 0x01;
        mask <<= position;
        mask = ~mask;
        tmp &= mask;
        //printf("DEBUG: the value after the stuck at is %d\n", tmp);
    }
    memcpy(&value, &tmp, sizeof(float));
    return value;
}

void remove_fault(network *net, int layer_n) {
    // in questo caso il fault viene levato per un solo layer
    // bisogna prevedere anche il caso in cui inietto su tutti i layer
    layer *l = &net->layers[layer_n];
    free(l->fault);
    l->f_model = NO_FAULT;
    l->fault = NULL;
}
