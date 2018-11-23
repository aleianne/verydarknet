#ifndef TRANSITION_INJECTOR_H
#define TRANSITION_INJECTOR_H

#include "darknet.h"

typedef struct fault_value_t {
    float previous_value;
    float actual_value;
} fault_value;

static float previous_mul_result;

fault_value delete_correct_value(float *B, int out_neuron, int weight, int ldb);
float inject_mul_fault(fault_value f, float *C, int ldc, int weight, int filter, int bit);

void multiple_fault_injection(network *net);

void create_new_fault(int target_layer, network *net,  transition_fault *fault_i);
void inject_transition_fault(transition_fault f, int i, int j, int k, float *result);

void create_new_fc_fault(int target_layer, network *net, fc_transition_fault *fault_i);
void inject_fc_transition_fault(fc_transition_fault f, int i, int j, int k, float *result);

float check_bit_on_fault_transition(int shift, float old_value, float new_value);

#endif