from fileLoader import FileLoader

class GoldenPredictionFile(FileLoader):

    def __init__(self, filename, dir):
        super(GoldenPredictionFile, self).__init__(filename, dir)
        
        # define a new dictionary used to store the information about the golden results
        # the dictionary has this structure->  image_name: label_predicted
        self.gold_res = dict()

    def load_golden_results(self, file, header):
        ''' this function generate a dictionary that is able to find label predicted 
            during the golden prediction, gived the image name. This method simplify the 
            subsequent operations of golden predioction retrieval '''

        # check if the golden prediction file exists into the filesystem
        if (self.check_file() == False):
            raise: FileNotFoundError("impossible to find file " + super(GoldenPredictionFile, self).filename)

        with open(file, "r") as fp: 

            if (header == True): 
                next(fp)

            for line in fp:
                # strip the line contained into the file
                line_s = line.rstrip('\n')
                tokens = line_s.split('\t')
            
                if (len(tokens) != 3):
                    raise Exception('impossible to complete the file analysys')
                
                # update the map with the image name as the key and the label predicted as the value
                self.gold_res[tokens[0]] = tokens[1]
            
        return self.gold_res

    def get_golden_result(self):
        return self.gold_res

    def change_file_info(self, dir, filename):
        self.change_file_info(dir, filename)