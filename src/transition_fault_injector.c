#include "transition_fault_injector.h" 

void multiple_fault_injection(network *net) {

    FILE *fp = fopen("multiple_layer_faults.txt", "a");
    if (fp == NULL) {
        printf("impossible to open the file, error: %s", strerror(error));
        exit(1);
    }

    layer l;
    fc_transition_fault *fault1;
    transition_fault *fault2;
    int i;
    for (i = 0; i < net->n; ++i) {
        l = net->layers[i];
        if (l.type == CONVOLUTIONAL) {
            fault2 = (transition_fault *) malloc(sizeof(transition_fault));
            int n_filters = l.n, n_weights = l.nweights / l.n, n_output_neurons = l.out_w * l.out_h;

            fault2->bit = rand() % 32;
            fault2->weight = rand() % n_weights;
            fault2->output_neuron = rand() % n_output_neurons;
            fault2->filter = rand() % n_filters;

            fprintf(fp, "layer %d, type: convolutional bit: %d weight: %d neuron: %d filter: %d", fault2->bit, fault2->weight, fault2->output_neuron, fault2->filter);

            create_new_fault(i, net, fault2);
        } else if (l.type == CONNECTED) {
            fault1 = (fc_transition_fault *) malloc(sizeof(fc_transition_fault));
            int batch_size = l.batch;
            int n_output = l.outputs;
            int n_input = l.inputs;

            fault1->bit = rand() % 32;
            fault1->batch = rand() % batch_size;
            fault1->output_neuron = rand() % n_output;
            fault1->input_neuron = rand() % n_input;

            fprintf(fp, "layer %d, type: fully connected bit: %d batch: %d input neuron: %d output neuron: %d", fault1->bit, fault1->batch, fault1->input_neuron, fault1->output_neuron);

            create_new_fc_fault(i, net, fault1);
        }
    }

    fclose(fp);
}

void create_new_fault(int target_layer, network *net, transition_fault *fault_i) {
    layer *l = &net->layers[target_layer];
    
    // per adesso inietta solo nel caso di layer convoluzionario
    if (l->type == CONVOLUTIONAL) {
        l->fault = fault_i;
        l->is_faulty = 1;
    } else {
        fprintf(stderr, "this kind of error should be injected into a convolutional layer\n");
    }
}

void create_new_fc_fault(int target_layer, network *net, fc_transition_fault *fault_i) {
    layer *l = &net->layers[target_layer];

    if (l->type == CONNECTED) {
        l->fc_fault = fault_i;
        l->is_faulty = 1;
    } else {
        fprintf(stderr, "this kind of error should be injected into a fully connected layer\n");
    }
}

void inject_fc_transition_fault(fc_transition_fault f, int i, int j, int k, float *result) {
    if (i == f.batch && j == f.input_neuron && k == f.output_neuron - 1) {
        previous_mul_result = *result;
    }

    if (i == f.batch && j == f.input_neuron && k == f.output_neuron) {
         *result = check_bit_on_fault_transition(f.bit, previous_mul_result, *result);
    }
}

void inject_transition_fault(transition_fault f, int i, int j, int k, float *result) {
    // store the previous value into a variable
    if (i == f.filter && j == f.weight && k == f.output_neuron-1) {
        previous_mul_result = *result;
    }

    // if the interation is the correct one is possible to inject the error
    if(i == f.filter && j == f.weight && f.output_neuron == k) {
        *result = check_bit_on_fault_transition(f.bit, previous_mul_result, *result);
    }
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
