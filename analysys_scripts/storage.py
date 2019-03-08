

# this class should contains all the data structures useful for the analysys

class StorageArea:

    # the attributes created into this class represents the main information to be extracted 
    # in particular the first two matrix contains the wrong label computed for each target bit

    # the classification list contains the information about the classification of the outputs

    def __init__(self):
        # define the 2 matrix one for stuck-at-1 and another one for stuck-at-0
        self.matrix_sa0 = np.zeros((8, 10), dtype=int)
        self.matrix_sa1 = np.zeros((8, 10), dtype=int)

        self.classification_sa1 = list()
        self.classification_sa0 = list()

        self.cp_sa1 = [0, 0, 0, 0, 0, 0, 0, 0]
        self.wp_sa1 = [0, 0, 0, 0, 0, 0, 0, 0]

        self.cp_sa0 = [0, 0, 0, 0, 0, 0, 0, 0]
        self.wp_sa0 = [0, 0, 0, 0, 0, 0, 0, 0]

    def retrieve_matrix_sa







    