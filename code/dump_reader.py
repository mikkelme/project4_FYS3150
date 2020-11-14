import numpy as np
import matplotlib.pyplot as plt

def read_file(filename):
    data = []
    with open(filename, "r") as infile:
        variables = infile.readline().split()
        for line in infile:
            data.append(np.array(line.split()).astype(float))
        data = np.array(data)
        for i in range(len(variables)):
            variables[i] += f" = data[:,{i}]"
            eval(variables[i])

            #= data[:,i]

        # Temp  = data[:,0]
        # E = data[:,1]
        # E_var = data[:,2]
        # M = data[:,3]
        # M_var = data[:,4]
        # M_abs = data[:,5]

    return #Temp, E, E_var, M, M_var, M_abs


read_file("test_run2x2")
# Temp, E, E_var, M, M_var, M_abs = read_file("test_run2x2")
# print(Temp)
# print(E)
# print(E_var)
# print(M)
# print(M_var)
# print(M_abs)
