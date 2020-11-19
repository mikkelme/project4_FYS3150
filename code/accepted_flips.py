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

def plot_results(filename):
    with open(filename, "r") as infile:
        T = []; AccFlips = [];
        for line in infile:
            val = line.split()
            T.append(float(val[0]))
            AccFlips.append(float(val[1]))
    T = np.array(T)
    AccFlips = np.array(AccFlips)

    plt.figure(num=None, dpi=80, facecolor='w', edgecolor='k')
    plt.plot(T, AccFlips, "o")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize = 14)
    plt.ylabel("Accepted Flips [%]", fontsize = 14)

    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    plt.savefig("../article/figures/accepted_flips.pdf", bbox_inches="tight")
    plt.show()


if __name__ == "__main__":
    filename = "accflips_flipped.txt"
    plot_results(filename)
