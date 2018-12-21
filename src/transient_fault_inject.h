#ifndef TRANSIENT_FAULT_INJECT_H
#define TRANSIENT_FAULT_INJECT_H

#include "darknet.h"

typedef struct weight_fault_t {
    float correct_value;
    float faulty_value;
} weight_fault;

void inject_convolutional_seu(int target_layer, network *net, transition_fault *fault_i);
void save_correct_value(float *matrix, int ld, int i, int j, int bit, weight_fault *fault);
float flip_bit(int bit, float value);

#endif
