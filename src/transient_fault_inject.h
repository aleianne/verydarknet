#ifndef TRANSIENT_FAULT_INJECT_H
#define TRANSIENT_FAULT_INJECT_H

#include "darknet.h"

void inject_convolutionals_seu(seu_fault *fault, network *net, int index);

float flip_bit(int bit, float value);

#endif