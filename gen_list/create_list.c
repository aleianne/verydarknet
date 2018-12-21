#include "list_generation.h"

int main (int argc, char **argv) {

  if (argc != 3) {
    printf("%s commad options: <fault model> <filename>\n", argv[0]);
    exit(1);
  }

  char *filename = argv[2];
  char *model = argv[1];

  if (strcmp(model, "transition_conv") == 0) {

    transition_fault fault;
    int n_mul, perc;
    printf("insert the number of multiplications: ");
    scanf("%d", &n_mul);
    printf("insert the fault percentage: ");
    scanf("%d", &perc);
    printf("insert the number of filters: ");
    scanf("%d", &fault.filter);
    printf("insert the number of weights (per filter): ");
    scanf("%d", &fault.weight);
    printf("insert the number of outputs: ");
    scanf("%d", &fault.output_neuron);

    create_fault_list_transition(filename, n_mul, perc, fault);

  } else if (strcmp(model, "transition_fc") == 0) {

    fc_transition_fault fault;
    int n_mul, perc;
    printf("insert the number of multiplications: ");
    scanf("%d", &n_mul);
    printf("insert the number of fault percentage: ");
    scanf("%d", &perc);
    printf("insert the number of inputs: ");
    scanf("%d", &fault.input_neuron);
    printf("insert the number of outputs: ");
    scanf("%d", &fault.output_neuron);
    printf("inser the number of batches: ");
    scanf("%d", &fault.batch);

    create_fault_list_transition_fc(filename, n_mul, perc, fault);

  } else {
    printf("unknown fault model...");
    exit(1);
  }
       
  return 1;
}
	

