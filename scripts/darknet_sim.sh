#!/bin/bash

# this script has been created in order to automatize the execution of a one simulation in darkent

# initialize the variable that should be used during darknet simualation
data="cfg/mnist.dataset"
network_configuration="cfg/mnist_lenet.cfg"
weight="/pretrained_weights/mnist_lenet.weights"
test_set="data/mnist_data/mnist/images/validation_images.txt"
fault_list="fault_list/fault_list.csv"

results_output="sim_results"

echo "begin the simulation"

# change the current directory into the darknet directory
cd ..

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
./darknet simulation $data $network_configuration $weight $test_set $fault_list -layer 0

