#! /bin/bash

# delete all the possible directory and files used during the darknet simulation

results_output="sim_results"
fault_list="fault_list/fault_list.csv"

echo "begin to clean darknet environment"

if [ -f $fault_list ];
then 
    echo "delete the fault list" $fault_list
    rm $fault_list
fi

if [ -d $results_output ];
then 
    echo "delete the output folder" $results_output
    rm -rf $results_output
fi

echo "cleaned correclty!"