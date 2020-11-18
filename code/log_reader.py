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
        # MCcycles, NSpins, Temp = Info

    return Info, data

def C_V(Temp, EE, E):
    return (EE - E**2)/Temp

def Chi(Temp, MM, M_abs):
    return (MM - M_abs**2)/Temp

def plot_log(Info, data):
    MCcycles = Info[0]
    NSpins = Info[1]
    Temp = Info[2]
    Cycles, E, EE, M, MM, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]


    plt.figure(num=None, figsize=(10, 5), dpi=80, facecolor='w', edgecolor='k')
    if len(Cycles) > 100:
        linestyle = "-"
    else:
        linestyle = "-o"

    plt.subplot(231)
    plt.plot(Cycles, E/NSpins, linestyle, label = "experimental")
    plt.ylabel(r"$ \langle E \rangle $ $[J]$", fontsize=14)
    plt.xscale("log")

    plt.subplot(232)
    plt.plot(Cycles, EE/NSpins, linestyle, label = "experimental")
    plt.ylabel(r"$\langle E^2 \rangle$ $[J^2]$", fontsize=14)
    plt.xscale("log")

    plt.subplot(233)
    plt.plot(Cycles, C_V(Temp, EE, E)/NSpins, linestyle, label = "experimental")
    plt.ylabel(r"$C_V$ $[k_B]$", fontsize=14)
    plt.xscale("log")

    plt.subplot(234)
    plt.plot(Cycles, M_abs/NSpins, linestyle, label = "experimental")
    plt.xlabel("Cycles", fontsize = 14)
    plt.ylabel(r"$\langle |M| \rangle$", fontsize=14)
    plt.xscale("log")

    plt.subplot(235)
    plt.plot(Cycles, MM/NSpins, linestyle, label = "experimental")
    plt.xlabel("Cycles", fontsize = 14)
    plt.ylabel(r"$\langle M^2 \rangle$")
    plt.xscale("log")

    plt.subplot(236)
    plt.plot(Cycles, Chi(Temp, MM, M_abs)/NSpins, linestyle, label = "experimental")
    plt.xlabel("Cycles", fontsize = 14)
    plt.ylabel("$\chi$ $[k_B^{-1}]$")
    plt.xscale("log")

    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    plt.legend(loc = "best", fontsize = 13)
    plt.savefig("../article/figures/equilibrium_benchmark.pdf", bbox_inches="tight")
    plt.show()



if __name__ == "__main__":
    filename = "..."
    Info, data = read_log(filename)
