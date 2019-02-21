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

void inject_fault(fault_list_entry_t fault_record, network *net, int target_layer) {
    // if the layer is 0 than the entire network must be tested
    if (target_layer >= 0) {

        int mac_position = fault_record.fault_position;
        int bit = fault_record.faulty_bit;

        // inject the fault into the network 
        FAULT_MODEL fault_model = fault_record.fault_type;
        switch (fault_model) {
            case PERMENANT_FAULT: {
            
            } break;

            case STUCK_AT_0: {
                // only for debug
                fprintf(stderr, "the fault injected into the network is a stuck at 0\n");
                inject_stuck_at_fault_into_layers(net, target_layer, bit, 0, mac_position);
            } break;

            case STUCK_AT_1: {
                // only for debug 
                fprintf(stderr, "the fault inejcted into the network is a stuck at 1\n");
                inject_stuck_at_fault_into_layers(net, target_layer, bit, 1, mac_position);
            } break;

            default: {
                fprintf(stderr, "no fault has been injected\n");
            }
        }

    } else {
        // only for debug 
        fprintf(stderr, "the target layer must be greater or equal to 0\n");
    }
}

void remove_fault(fault_list_entry_t fault_record, network *net, int target_layer) {
    FAULT_MODEL fault_model = fault_record.fault_type;

    switch (fault_model) {

        case NO_FAULT: {
            fprintf(stderr, "no fault should be injected into the network");
        }

        case STUCK_AT_0: {
            remove_stuck_at_fault(net, target_layer);
        }

        case STUCK_AT_1: {
            remove_stuck_at_fault(net, target_layer);
        }

        default: {
            // only for debug 
            fprintf(stderr, "not \n");
        }
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

// if the network_layer is less than 0 apply the fault injection to all network layers
void stuck_at_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer) {
    /*int j = 0;

    int top = net->outputs;
    int *indexes = calloc(top, sizeof(int));
    float *predictions = calloc(top, sizeof(float));

    int max_f;
    int max_i = max(g_pred, top);

    // print this information only for debug
    fprintf(stderr, "begin the stuck-at simulation\n");
    clock_t sim_begin_time = clock();

    int mac_number = MAC_UNIT_N;
    int bit_number = 8;
    int bit_position = 7;
    int k, m, n;*/

    /*for (k = 0; k < 2; k++) {
        for (m = 0; m < mac_number; m++)  {
            for (n = 0; n < bit_number; n++) {*/
                // add the 22 offset in order to create a stuck-at only into exponential bits
                /*bit_position = bit_position + 23;
                k = 1;
                m = 81;
                // inject fault into convolutional network
                inject_stuck_at_fault_into_layers(net, network_layer, bit_position, k, m);
                
                // predict the outcome
                predictions = network_predict(net, X);
                if(net->hierarchy) hierarchy_predictions(predictions, net->outputs, net->hierarchy, 1, 1);
                top_k(predictions, net->outputs, net->outputs, indexes);
                max_f = max(predictions, top);
                //check_max_outcome (&outcome, predictions, g_pred, max_i, max_f);

                // remove fault 
                remove_fault(net, network_layer);

                fprintf(stderr, "results for golden prediction:\n");
                print_prediction_results(g_pred, top);
                fprintf(stderr, "\nresults for the faulty prediction:\n");
                print_prediction_results(predictions, top);
                fprintf(stderr, "the golden prediction is %d\n", max_i);
                fprintf(stderr, "the faulty prediction is %d\n", max_f);*/
                /*
            }
        }
    }*/

    /*clock_t sim_end_time = clock();
    fprintf(stderr, "simulation completed in %f seconds\n", sec(sim_end_time - sim_begin_time));*/
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
    
    /*
    
    int layer_n = network_layer;
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