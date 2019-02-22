#!/bin/bash

# this script has been created in order to automatize the execution of a one simulation in darkent

# initialize the variable that should be used during darknet simualation

data="../cfg/mnist.dataset"
network_configuration="../cfg/mnist_lenet.cfg"
weight="../pretrained_weights/mnist_lenet.weights"
test_set="../data/mnist_data/mnist/images/validation_images.txt"
fault_list="../fault_list/fault_list.csv"

echo "begin the simulation"

# execute darknet 
../darknet simulation $data $network_configuration $weight $test_set $fault_list -layer -1

