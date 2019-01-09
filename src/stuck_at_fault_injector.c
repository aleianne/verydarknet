#include "stuck_at_fault_injector.h"

void inject_stuck_at_fault(network *net, int layer_n, int bit, int type) {
  layer *l = &net->layers[layer_n];
  // set the faulty flag to 1 and inject the fault
  l->f_model = STUCK_AT;
  l->st_fault =  malloc(sizeof(stuck_at_fault));
  l->st_fault->bit = bit + 22;
  l->st_fault->type = type;
}

float compute_faulty_multiplication(float value, int position, int type) {
  int tmp, mask;
  memcpy(&tmp, &value, sizeof(float));
  if (type == 1) {
    //printf("DEBUG: the value before the stuck at is %d\n", tmp);
    mask = 0x01;
    mask <<= position;
    tmp |= mask;
    //printf("DEBUG: the value after the stuck at is %d\n", tmp);
  } else {
    //printf("the stuck at position is %d\n", position);
    //printf("DEBUG: the value before the stuck at is %d\n", tmp);
    mask = 0x01;
    mask <<= position;
    mask = ~mask;
    tmp &= mask;
    //printf("DEBUG: the value after the stuck at is %d\n", tmp);
  }
  memcpy(&value, &tmp, sizeof(float));
  return value;
}

void remove_fault(network *net, int layer_n) {
  layer *l = &net->layers[layer_n];
  free(l->st_fault);
  l->f_model = NO_FAULT;
  l->st_fault = NULL;
}
