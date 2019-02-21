#include "fault_list.h"

fault_list_entry_t *convert_faultlist_record(char *buffer, int size) {
    // conver the line contained into the fault list into a fault entry
    char c = buffer[0];
    char fault_type[128];
    FAULT_MODEL f;

    // check if the fault list line begins with # are considered as comments
    if (c == '#') {
        return NULL;
    } else {
        fault_list_entry_t *new_fault = malloc(sizeof(fault_list_entry_t));
        sscanf(buffer, "%s\t%d\t%d", fault_type, &new_fault->fault_position, &new_fault->faulty_bit); 

        // convert the fault type from string to a FAULT_MODEL type
        if ((strcmp(buffer, "no-faults") == 0) || (strcmp(buffer, "no-fault") == 0)) {
            f = NO_FAULT;
        } else if (strcmp(buffer, "permanent-fault") == 0) {
            f = PERMENANT_FAULT;
        } else if (strcmp(buffer, "stuck-at-1") == 0) {
            f = STUCK_AT_1;
        } else if (strcmp(buffer, "stuck-at-0") == 0) {
            f = STUCK_AT_0;
        } else {
            f = NO_FAULT;
        }
        
        new_fault->fault_type = f;
        return new_fault;
    }   
}

list *load_faultlist_file(char *faultlist) {
    FILE *file = fopen(faultlist, "r");

    if (file == NULL) file_error(faultlist);

    char buffer[BUFF_SIZE];
    list *fault_l = make_list();

    while (fgets(buffer, BUFF_SIZE, file) != NULL) {
        fault_list_entry_t *new_entry = convert_faultlist_record(buffer, BUFF_SIZE);
        if (new_entry != NULL) {
            list_insert(fault_l, new_entry);
        } else {
            // only for debug
            fprintf(stderr, "row not parsed");
        }
    }

    return fault_l;
}

