#include "transient_fault_inject.h"

void inject_convolutionals_seu(seu_fault *fault, network *net, int index) {
    layer *l = &net->layers[index];

    if (l->type == CONVOLUTIONAL) {
        l->seu_fault = fault;
        l->is_faulty = 1;
    } else {
        printf("the target layer should be convolutional\n");
    }
}

float flip_bit(int bit, float value) {
    int temp1, temp3, temp2;
    memcpy (&temp1, &value, sizeof (value));
	int mask = mask << bit;
    int mask2 = ~ mask; 
    temp2 = temp1 ^ mask;
    temp3 = temp1 & mask2;
    if (temp3 == mask2) {
        temp2 &= mask2;
    }
    memcpy (&value, &temp2, sizeof (value));
    return value;
}