import numpy as np
import matplotlib.pyplot as plt
import sys
import seaborn as sns

plt.style.use("bmh")
sns.color_palette("hls", 1)

import matplotlib
matplotlib.rc('xtick', labelsize=14)
matplotlib.rc('ytick', labelsize=14)
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'


def read_dump(filename):
    data = []
    with open(filename, "r") as infile:
        MCcyles = int(infile.readline().split()[1])
        variables = infile.readline().split()
        for line in infile:
            data.append(np.array(line.split()).astype(float))
        data = np.array(data)
        Temp, E, EE, M, MM, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]

    return Temp, E, EE, M, MM, M_abs

def C_V(Temp, EE, E):
    return (EE - E**2)/Temp**2

def Chi(Temp, MM, M_abs):
    return (MM - M_abs**2)/Temp

def plot_results(Temp, E, EE, M, MM, M_abs, NSpins):
    plt.figure(num=None, figsize=(10, 5), dpi=80, facecolor='w', edgecolor='k')

    if len(Temp) > 100:
        linestyle = "-"
    else:
        linestyle = "-o"

    plt.subplot(231)
    plt.plot(Temp, E/NSpins**2, linestyle, label = "experimental")
    plt.xlabel("T [k/J]")
    plt.ylabel(r"$ \langle E \rangle $ $[J]$", fontsize=14)

    plt.subplot(232)
    plt.plot(Temp, EE/NSpins**4, linestyle, label = "experimental")
    plt.ylabel(r"$\langle E^2 \rangle$ $[J^2]$", fontsize=14)

    plt.subplot(233)
    plt.plot(Temp, C_V(Temp, EE, E)/NSpins**2, linestyle, label = "experimental")
    plt.ylabel(r"$C_V$ $[k_B]$", fontsize=14)

    plt.subplot(234)
    plt.plot(Temp, M_abs/NSpins**2, linestyle, label = "experimental")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
    plt.ylabel(r"$\langle |M| \rangle$", fontsize=14)

    plt.subplot(235)
    plt.plot(Temp, MM/NSpins**4, linestyle, label = "experimental")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
    plt.ylabel(r"$\langle M^2 \rangle$")

    plt.subplot(236)
    plt.plot(Temp, Chi(Temp, MM, M_abs)/NSpins**2, linestyle, label = "experimental")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
    plt.ylabel("$\chi$ $[k_B^{-1}]$")

    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    plt.legend(loc = "best", fontsize = 13)
    plt.show()


if __name__ == "__main__":
    filename = "bm2x2_dump.txt"
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    Temp, E, EE, M, MM, M_abs = read_dump(filename)
    NSpins = int(filename.split("x")[1].split("_")[0])
    plot_results(Temp, E, EE, M, MM, M_abs, NSpins)
