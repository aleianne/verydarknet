#ifndef INJ_DARKNET_API
#define INJ_DARKNET_API

#include <stdlib.h>
#include <time.h>
#include "darknet.h"

#define MAX_INJ 100

typedef struct _fault_t {

	int weigth;
	int bit;
	int layer_index;
	float good_w;

} fault_t;

typedef struct _outcome_t {

	int SDC;
	int Crit_SDC;
	int No_Crit_SDC;
	int MSK;
	

} outcome_t;


void init_inj (network *net);
void inject (network *net, fault_t *fault); 
void release (network *net, fault_t *fault); 
void create_fault_list (int connections, int bit_size, int perc); 

int check_outcome (outcome_t *o, float *f_out, float *g_out, int N); 
int check_max_outcome (outcome_t *o, float *f_out, float *g_out, int g_max_index, int f_max_index); 
int max (float *vec, int N);

#endif
