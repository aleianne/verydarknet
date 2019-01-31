#ifndef STUCK_AT_FAULT_H
#define STUCK_AT_FAULT_H

#include "darknet.h"

typedef struct stuck_at_fault_t {
    int bit;
    int type;
} stuck_at_fault;

void inject_stuck_at_fault_into_layers(network *net, int layer_n, int bit, int type);
void inject_stuck_at_fault(network *net, int layer_n, stuck_at_fault *fault);
void remove_fault(network *net, int layer_n);
float compute_faulty_multiplication(float value, int position, int type);

#endif
