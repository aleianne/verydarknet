class FileLoader():

    def __init__(self, filename, dir):
        # create a new instance variable in the fileloader
        if (isinstance(dir, Path)):
            self.dir = dir
        elif (isinstance(dir, basestring)):
            self.dir = Path(dir)
        else 
            raise: Exception("object type not recognized")

        self.filename
        # generate the new filepath
        self.filepath = self.dir / self.filename


    def check_file(self):
        # check if the filename exists
        if (self.filepaht.exits()):
            if (self.filepath.is_file() == False):
                return False
        else: 
                return False

    def change_file_information(self, dir, filename):
        if (isinstance(dir, Path) == False):
            self.dir = dir
        else:
            self.dir = Path(dir)
    
        self.filename = filename
        self.filepath = self.dir / self.filename

        


