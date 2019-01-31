#ifndef STUCK_AT_FAULT_H
#define STUCK_AT_FAULT_H

#include "darknet.h"

typedef struct stuck_at_fault_t {
    int bit;
    int type;
    int fault_location;
} stuck_at_fault;

void inject_stuck_at_fault_into_layers(network *net, int layer_n, int bit, int type, int fault_location);
void inject_stuck_at_fault(network *net, int layer_n, stuck_at_fault *fault);
void remove_fault(network *net, int layer_n);
float compute_faulty_multiplication(float value, int bit_position, int type);

#endif
