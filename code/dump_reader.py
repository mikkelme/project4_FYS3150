import numpy as np
import matplotlib.pyplot as plt

def read_file(filename):
    data = []
    with open(filename, "r") as infile:
        variables = infile.readline().split()
        for line in infile:
            data.append(np.array(line.split()).astype(float))
        data = np.array(data)
        Temp, E, C_V, M, Chi, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]

    return Temp, E, C_V, M, Chi, M_abs


def plot_results(Temp, E, C_v, M, Chi, M_abs):
    fig, ax = plt.subplots(ncols=2, nrows=3, figsize=(8,8))
    fig.tight_layout()

    if len(Temp) > 100:
        linestyle = "-"
    else:
        linestyle = "-o"

    plt.subplot(321)
    plt.title("Energy")
    plt.plot(Temp, E, linestyle, label = "experimental")

    plt.subplot(322)
    plt.title("M")
    plt.plot(Temp, M, linestyle, label = "experimental")

    plt.subplot(323)
    plt.title("M_abs")
    plt.plot(Temp, M_abs, linestyle, label = "experimental")

    plt.subplot(324)
    plt.title("Susceptibility")
    plt.plot(Temp, Chi, linestyle, label = "experimental")

    plt.subplot(325)
    plt.title("C_V")
    plt.plot(Temp, C_V, linestyle, label = "experimental")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    filename = "test_run2x2.data"
    Temp, E, C_V, M, Chi, M_abs = read_file(filename)
    plot_results(Temp, E, C_V, M, Chi, M_abs)
