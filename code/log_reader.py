import numpy as np
import matplotlib.pyplot as plt

def read_log(filename):
    data = []
    with open(filename, "r") as infile:
        Info_len = int(infile.readline().split()[-1])
        Info = [float(infile.readline().split()[-1]) for i in range(Info_len)]
        MCcycles = int(Info[0])
        variables = infile.readline().split()

        data = np.zeros((MCcycles, len(variables)))
        for i in range(MCcycles):
           data[i] = infile.readline().split()
        # Cycles, E, EE, M, MM, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]
        # MCcycles, Nspins, Temp = Info

    return Info, data


if __name__ == "__main__":
    filename = "..."
    Info, data = read_log(filename)
