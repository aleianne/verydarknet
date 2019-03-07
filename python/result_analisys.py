from classicifaction import Classification


def convert_result_file(filename, header):
    ''' this function open the file received from input and generate all the statistics needed '''

    
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

    with open(filename, "r") as fp: 

        items = 0

        fault_model = ''
        position = 0
        label = 0

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
                raise Exception("error into the tokens list, inconsitent number of elements")

            if (tokens[0] == 'stuck-at-1'):
                # add the stuck-at-0 list
            elif (tokens[0] == 'stuck-at-0'):
                # add the stuck-at-1 list



            collect_results_for_bit(items)
            collect_faulty_predictions(items)


''' this function should be used two times, one for each stuck-at'''
def generate_classification_matrix():

    # this implementation is not good because the namedtuple is unmutable object
    '''# fare attenzione ai nomi utilizzati per la classificazione
    resClassification = namedtuple('resClassification', [])

    # generate the list that should contains the information 
    newList = []

    for i in range(10):
        # create a new entry 
        e1 = resClassification(0, 0, 0)
        newList.append(e1)

    return newList '''

    newList = []

    for i in range(10):
        c = Classification(0, 0, 0)
        newList.append(c)

    return newList

        
# the threshold is not useful into this new logic
def collect_results_for_bit(items, threshold, classificationList1):

    # save tokens into specific variables

    fault_model = tokens[0]
    position = int(tokens[1])
    bit = int(tokens[2])
    label = int(tokens[3])
    confidence_score = float(tokens[4]) 
    margin = float(tokens[5])


    # add the data line read from the file into the classification list passed as argument

    








def collect_faulty_predictions(items, threshold):

    fault_model = tokens[0]
    position = int(tokens[1])
    bit = int(tokens[2])
    label = int(tokens[3])
    confidence_score = float(tokens[4])
        

''' this function generate the list of elements contained into the golden reults file'''
# al posto di fare una lista di golden prediction posso fare la stasse cosa che farei in java utilizzando le mappe 
def load_golden_results(filename, header):

    # create a new named tuple in order to store the information about the the golden results into a list
    GoldenEntry = namedtuple('GoldenEntry', ['name', 'label', 'score'])

    # create an empty list of elements
    goldenResList = []

    # open the golden prediction file
    with open(filename, "r") as fp: 
        
        if (header == True):
            next(fp)

        for line in fp:
            # strips the input line 
            line_s = line.rstrip('\n')
            tokens = line_s.split('\t')

            entry = GoldenEntry(tokens[0], int(tokens[1]), int(tokens[2]))
            goldenResList.append(entry) 

    return goldenResList


''' this second function generate a dictionary that is able to find label predicted 
    during the golden prediction, gived the image name. This method simplify the 
    subsequent operations of golden predioction retrieval '''
def load_golden_results2(filename, header):

    # create an empty dictionary in order to store the values contained into the golden prediction file
    # the dictionary has this structure image_name: label_predicted
    gold_res = {}

    with open(filename, "r") as fp: 

        if (header == True): 
            next(fp)

        for line in fp:
            
            # strip the line contained into the file
            line_s = line.rstrip('\n')
            tokens = line_s.split('\t')
        
            if (len(tokens) != 3):
                raise Exception('impossible to complete the file analysys')
            
            # update the map with the image name as the key and the label predicted as the value
            gold_res[tokens[0]] = tokens[1]
        
    return gold_res



# conviene creare una classe che gestisca le informazioni necessarie per la classificazione