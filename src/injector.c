
#include "injector.h"
#include <stdlib.h>
#include <stdio.h>


void init_inj (network *net) {

	int i;
	for(i = 0; i < net->n; ++i){
        	layer l = net->layers[i];
		
		if (l.type == CONVOLUTIONAL) 
			printf ("layer %d, type = %d, number of connections = %d \n",i,l.type, l.nweights);

		if (l.type == CONNECTED) 
			printf ("layer %d, type = %d, number of connections = %d \n",i,l.type, l.outputs*l.inputs );

	}
}


void create_fault_list (int connections, int bit_size, int perc) {

	FILE *out;
	int n_inj, i;
	int w, bit;
	
	out = fopen  ("fault_list.txt", "w");
	srand (time (NULL));
	n_inj =  (connections * bit_size) / perc ;
	
	for (i = 0; i < n_inj; i++){

		
		w = rand () % connections;
		bit = rand () % bit_size;

		fprintf (out, "%d %d\n", w, bit);


	} 

	fclose (out);
}

 
// injector


// this method simply inject a fault by applyng a bit flip to the n-th bit 
void inject (network *net, fault_t *fault) 
{
	float w;   
    unsigned int mask = 1;
    int i_t;
	layer l = net->layers[fault->layer_index];
	w = l.weights[fault->weigth];
	fault->good_w = w; 
//	printf ("original w = %f\n", w);
    memcpy (&i_t, &w, sizeof (w));
	mask = mask << fault->bit; 
    i_t = i_t ^ mask;
    memcpy (&w, &i_t, sizeof (w));
//	printf ("faulty w = %f\n", w);
	l.weights[fault->weigth] = w;


}

void release (network *net, fault_t *fault) 
{

	layer l = net->layers[fault->layer_index];
	l.weights[fault->weigth] = fault->good_w; 

}
int check_outcome (outcome_t *o, float *f_out, float *g_out, int N) {


	int i;
	int ret = 0;

	float tmp;
	for (i = 0; i < N; i++) {

		tmp =f_out[i] - g_out [i]; 
		if (tmp == 0)
			o->MSK++;
		else {
			
			o->SDC++;
			if (tmp < 0.0)
				tmp *= -1.0;
			if (tmp > 0.05) {
				o->Crit_SDC++;
				ret = 1;
			}
			else 
				o->No_Crit_SDC++;
		}

	}
	return ret;
}

int max (float *vec, int N) {
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
int check_max_outcome (outcome_t *o, float *f_out, float *g_out, int g_max_index, int f_max_index) {


	int ret = 0;
	float tmp;

	if (g_max_index != f_max_index) {
		o->Crit_SDC++;
		o->SDC++;
		ret = 1;

	} else {
	
		if (f_out[f_max_index] > g_out[g_max_index]) {
			o->SDC++;
			o->No_Crit_SDC++;
		} else {
			if (f_out[f_max_index] == g_out[g_max_index]) {
				o->MSK++;
			}
			else {
				if (( g_out[g_max_index] - f_out[f_max_index]) > 0.05) { 
					o->SDC++;
					o->Crit_SDC++;
					ret = 1;
				}	
				else {
					o->SDC++;
					o->No_Crit_SDC++;
				}
			}

		}

	}
		
	return ret;
}
