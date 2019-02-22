#include "data_collector.h"

/*
void print_network_informations(network *net) {
     int i;
    layer l;

    printf("NETWORK INFORMATIONS\n");
    for (i = 0; i < net->n; ++i) {
        l = net->layers[i];
        print_layer_n_mul(l, i);
        print_layer_n_weights(l);
    }
    printf("\n");
}

void print_layer_n_mul(layer l, int layer_id) {
    if (l.type == CONVOLUTIONAL) {
        print_layer_conv_info(l, layer_id); 
    } else if (l.type == CONNECTED) {
        print_layer_fc_info(l, layer_id);
    } else {
        // do nothing;
    }  
}

void print_layer_n_weights(layer l) {
    if (l.type == CONVOLUTIONAL) {
        printf("WEIGHTS (total): %d\n", l.nweights);
    } else if (l.type == CONNECTED) {
        int n_weights = l.outputs * l.inputs;
        printf("WEIGHTS (total): %d\n", n_weights);
    }
}

void print_layer_conv_info(layer l, int layer_id) {
    int n_iteration = l.outputs * l.size * l.size * l.c;
    int n_filters = l.n;
    int n_weights = l.nweights / l.n;
    int n_output_neurons = l.out_w * l.out_h;
    printf("layer convolutional n: %d; MUL: %d, FILTERS: %d, WEIGHTS (per filter): %d, OUTPUTS: %d ", layer_id, n_iteration, n_filters, n_weights, n_output_neurons);
}

void print_layer_fc_info(layer l, int layer_id) {
    int n_iteration = l.outputs * l.inputs;
    printf("layer fully connected n: %d; MUL: %d, INPUTS %d, OUTPUTS %d, BATCHES %d ", layer_id, n_iteration, l.inputs, l.outputs, l.batch);
}

void print_prediction_results(float *pred, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("prediction score for label %d: %f\n", i, pred[i]);
    }
}

void init_stuck_at_data_struct() {
    int i, j;
    for (i = 0; i < BIT_N; i++) {
        for (j = 0; j < LABEL_N; i++) {
            sa0_label_freq[i][j] = 0;
            sa1_label_freq[i][j] = 0;
        }
    }
}

void update_sa_freq(int st_type, int bit,  int label) {
    if (label < 0 || label > LABEL_N) {
        return;
    }
    if (st_type == 1) {
        sa0_label_freq[bit][label]++;
    } if (st_type == 0) {
        sa1_label_freq[bit][label]++;
    }
}
*/

FILE *handle_file_open(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
       file_error(filename);
    }
    return file;
}

// this method should be correct since 
/*char *path_extension(char **filename, char *path) {
    int filename_size = strlen(*filename);
    int path_size = strlen(path);
    int tot_length = filename_size + path_size;

    // delete the last \n in the line 
    *filename[filename_size - 1] = '\0';

    // allocate a buffer that contains the complete image path;
    char *cpt_image_path = calloc(tot_length + 1, sizeof(char));
    strncpy(cpt_image_path, path, path_size);
    strcat(cpt_image_path, filename);

    return cpt_image_path;
} */

// this function generate output result file
char *faulty_prediction_name_generator(fault_list_entry_t fault_record) {

    // in this function i must generate the information about results generated file
    FAULT_MODEL fault_model = fault_record.fault_type;
    int fault_position = fault_record.fault_position;
    int fault_bit = fault_record.faulty_bit;

    char *s_bit = calloc(3, sizeof(char));
    char *s_position = calloc(5, sizeof(char));

    char *filename = calloc(256, sizeof(char));

    sprintf(s_bit, "%d", fault_bit);
    sprintf(s_position, "%d", fault_position);

    switch (fault_model)
    {
        case NO_FAULT: {
            // this is a very strange case since the fault record has been already checked
            fprintf(stderr, "no faults are been specified");
        } break;

        case STUCK_AT_0: {
            strcpy(filename, "s-a-0_");
        } break;

        case STUCK_AT_1: {
            strcpy(filename, "s-a-1_");
        } break;

        default: {
            fprintf(stderr, "no faults are been specified");
        } break;
    }

    // concatenate to the filename string the mac position and the faulty bit
    strcat(filename, s_position);
    strcat(filename, "_");
    strcat(filename, s_bit);
    strcat(filename, ".csv");

    // only for debug
    fprintf(stderr, "the filename to be used during the simulation is %s\n\n", filename);
    return filename;
}

void write_prediction_file(prediction_results_t *prediction_array, char *filename, int size, char *header) {
    
    FILE *file = handle_file_open(filename); 
    // insert the header row into the file
    fprintf(file, "%s", header);

    int i;
    for (i = 0; i < size; i++) {
        fprintf(file, "%s\t%d\t%f10.2\n", prediction_array[i].imagepath, prediction_array[i].label_pred, prediction_array[i].c_score);
    }
    fclose(file);
}

void write_faulty_prediction_file(prediction_results_t *prediction_array, char *filename, int size) {
    char header[256] = "image name\tprediction\tconfidence score\n\n";
    write_prediction_file(prediction_array, filename, size, header);
}

void write_golden_prediction_file(prediction_results_t *prediction_array, char *filename, int size) {
    char header[256] = "image name\tgolden prediction\tconfidence score\n\n";
    write_prediction_file(prediction_array, filename, size, header);
}

