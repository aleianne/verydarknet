#ifndef TRANSITION_INJECTOR_H
#define TRANSITION_INJECTOR_H

#include "darknet.h"

static float previous_mul_result;

void multiple_fault_injection(network *net);

void create_new_fault(int target_layer, network *net,  transition_fault *fault_i);
void inject_transition_fault(transition_fault f, int i, int j, int k, float *result);

void create_new_fc_fault(int target_layer, network *net, fc_transition_fault *fault_i);
void inject_fc_transition_fault(fc_transition_fault f, int i, int j, int k, float *result);

float check_bit_on_fault_transition(int shift, float old_value, float new_value);

#endif