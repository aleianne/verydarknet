from pathlib import Path
from goldenPredictionFile import GoldenPredictionFile

class ResultDirectoryHandler:

    def __init__(self, dir):
        # set the path of the root directory that contains all the results
        self.dir = dir
        self.rootpath = Path(dir)

    def change_dir_path(self, dir):
        self.dir = dir
        self.rootpath = Path(dir)
   
    def extract_file(self):
        # create the path for the result directory
        result_dir_path = Path(dir)

        p = dir + '/golden_prediction.csv'
        gold_prediction_file = Path(p)

        # create a list that contains the files contained into the directory
        # add into the list only the files and exclude the golden prediction file
        files = [x for x in result_dir_path.iterdir() if (x.is_file and x != gold_prediction_file)]

        return files

    def search_golden_prediction(self):
        # search the golden prediction file
        golden_prediciton_path = result_dir / golden_prediciton_path

        if (golden_prediciton_path.exists() and golden_prediciton_path.is_file()):
            print('the golden prediction file exists into the path ', golden_prediciton_path.as_posix())
        else: 
            raise: Exception('impossible to find the golden prediction file')
        
        # store the golden prediction values
        golden_results = load_golden_results2(golden_prediciton_path, True

        # generate the 