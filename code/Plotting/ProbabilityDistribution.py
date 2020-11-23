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



def plot_histrogram(filenames):
    equilibrium_time = int(1e4)


    def bins(Temp):
        if Temp == 1: return 8
        elif Temp == 2.4: return 50

    plt.figure(num=None, dpi=80, facecolor='w', edgecolor='k')
    for filename in filenames:
        E_states = []
        with open(filename, "r") as infile:
            Temp = float(infile.readline().split()[1])
            NSpins = int(infile.readline().split()[1])
            for line in infile:
                E_states.append(float(line.split()[1]))
        E_states = np.array(E_states)/NSpins**2


        plt.hist(E_states[equilibrium_time:], bins=bins(Temp), label=f"T = {Temp:.1f} " + r"$[J/k_B]$", log=True, histtype='bar', ec='black',stacked=True, density=True)
    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    plt.legend(loc = "best", fontsize = 13)
    plt.xlabel(r"$E [J]$", fontsize = 14)
    plt.ylabel("Probability (Normalized)", fontsize = 14)
    # plt.savefig("../article/figures/probability_distribution.pdf", bbox_inches="tight")
    plt.show()



        #plt.hist(energy2[first_15:], bins=50, alpha=0.60, label=r"T=2.4", histtype='bar', ec='black', normed=True)










if __name__ == "__main__":
    filenames = [ "pd1.0_EnergyStates.txt", "pd2.4_EnergyStates.txt"]
    plot_histrogram(filenames)
