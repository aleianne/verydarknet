#include "fault_injector.h"

FILE *open_fault_list_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("impossible to open the file %s\n", filename);
        exit(1);
    }
    return fp;
}

void close_fault_list(FILE *fp) {
    if (fp != NULL) {
        fclose(fp);
    }
}

void seu_fault_injector_sim(float *X, network *net, float *g_pred, int fault_percentage, char *filename, int network_layer) {
   
    /*int layer_n = network_layer;
    int top = net->outputs;

    int *indexes = calloc(top, sizeof(int)); 
    float *predictions = calloc(top, sizeof(float));

    int max_i = max (g_pred, top);

    outcome_t outcome;
    outcome.SDC = 0;
    outcome.Crit_SDC = 0;
    outcome.No_Crit_SDC = 0;
    outcome.MSK = 0;

    layer l = net->layers[layer_n];
    int n_iteration = (int) (l.outputs * l.size * l.size * l.c * fault_percentage) / 100;
    int n_filters = l.n, n_weights = l.nweights / l.n, n_output_neurons = l.out_w * l.out_h;

    int max_f;

    srand(time(NULL));

    FILE *fp = open_fault_list_file(filename);

    // this is the value that represent the max number of neuron that can be affected by the fault
    int max_neurons;

    clock_t begin_time = clock();

    if (l.type == CONVOLUTIONAL) {
        int i;
        fprintf(fp, "BIT\tFILTER\tWEIGHT\tNEURON");

        for (i = 0; i < n_iteration; ++i) {
            seu_fault *fault_i = (seu_fault *) malloc(sizeof(seu_fault));
            fault_i->bit = rand() % 32;
            fault_i->filter = rand() % n_filters;
            fault_i->weight = rand() % n_weights;
            fault_i->output_neuron = rand() % n_output_neurons;

	        // compute the number of neurons affected by the fault
	        max_neurons = n_output_neurons - fault_i->output_neuron;
	        fault_i->output_neuron_end = rand() %  max_neurons;

            // print into the file the informations about the fault
            fprintf(fp, "%10d\t%10d\t%10d\t%10d", fault_i->bit, fault_i->filter, fault_i->weight, fault_i->output_neuron);

            inject_convolutional_seu(fault_i, net, layer_n);

            predictions = network_predict(net, X);
            if(net->hierarchy) hierarchy_predictions(predictions, net->outputs, net->hierarchy, 1, 1);
            top_k(predictions, net->outputs, net->outputs, indexes);
            max_f = max (predictions, top);

            check_max_outcome (&outcome, predictions, g_pred, max_i, max_f); 
        }
    }

    fclose(fp);*/
}

// if the network layer is less than 0 apply the fault injection to all network layers
outcome_t stuck_at_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer) {
    int j = 0;

    int top = net->outputs;
    int *indexes = calloc(top, sizeof(int));
    float *predictions = calloc(top, sizeof(float));

    int max_f;
    int max_i = max(g_pred, top);

    outcome_t outcome;
    outcome.SDC = 0;
    outcome.Crit_SDC = 0;
    outcome.No_Crit_SDC = 0;
    outcome.MSK = 0;

    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("impossible to open file %s\n", filename);
        exit(1);
    }

    int mac_number = 1024;
    int bit_number = 8;
    int bit_position = 0;
    int k, m, n;

    for (k = 0; k < 2; k++) {
        for (m = 0; m < mac_number; m++)  {
            for (n = 0; n < bit_number; n++) {
                // add the 22 offset in order to create a stuck-at only to exponential bits
                bit_position = bit_position + 22;

                // inject fault into convolutional network
                inject_stuck_at_fault_into_layers(net, network_layer, bit_position, k);
                
                // predict the outcome
                predictions = network_predict(net, X);
                if(net->hierarchy) hierarchy_predictions(predictions, net->outputs, net->hierarchy, 1, 1);
                top_k(predictions, net->outputs, net->outputs, indexes);
                max_f = max (predictions, top);
                //check_max_outcome (&outcome, predictions, g_pred, max_i, max_f);

                // remove fault 
                remove_fault(net, network_layer);
            }
        }
    }
    
    fclose(fp);
    return outcome;
}

