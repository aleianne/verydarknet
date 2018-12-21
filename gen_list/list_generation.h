#ifndef LIST_GENERATION_H
#define LIST_GENERATION_H

#include "darknet.h"

void create_fault_list_transition(char *file_name, int  n_multiplication,  int perc, transition_fault fault);
void create_fault_list_transition_fc(char *filename, int  n_multiplication, int perc, fc_transition_fault fault);
#endif
