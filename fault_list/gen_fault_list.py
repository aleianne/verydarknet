def generate_fault_list(fault_list_name, fault_model,  component_num):
    ''' This function generate the fault list '''

    i = int(component_num)

    if (fault_model == "stuck-at"): 
        generate_stuck_at_fault_list(fault_list_name, i)
    else:
        print("no others fault model has been implemented")


def generate_stuck_at_fault_list(fault_list_name, component_num): 
    ''' This function generate the stuck at fault list '''

    print("begin to create the fault list file")

    with open(fault_list_name, "w") as fp: 
        for i in range(component_num): 
            for j in range(23, 31):
                # generate the string to be insert into the fault list file
                fault_list_entry = "stuck-at-1\t" + str(i) + "\t" + str(j) + "\n"
                fp.write(fault_list_entry)


    print("fault list generated correclty")
    fp.close()


if __name__ == "__main__":
    import sys
    generate_fault_list(sys.argv[1], sys.argv[2], sys.argv[3])