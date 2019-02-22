#ifndef TRANSITION_INJECTOR_H
#define TRANSITION_INJECTOR_H

#include "darknet.h"

typedef struct transition_fault_t {
    int bit;
    int filter;
    int weight;
    int output_neuron;
} transition_fault;

typedef struct fc_transition_fault_t {
    int bit;
    int batch;
    int input_neuron;
    int output_neuron;
} fc_transition_fault;

typedef struct fault_value_t {
    float previous_value;
    float actual_value;
} fault_value;

fault_value delete_correct_value(float *B, int out_neuron, int weight, int ldb);
float inject_mul_fault(fault_value f, float *C, int ldc, int weight, int filter, int bit);

void create_new_fault(network *net,  transition_fault *fault_i, int target_layer);
void create_new_fc_fault(network *net, fc_transition_fault *fault_i, int target_layer);

float check_bit_on_fault_transition(int shift, float old_value, float new_value);

// never used
float inject_transition_fault(transition_fault f, int i, int j, int k, float result);
float inject_fc_transition_fault(fc_transition_fault f, int i, int j, int k, float result);

#endif
