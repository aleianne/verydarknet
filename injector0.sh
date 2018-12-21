#/bin/bash

if [ $# -eq 3]
then
	echo "wrong number of parameters..."
	exit 
fi

# set the value to be used during the simualtion
begin=$1
end=$2
quit=$(($end +1))

layer=$3

list=`cat ./data/mnist_data/mnist/images/validation_images.txt | sed -n '$begin, $end p; $quit q' `    

index=0;

echo -e "Workload\tCrit_SDC\tNON_Critc_SDC\tMSK" > LeNet5_layer2_fault_result.csv

for i in $list 
do
	#echo DEBUG $i

	wl=`echo $i | awk -F "." '{print $1}'`  
	
	echo "workload = $wl"
	echo "Create FL"
	#./src/create_list 2400  32 10  

	./darknet classifier predict -faultModel transition -layer $layer ~/documents/darknet/cfg/mnist.dataset ~/documents/darknet/cfg/mnist_lenet.cfg ~/documents/darknet/pretrained_weights/mnist_lenet.weights ~/documents/darknet/data/mnist_data/mnist/images/$i >> tmp0
	
	CritSDC=`grep CritSDC tmp0 | awk -F " " '{print $3}'`
	No_CritSDC=`grep No_Crit_SDC tmp0 | awk -F " " '{print $3}'`
	MSK=`grep MSK tmp0  | awk -F " " '{print $3}'`
	Minutes=`grep minutes tmp0 | awk -F " " '{print $3}'`

	echo -e  "$i\t$CritSDC\t$No_CritSDC\t$MSK\t$Minutes" >> LeNet5_layer2_fault_result.csv

done
