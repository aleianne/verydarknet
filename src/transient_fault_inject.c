#include "transient_fault_inject.h"

// TODO da controllare bene questo file perchè probabilmente c'è la necessità di adattarlo 
// per farlo funzionare con fault di tipo seu, visto anche che viene utilizzato nella funzione
// seu_generation nel file classifier.c di darknet


void inject_convolutional_seu(int target_layer, network *net, transition_fault *fault_i) {
    layer *l = &net->layers[target_layer];
    
    // per adesso inietta solo nel caso di layer convoluzionario
    /*if (l->type == CONVOLUTIONAL) {
        l->fault = fault_i;
        //l->is_faulty = 1;
    } else {
        fprintf(stderr, "this kind of error should be injected into a convolutional layer\n");
	}*/
}


void save_correct_value(float *matrix, int ld, int i, int j, int bit, weight_fault *fault) {
    fault->correct_value = matrix[ld*i + j];
    fault->faulty_value = flip_bit(bit, fault->faulty_value);
    matrix[ld*i + j] = 0;
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
