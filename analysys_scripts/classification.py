class Classification:

    ''' little documentation about the instance variable meaning:
            -x : is the number of label correclty detected by the network (i.e. the label is equal to the golden one and the margin between label with the highest confidence score and the second highest confidence score is higer than a given threshold)
            -y : is the number of label correclty detected by the network but the marigin between the two highest scores is below a given threshold
            -z : in the number of label wronlgy predicted by the netowork        
    ''' 

    # init the class with the parameters passed as argument
    def __init__(self, x, y, z):
        self.x = x 
        self.y = y
        self.z = z
        # define the threshold value used during the classification
        self.th = 2.0

    def delete_all_values(self):
        self.x = 0
        self.y = 0
        self.z = 0

    # this method should be used in order to classify the data read from the file
    def classify_data(self, label, confidence_score, margin, golden_label):

        if (label != golden_label):
            # in this case the two label differ so the network has predicted a wrong label
            self.update_z_value()
        elif (label == golden_label):

            if (magin >= th):
                # in this case the network has predicted the correct label and the margin is over the threshold
                self.update_x_value()
            else 
                # in this case the network has predicted the correcte label but the margin is below the given threshold
                self.update_y_value()

    # update only the single instance variable
    def update_x_value(self):
        self.x = self.x + 1

    def update_y_value(self):
        self.y = self.y + 1
    
    def update_z_value(self):
        self.z = self.z + 1

    