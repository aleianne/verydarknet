from classicifaction import Classification
import numpy as np


''' 
    this function is the entry point of all the analysys
'''
def analize_result_files():


''' 
    this function open the file received from input and generate all the data statistics
'''
def convert_result_file(filename, header, golden_results):

    # define the threshold value that should be used to characterize the reult obtained 
    threshold = 2.0

    # define the image name as the filename passed as argument to this function minus the txt extension
    f = filename.split('.')
    imagename = f[0]

    # check if the imagename begin with t_ or v_
    check1 = imagename.startswith('t_')
    check2 = imagename.startswith('v_')

    if (check1 == False and check2 == False):
        raise Exception('input file not recognized')

    # retrieve the golden prediction saved into the golden results
    golden_label = golden_results.get(imagename)

    if (golden_label == None):
        raise Exception('impossible to find the golden prediction associated to this image')

    with open(filename, 'r') as fp: 
        # check if the file contains the header, if true skip the first line
        if (header = True): 
            header_s = fp.readline()

            # strip the line from the end line character
            header1 = header_s.rstrip('\n')
            header2 = header_s.split('\t')

            # save the number of the header items
            items = len(header2)

        for line in fp:
            # stript and split the line into tokens than contain information stored into the file
            line_s = line.rstrip('\n')
            tokens = line_s.split('\t')
            
            if (items != len(tokens) and items != 0):
                raise Exception('error into the tokens list, inconsitent number of elements')

            if (tokens[0] == 'stuck-at-1'):
                # add the stuck-at-0 list
            elif (tokens[0] == 'stuck-at-0'):
                # add the stuck-at-1 list



            collect_results_for_bit(items)
            collect_faulty_predictions(items)

def init_classification_matrix():

    # create two matrix that should contains the label predicted for each faulty bit position
    # the first one is used for s-a-1
    # the second one is usde for s-a-0
    matrix1 = np.zeros((8, 10), dtype=int)
    matrix0 = np.zeros((8, 10), dtype=int)

''' 
    this method collect the result from the result file and store the information 
    into a list of Classification object, the index of the list is the target bit of the injection
''' 
def collect_results_for_bit(items, classificationList1, golden_label):

    # save tokens into specific variables
    fault_model = tokens[0]
    position = int(tokens[1])
    bit = int(tokens[2])
    label = int(tokens[3])
    confidence_score = float(tokens[4]) 
    margin = float(tokens[5])

    if (bit > 8 or bit < 1):
        raise Exception('bit position is not valid')

    # add the data line read from the file into the classification list passed as argument
    c1 = classificationList1[bit]
    c1.classify_data(label, confidence_score, margin, golden_label)


def collect_faulty_predictions(items, matrix, golden_prediction):
    # collect the faulty prediction 
    # take the label matrix and than save the label predicted
    fault_model = tokens[0]
    position = int(tokens[1])
    bit = int(tokens[2])
    label = int(tokens[3])
    confidence_score = float(tokens[4])

    # update the value contained intp the frequency matrix only if the prediction is different from the golded
    if (golden_prediction != label):
        value = matrix[bit][label]
        matrix[bit][label] = value + 1


if __name__ = "__main__": 
    analyze_result_files()