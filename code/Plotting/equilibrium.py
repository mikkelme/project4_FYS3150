from log_reader import*
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

plt.style.use("bmh")
sns.color_palette("hls", 1)

import matplotlib
matplotlib.rc('xtick', labelsize=14)
matplotlib.rc('ytick', labelsize=14)
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'


def plot_equilibrium(filenames):
    for filename in filenames:
        Info, data = read_log(filename)
        MCcycles = Info[0]
        NSpins = Info[1]
        Temp = Info[2]
        Cycles, E, EE, M, MM, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]

        text = filename.strip("eq").split("_")
        label = r"T = " + text[0] + " $[k_B/J]$ " + "(" + text[1].strip("20x20") + ")"


        figE = plt.figure(num=0, dpi=80, facecolor='w', edgecolor='k')
        plt.plot(Cycles, E/NSpins**2, label = label)
        plt.xlabel("MC cycles", fontsize = 14)
        plt.ylabel(r"$ \langle E \rangle $ $[J]$", fontsize=14)
        plt.legend(loc = "best", fontsize = 13)
        plt.xscale("log")

        figM = plt.figure(num=1, dpi=80, facecolor='w', edgecolor='k')
        plt.plot(Cycles, M_abs/NSpins**2, label = label)
        plt.xlabel("MC cycles", fontsize = 14)
        plt.ylabel(r"$\langle |M| \rangle$", fontsize=14)
        plt.legend(loc = "best", fontsize = 13)
        plt.xscale("log")

    figE.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    figE.savefig("../article/figures/equilibrium_energy.pdf", bbox_inches="tight")
    figM.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    figM.savefig("../article/figures/equilibrium_magnetization.pdf", bbox_inches="tight")
    plt.show()





if __name__ == "__main__":
    filenames = [   "eq1.0_ordered20x20_log.txt",
                    "eq1.0_random20x20_log.txt",
                    "eq2.4_ordered20x20_log.txt",
                    "eq2.4_random20x20_log.txt",    ]



    plot_equilibrium(filenames)
