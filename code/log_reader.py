import numpy as np
import matplotlib.pyplot as plt

def read_log(filename):
    data = []
    with open(filename, "r") as infile:
        variables = infile.readline().split()
        for line in infile:
            data.append(np.array(line.split()).astype(float))
        data = np.array(data)
        cycles, E, M = data[:,0], data[:,1], data[:,2]

    return cycles, E, M


filename = "Metropolis_log"
cycles, E, M = read_log(filename)
plt.plot(cycles, E, label = "E")
plt.plot(cycles, M, label = "M")
plt.legend()
plt.show()
