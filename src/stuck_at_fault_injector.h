#ifndef STUCK_AT_FAULT_H
#define STUCK_AT_FAULT_H

#include "darknet.h"

void inject_stuck_at_fault(network *net, int layer_n, int bit, int type);
void remove_fault(network *net, int layer_n);
float compute_faulty_multiplication(float value, int position, int type);

#endif
