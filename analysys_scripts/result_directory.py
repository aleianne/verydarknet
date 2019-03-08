from pathlib import Path

class ResultDirectoryHandler:

    # pass as argument to this class the directory that contains the result file
    def __init__(self, dir):
        self.dir = dir
        self.gold_pred_filename = 'golden_prediction.txt'


    def change_dir_path(self, dir):
        self.dir = dir

    # this method extract all the file contained into the directory and return a list of filename
    def extract_file(self):
        # create the path for the result directory
        result_dir_path = Path(dir)

        p = dir + '/golden_prediction.csv'
        gold_prediction_file = Path(p)

        # create a list that contains the files contained into the directory
        # add into the list only the files and exclude the golden prediction file
        files = [x for x in result_dir_path.iterdir() if (x.is_file and x != gold_prediction_file)]

        return files


    ''' this function generate the list of elements contained into the golden reults file'''
    def load_golden_results(self, file, header):

        # create a new named tuple in order to store the information about the the golden results into a list
        GoldenEntry = namedtuple('GoldenEntry', ['name', 'label', 'score'])

        # create an empty list of elements
        goldenResList = []

        # open the golden prediction file
        with file.open() as fp: 
            
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
    def load_golden_results2(self, filename, header):

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

    # this method search the golden prediction file, pass as argument the directory that contains the result file
    def search_golden_prediction(self):
        result_dir = Path(dir)

        # search the golden prediction file
        golden_prediciton_path = result_dir / golden_prediciton_path

        if (golden_prediciton_path.exists() and golden_prediciton_path.is_file()):
            print('the golden prediction file exists into the path ', golden_prediciton_path.as_posix())
        else: 
            raise: Exception('impossible to find the golden prediction file')
        
        # store the golden prediction values
        golden_results = load_golden_results2(golden_prediciton_path, True)