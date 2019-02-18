#include "darknet.h"

#define MAX_BUFF 256

typedef struct golden_prediction_t {
    char *imagepath;        
    int label_pred;     // label predicted
    float c_score;      // confidence score of the predicted label
} golden_prediction;

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
    FILE *file= fopen(testsetfile, "r");

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

void execute_golden_prediction(network *net, list *imagepaths) {

    // convert the list into an array of elements
    char **imagepaths_array = list_to_array(imagepaths);

    int i = 0;
    int top_predictions = net->outputs;
    float *image_stream;
    float *prediction;

    while (imagepaths_array[i++] != NULL) {
        
        // load a new image 
        image i = load_image_color(imagepaths_array[i], 0, 0);
        image r = letterbox_image(i, net->w, net->h);

        image_stream = r.data;

        float *prediction = network_predict(net, image_stream);
        int max_i = max_confidence_score(prediction, net->outputs);

        // store the information about the golden prediction into the result file


    } 


}

void fault_simulation(char *cfgfile, char *weightsfile, char *testsetfile, char *faultlistfile) {

    network *net = load_network(cfgfile, weightsfile, 0);
    set_batch_network(net, 1);

    list *opt = read_data_cfg(cfgfile);

    char *label_list = option_find_str(opt, "name", 0);
    if (!label_list) label_list = option_find_str(opt, "d");

    char **labels = get_label(label_list);
    char buffer[256];                                                                                                                                                                        

}