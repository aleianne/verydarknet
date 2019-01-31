#ifndef FAULT_INJECTOR_H
#define FAULT_INJECTOR_H

#include "darknet.h"
#include "injector.h"
#include "stuck_at_fault_injector.h"
#include "transition_fault_injector.h"
#include "transient_fault_injector.h"

#define MAC_UNIT_N 1024

FILE *open_fault_list(char *filename);
void close_fault_list(FILE *fp);
void seu_fault_injector_sim(float *X, network *net, float *g_pred, int fault_percentage, char *filename, int network_layer);
void stuck_at_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer);
void permanent_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer); 
void transition_fault_injector_sim(float *X, network *net, float *g_pred, char *filename, int network_layer);

#endif 

