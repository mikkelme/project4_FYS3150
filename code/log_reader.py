import numpy as np
import matplotlib.pyplot as plt

def read_log(filename):
    data = []
    with open(filename, "r") as infile:
        Info_len = int(infile.readline().split()[-1])
        Info = [int(infile.readline().split()[-1]) for i in range(Info_len)]
        variables = infile.readline().split()
        data = np.zeros((Info[0], len(variables)))
        for i in range(Info[0]):
            data[i] = infile.readline().split()
        # Cycles, E, C_V, M, Chi, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]
        # MCcycles, Nspins, Temp = Info

    return Info, data
