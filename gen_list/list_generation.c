#include "list_generation.h" 

void create_fault_list_transition(char *file_name, int  n_multiplication,  int perc, transition_fault fault) {
        FILE *out;
        int n_inj, i;
        int w, f, n;

        out = fopen  (file_name, "w");
        srand (time (NULL));
        n_inj =  (n_multiplication) / perc ;

        for (i = 0; i < n_inj; i++) {

                w = rand() % fault.weight;
                f = rand() % fault.filter;
                n = rand() % fault.output_neuron;

                fprintf (out, "%d %d %d\n", w, f, n);

        }

        fclose (out);
}

void create_fault_list_transition_fc(char *filename, int  n_multiplication, int perc, fc_transition_fault fault) {
	FILE *out;
        int n_inj, i;
        int b, in_n, out_n;

        out = fopen  (filename, "w");
        srand (time (NULL));
        n_inj =  (n_multiplication) / perc ;

        for (i = 0; i < n_inj; i++) {

                b = rand() % fault.batch;
                in_n = rand() % fault.input_neuron;
                out_n = rand() % fault.output_neuron;

                fprintf (out, "%d %d %d\n", b, in_n, out_n);

        }

        fclose (out);
}
