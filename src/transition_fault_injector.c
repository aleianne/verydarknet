#include "transition_fault_injector.h" 

void create_new_fault(int target_layer, network *net, transition_fault *fault_i) {
    layer *l = &net->layers[target_layer];
    
    // per adesso inietta solo nel caso di layer convoluzionario
    if (l->type == CONVOLUTIONAL) {
        l->fault = fault_i;
    } else {
        fprintf(stderr, "this kind of error should be injected into a convolutional layer");
    }
}

void inject_transition_fault(transition_fault f, int i, int j, float *result) {
    // store the previous value into a variable
    if (i == f.row && j == f.column-1) {
        previous_mul_result = *result;
    }

    // if the interation is the correct one is possible to inject the error
    if(i == f.row && j == f.column) {
        *result = check_bit_on_fault_transition(f.bit, previous_mul_result, *result);
    }
}

float check_bit_on_fault_transition(int shift, float old_value, float new_value) {
    int b_value1, b_value2;
    int mask = 0x1;
    int mask2;
    mask <<= shift;
    mask2 = ~ mask;
    memcpy(&b_value1, &old_value, sizeof(old_value));
    memcpy(&b_value2, &new_value, sizeof(new_value));
    b_value1 &= mask;
    b_value2 &= mask;
    if (b_value1 != b_value2) {
        int temp;
        memcpy(&temp, &new_value, sizeof(float));
        temp &= mask2;
        temp += b_value1;
        memcpy(&new_value, &temp, sizeof(float));
    } 
    return new_value;
}