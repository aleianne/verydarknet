#include "transition_fault_injector.h" 

void create_new_fault(network *net, transition_fault *fault_i, int target_layer) {
    layer *l = &net->layers[target_layer];
    l->f_model = TRANSITION_FAULT;

    if (l->type == CONVOLUTIONAL) {
        l->t_conv_fault = fault_i;
        l->f_model = TRANSITION_FAULT;
    } else {
        fprintf(stderr, "this kind of error should be injected into a convolutional layer\n");
    }
}

void create_new_fc_fault(network *net, fc_transition_fault *fault_i, int target_layer) {
    layer *l = &net->layers[target_layer];
    
    if (l->type == CONNECTED) {
        l->t_fc_fault = fault_i;
	l->f_model = TRANSITION_FAULT;
    } else {
        fprintf(stderr, "this kind of error should be injected into a fully connected layer\n");
    }
}

// not used
float inject_fc_transition_fault(fc_transition_fault f, int i, int j, int k, float result) {
  /*if (i == f.batch && j == f.input_neuron && k == f.output_neuron - 1) {
        previous_mul_result = result;
    }

    if (i == f.batch && j == f.input_neuron && k == f.output_neuron) {
         return  check_bit_on_fault_transition(f.bit, previous_mul_result, result);
	 }*/

    return result;
}

// not used
float inject_transition_fault(transition_fault f, int i, int j, int k, float result) {
    // store the previous value into a variable
    /*if (i == f.filter && j == f.weight && k == f.output_neuron - 1) {
        previous_mul_result = result;
    }

    // if the interation is the correct one is possible to inject the error
    if(i == f.filter && j == f.weight && f.output_neuron == k) {
        return check_bit_on_fault_transition(f.bit, previous_mul_result, result);
	}*/

    return result;
}

fault_value delete_correct_value(float *B, int out_neuron, int weight, int ldb)  {
    fault_value f;
    f.actual_value = B[ldb*weight + out_neuron];
    f.previous_value = B[ldb*weight + out_neuron - 1];
    B[ldb*weight + out_neuron] = 0;
    return f;
}

float inject_mul_fault(fault_value f, float *A, int lda, int weight, int filter, int bit) {
    float c = A[lda*filter + weight];
    float old_value = f.previous_value * c;
    float new_value = f.actual_value * c ;
    return check_bit_on_fault_transition(bit, old_value, new_value);
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