void permanent_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer) {
   
   /* int top = net->outputs;
    
    int *indexes = calloc(top, sizeof(int));
    float *predictions = calloc(top, sizeof(float));

    // this value return the index of the highest score of the prediction, without faults
    int max_i = max (g_pred, top);

    fault_t fault;
    outcome_t  outcome;
    outcome.SDC = 0;
    outcome.Crit_SDC = 0;
    outcome.No_Crit_SDC = 0;
    outcome.MSK = 0;

    int max_f;

    FILE *fl = open_fault_list_file(filename);

    clock_t begin_time = clock();

    fault.layer_index = network_layer;
    while (fscanf (fl, "%d %d", &(fault.weigth), &(fault.bit)) != EOF) {
        inject (net, &fault);
        predictions = network_predict(net, X);
        if(net->hierarchy) hierarchy_predictions(predictions, net->outputs, net->hierarchy, 1, 1);
        top_k(predictions, net->outputs, net->outputs, indexes);
        max_f = max (predictions, top);

        check_max_outcome (&outcome, predictions, g_pred, max_i, max_f); 
        release (net, &fault);	
    }

    fclose(fl);*/
}

void transition_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer) {
    
    /*int layer_n = network_layer;
    int top = net->outputs;

    int *indexes = calloc(top, sizeof(int));
    float *predictions = calloc(top, sizeof(float));

    int max_i = max (g_pred, top);

    outcome_t  outcome;
    outcome.SDC = 0;
    outcome.Crit_SDC = 0;
    outcome.No_Crit_SDC = 0;
    outcome.MSK = 0;

    int max_f;

    layer l = net->layers[layer_n];
    srand(time(NULL));

    FILE *fp = open_fault_list_file(filename);

    clock_t begin_time = clock();
    
    printf("\n=== BEGIN THE SIMULATION ===\n");

    if (l.type == CONVOLUTIONAL) {
        int i;
        int f, w, n;
       	while (fscanf(fp, "%d %d %d", &w, &f, &n) != EOF) {
            // read the information from the fault list and inject the error into the network
            i = 8;
            while(i > 0) {
                transition_fault *t_fault = (transition_fault *) malloc(sizeof(transition_fault));
                t_fault->bit =  22 + i;
                t_fault->filter = f;
                t_fault->weight = w;
                t_fault->output_neuron = n;

                // inject here a transition fault
                create_new_fault(net, t_fault, layer_n);

                // predict the outcome
                predictions = network_predict(net, X);
                if(net->hierarchy) hierarchy_predictions(predictions, net->outputs, net->hierarchy, 1, 1);
                top_k(predictions, net->outputs, net->outputs, indexes);
                max_f = max (predictions, top);
                check_max_outcome (&outcome, predictions, g_pred, max_i, max_f);
                i--;
            }
        }
    } else if (l.type == CONNECTED) {
        int j;
        int b, i, o;
       	while (fscanf(fp, "%d %d %d", &b, &i, &o) != EOF) {
	        j = 8;
            while (j > 0) {
                // allocate the fualt structure that contains the information about the fault position
                fc_transition_fault *ft = (fc_transition_fault *) calloc(1, sizeof(fc_transition_fault));
                ft->bit = 22 + j;
                ft->batch = b;
                ft->input_neuron = i;
                ft->output_neuron = o;

                // inject here the transition fault
                create_new_fc_fault(net, ft, layer_n);

                // predict the outcome
                predictions = network_predict(net, X);
                if(net->hierarchy) hierarchy_predictions(predictions, net->outputs, net->hierarchy, 1, 1);
                top_k(predictions, net->outputs, net->outputs, indexes);
                max_f = max (predictions, top);
                check_max_outcome (&outcome, predictions, g_pred, max_i, max_f);
                j--;
            }
        }
    }

    fclose(fp);*/
}