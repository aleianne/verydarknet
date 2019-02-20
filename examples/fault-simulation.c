#include "darknet.h"
#include "data_collector.h"
#include "fault_injector.h"
#include "fault_list.h"
#include "image.h"
#include "list.h"
#include "string.h"
#include "utils.h"

#define MAX_BUFF 256

int max_confidence_score(float *vec, int N) {
	int i;
	float tmp = vec[0];
	int i_max = 0;
	for (i = 1; i < N; i++) {
		if (vec[i] > tmp) {
			tmp = vec[i];
			i_max = i;
		}
	}
	return i_max;
}

list *load_imagepath_array(char *testsetfile) {
    FILE *file = fopen(testsetfile, "r");

    if (file == 0) file_error(testsetfile);

    char buffer[MAX_BUFF];
    list *imagepaths = make_list();

    while (fgets(buffer, MAX_BUFF, file) != NULL) {
        // add image path into the list
        char *imagepath = calloc(MAX_BUFF, sizeof(char));
        strncpy(imagepath, buffer, MAX_BUFF);
        list_insert(imagepaths, imagepath);
    }

    fclose(file);

    return imagepaths;
}

void execute_golden_prediction(network *net, list *imagepaths, char *pathname) {

    // convert the list into an array of elements
    char **imagepaths_array = (char **) list_to_array(imagepaths);
    int list_elements = imagepaths->size;

    int i = 0;
    int top_predictions = net->outputs;
    float *image_stream;
    float *predictions;

    // declare a new array of golden prediction
    prediction_results_t *net_predictions = calloc(list_elements, sizeof(prediction_results_t));

    while (imagepaths_array[i++] != NULL) {

        // catenate the path with the image name
        char *imagename = path_extension(imagepaths_array[i], pathname);

        // load a new image 
        image im = load_image_color(imagename, 0, 0);
        image r = letterbox_image(im, net->w, net->h);

        image_stream = r.data;

        predictions = network_predict(net, image_stream);
        int max_i = max_confidence_score(predictions, net->outputs);

        // store the information about the golden prediction into the results file
        net_predictions[i].imagepath = imagepaths_array[i];
        net_predictions[i].label_pred = max_i;
        net_predictions[i].c_score = predictions[max_i];

        // release the memory used for catenation
        free(imagename);
    } 

    // save all the data retrieved from the prediction into the golden_prediction.csv file
    char result[256] = "golden_prediction.csv";
    write_golden_prediction_file(net_predictions, result, list_elements);

    free(imagepaths_array);
}

void execute_faulty_prediction(network *net, list *image_list, list *fault_list, int target_layer) {

    char **imagepath_array = (char **) list_to_array(image_list);
    fault_list_entry_t **faultlist_array = (fault_list_entry_t **) list_to_array(fault_list);

    int test_set_size = image_list->size;
    int fault_list_size = image_list->size;
    int i, j;

    float *image_data;
    float *predictions;

    int top = net->outputs;

    // create a new output prediction array 
    prediction_results_t *prediction_results = calloc(test_set_size, sizeof(prediction_results_t)); 

    for (i = 0; i < fault_list_size; i++) {
        fault_list_entry_t *entry = faultlist_array[i];

        // inject the fault into the network
        inject_fault(*entry, net, target_layer);

        for (j = 0; j < test_set_size; j++) {
            // create a new simulation for the selected image
            char *img = imagepath_array[j];

            // load a new image
            image im = load_image_color(img, 0, 0);
            image r = letterbox_image(im, net->w, net->h);

            // prediction 
            image_data = r.data;
            predictions = network_predict(net, image_data);

            int predicted_label = max_confidence_score(predictions, top);
            float max_f = predictions[predicted_label];

            prediction_results[i].c_score = max_f;
            prediction_results[i].imagepath = img;
            prediction_results[i].label_pred = predicted_label;

            // release the memory used to store the image
            if(r.data != im.data) free_image(r);
            free_image(im);
        }

        // create a new filename in order to store the data contained into the prediction results array
        char *filename = faulty_prediction_name_generator(*entry);

        // save the prediction into a file  
        write_faulty_prediction_file(prediction_results, filename, test_set_size);
    }
}

void fault_simulation(char *cfgfile, char *weightsfile, char *testsetfile, char *faultlistfile, int target_layer) {

    network *net = load_network(cfgfile, weightsfile, 0);
    set_batch_network(net, 1);

    list *opt = read_data_cfg(cfgfile);

    char *label_list = option_find_str(opt, "names", 0);
    if (!label_list) label_list = option_find_str(opt, "labels", "data/labels.list");
    char *path = option_find_str(opt, "path", "./data/");

    // only for debug 
    if (strcmp(path, "./data") == 0) {
        fprintf(stderr, "the default path has been used");
    }

    // load labels
    char **labels = get_labels(label_list);
    char buffer[256];     
    
    // load the image from the test set file and execute the golden prediction
    list *image_list = load_imagepath_array(testsetfile);
    execute_golden_prediction(net, image_list, path);     

    // load the fault list from the fault_list.txt file
    list *fault_list = load_faultlist_file(faultlistfile);
    if (fault_list->size == 0) {
        fprintf(stderr, "the fault list file %s is empty\n", faultlistfile);
        return;
    }
    execute_faulty_prediction(net, image_list, fault_list, target_layer);

    // release the memory used by the list
    free_list_contents(image_list); 
    free_list(image_list);

    // release the the memory used by the fault list
    free_list_contents(fault_list);
    free_list(fault_list);
}