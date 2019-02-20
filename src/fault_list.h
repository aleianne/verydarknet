#include "darknet.h"
#include "list.h"
#include "utils.h"

#define BUFF_SIZE 1024 // 1MB buffer

fault_list_entry_t *convert_faultlist_record(char *buffer, int size);
list *load_faultlist_file(char *faultlist);