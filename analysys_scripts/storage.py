import Classification from classification.py

# this class should contains all the data structures useful for the analysys

class StorageArea:

    # the attributes created into this class represents the main information to be extracted 
    # in particular the first two matrix contains the wrong label computed for each target bit

    # the classification list contains the information about the classification of the outputs

    def __init__(self):
        # define the 2 matrix one for stuck-at-1 and another one for stuck-at-0
        self.matrix_sa0 = np.zeros((8, 10), dtype=int)
        self.matrix_sa1 = np.zeros((8, 10), dtype=int)

        self.classification_sa1 = self.generate_classification_matrix()
        self.classification_sa0 = self.generate_classification_matrix()

        self.cp_sa1 = [0, 0, 0, 0, 0, 0, 0, 0]
        self.wp_sa1 = [0, 0, 0, 0, 0, 0, 0, 0]

        self.cp_sa0 = [0, 0, 0, 0, 0, 0, 0, 0]
        self.wp_sa0 = [0, 0, 0, 0, 0, 0, 0, 0]

    
    ''' 
        this method should be used two times, one for each stuck-at
    '''
    def generate_classification_matrix(self):

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


    def get_matrix_sa1(self):
        return self.matrix_sa1

    def get_matrix_sa0(self):
        return self.matrix_sa0

    def get_classification_sa0(self):
        return self.classification_sa0

    def get_classification_sa1(self):
        return self.classification_sa1

    def get_cp_sa1(self):
        return self.cp_sa1

    def get_wp_sa1(self):
        return self.wp_sa1
    
    def get_cp_sa0(self):
        return self.cp_sa0

    def get_wp_sa0(self):
        return self.wp_sa0






    