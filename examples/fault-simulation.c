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

float confidence_score_margin(float *predictions, int n) {
    int *indexes = malloc(n*sizeof(int));
    top_k(predictions, n, n, indexes);
    float first = predictions[indexes[0]];
    float second = predictions[indexes[1]];
    return (first - second);
}

list *load_imagepath_array(char *testsetfile) {
    FILE *file = fopen(testsetfile, "r");

    if (file == 0) file_error(testsetfile);

    char buffer[MAX_BUFF];
    int length;
    list *imagepaths = make_list();

    while (fgets(buffer, MAX_BUFF, file) != NULL) {
        // add image path into the list
        length = strlen(buffer);
        char *imagepath = calloc(length, sizeof(char));
        strncpy(imagepath, buffer, length - 1);
        list_insert(imagepaths, imagepath);
    }

    fclose(file);

    return imagepaths;
}

void *image_path(char *image, char *path) {
    int image_length = strlen(image);
    int path_length = strlen(path);
    char *filename = calloc(path_length + image_length, sizeof(char));

    strncpy(filename, path, path_length);
    strcat(filename, image);
    return filename;
}

void execute_golden_prediction(network *net, list *images, char *pathname) {

    // convert the list into an array of elements
    char **imagepaths_array = (char **) list_to_array(images);
    int list_elements = images->size;

    int i = 0;
    int top_predictions = net->outputs;
    float *image_stream;
    float *predictions;

    // only for debug
    fprintf(stderr, "begin the golden prediction\n");
    fprintf(stderr, "the total number of image into the test set is %d\n", list_elements);

    // declare a new array of golden prediction
    prediction_results_t *net_predictions = calloc(list_elements, sizeof(prediction_results_t));

    clock_t begin = clock();

    while (i < list_elements) {

        char *imagename = image_path(imagepaths_array[i], pathname);

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

        i++;
    } 

    fprintf(stderr, "golden prediction finished in %f sec\n", sec(clock() - begin));

    // save all the data retrieved from the prediction into the golden_prediction.csv file
    fprintf(stderr, "begin to save the golden prediction into a file\n");

    char result[256] = "golden_prediction.csv";
    write_golden_prediction_file(net_predictions, result, list_elements);

    fprintf(stderr, "golden predictions saved correclty into the file\n");
    free(imagepaths_array);
}

void execute_faulty_prediction(network *net, list *image_list, list *fault_list, int target_layer, char *pathname) {

    char **imagepath_array = (char **) list_to_array(image_list);
    fault_list_entry_t **faultlist_array = (fault_list_entry_t **) list_to_array(fault_list);

    int test_set_size = image_list->size;
    int fault_list_size = fault_list->size;

    //int fault_list_size = 1;
    //int test_set_size = 1;

    int i, j;

    float *image_data;
    float *predictions;

    int top = net->outputs;

    // create a new output prediction array 
    prediction_result_fault_t *prediction_results = calloc(fault_list_size, sizeof(prediction_result_fault_t)); 

    // only for debug 
    fprintf(stderr, "begin the faulty prediction\n\n");
    clock_t begin = clock();
    
    for (i = 0; i < test_set_size; i++) {

        // create a new simulation for the selected image
        char *img = image_path(imagepath_array[i], pathname);

        // load a new image
        image im = load_image_color(img, 0, 0);
        image r = letterbox_image(im, net->w, net->h);
        image_data = r.data;

        for (j = 0; j < fault_list_size; j++) {

            fault_list_entry_t *entry = faultlist_array[j];

            // inject the fault into the network
            inject_fault(*entry, net, target_layer);            

            predictions = network_predict(net, image_data);

            int predicted_label = max_confidence_score(predictions, top);
            float max_f = predictions[predicted_label];
            float margin = confidence_score_margin(predictions, top);

            prediction_results[j].c_score = max_f;
            prediction_results[j].fault = entry;
            prediction_results[j].label_pred = predicted_label;
            prediction_results[j].score_margin = margin;

            remove_fault(*entry, net, target_layer);
        }
        
        // save the results into the sim results directory
        write_prediction_file_2(prediction_results, imagepath_array[i], fault_list_size);

        fprintf(stderr, "image %s completed succesfully\n", imagepath_array[i]);

        // release the memory used to store the image
        if(r.data != im.data) free_image(r);
        free_image(im);
        free(img);
    }

    fprintf(stderr, "prediction terminated in %f sec\n", sec(clock() - begin));
}

void fault_simulation(char *datacfg, char *cfgfile, char *weightsfile, char *testsetfile, char *faultlistfile, int target_layer) {

    network *net = load_network(cfgfile, weightsfile, 0);
    set_batch_network(net, 1);

    list *opt = read_data_cfg(datacfg);

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
    //execute_golden_prediction(net, image_list, path);     

    // load the fault list from the fault_list.txt file
    list *fault_list = load_faultlist_file(faultlistfile);
    if (fault_list->size == 0) {
        fprintf(stderr, "the fault list file %s is empty\n", faultlistfile);
        // release the memory used by the image list
        free_list_contents(image_list); 
        free_list(image_list);
        return;
    }
    
    execute_faulty_prediction(net, image_list, fault_list, target_layer, path);

    // release the memory used by the list
    free_list_contents(image_list); 
    free_list(image_list);

    // release the the memory used by the fault list
    free_list_contents(fault_list);
    free_list(fault_list);
}

// the example od string passed to darknet.exe ./darknet [cfg] [weights] [test set] [fault list] [-target] 
void run_simulation(int argc, char **argv) {
    
    if (argc < 4) {
        fprintf(stderr, "the usage of the simualtion is: [cfg] [weights] [test set] [fault list] [-target]");
        return;
    }

    char *data = argv[2]; 
    char *network_configuration = argv[3]; 
    char *weights = (argc > 4) ? argv[4] : 0;
    char *testset = (argc > 5) ? argv[5] : 0; 
    char *faultlist = (argc > 6) ? argv[6] : 0;

    // get the target layer, if is not included return 0
    int network_layer = find_int_arg(argc, argv, "-layer", 0);

    // run the simulation 
    fault_simulation(data, network_configuration, weights, testset, faultlist, network_layer);
}