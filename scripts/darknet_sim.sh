#!/bin/bash

# this script has been created in order to automatize the execution of a one simulation in darkent

# initialize the variable that should be used during darknet simualation
data="/home/a.ianne/documents/darknet/cfg/mnist.dataset"
network_configuration="/home/a.ianne/documents/darknet/cfg/mnist_lenet.cfg"
weight="/home/a.ianne/documents/darknet/pretrained_weights/mnist_lenet.weights"
test_set="./test_set/image_set.txt"
fault_list="./fault_list/fault_list.csv"

results_output="sim_results"

echo "begin the simulation"

if [ ! -f $fault_list ]; 
then 
    # create a new fault list file
    python fault_list/gen_fault_list.py "fault_list.csv" "stuck-at" 1024
    mv fault_list.csv fault_list
fi

if [ ! -d $results_output ]; 
then 
    # create the results dir
    mkdir $results_output
fi

# execute darknet 
./darknet simulation $data $network_configuration $weight $test_set $fault_list -layer -1

