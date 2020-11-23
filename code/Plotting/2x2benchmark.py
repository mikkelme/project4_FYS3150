from log_reader import read_log
from dump_reader import read_dump
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


def analytical_results(Temp):
    Z = 4*np.cosh((8/Temp)) + 12
    E = -32*np.sinh(8/Temp)/Z
    EE = 256*np.cosh(8/Temp)/Z
    M = 0*Temp
    MM = 32*(np.exp(8/Temp) + 1)/Z
    M_abs = (8*np.exp(8/Temp) + 16)/Z
    return Z, Temp, E, EE, M, MM, M_abs

def C_V(Temp, EE, E):
    return (EE - E**2)/Temp

def Chi(Temp, MM, M_abs):
    return (MM - M_abs**2)/Temp


def equlibrium_benchmark(Info, data):
    Temp = Info[2]
    MCcycles = Info[0]
    Cycles, E, EE, M, MM, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]
    Z, Temp, E_ana, EE_ana, M_ana, MM_ana, M_abs_ana = analytical_results(Temp)


    plt.figure(num=None, figsize=(10, 5), dpi=80, facecolor='w', edgecolor='k')
    if len(Cycles) > 100:
        linestyle = "-"
    else:
        linestyle = "-o"


    plt.subplot(231)
    plt.plot(Cycles, E/4, linestyle, label = "experimental")
    plt.hlines(E_ana/4, 0, MCcycles, color = u'#A60628', linestyle = "--", label = "analytical")
    plt.ylabel(r"$ \langle E \rangle $ $[J]$", fontsize=14)
    plt.xscale("log")

    plt.subplot(232)
    plt.plot(Cycles, EE/4**2, linestyle, label = "experimental")
    plt.hlines(EE_ana/4**2, 0, MCcycles, color = u'#A60628', linestyle = "--", label = "analytical")
    plt.ylabel(r"$\langle E^2 \rangle$ $[J^2]$", fontsize=14)
    plt.xscale("log")

    plt.subplot(233)
    plt.plot(Cycles, C_V(Temp, EE, E)/4, linestyle, label = "experimental")
    plt.hlines(C_V(Temp, EE_ana, E_ana)/4, 0, MCcycles, color = u'#A60628', linestyle = "--", label = "analytical")
    plt.ylabel(r"$C_V$ $[k_B]$", fontsize=14)
    plt.xscale("log")

    plt.subplot(234)
    plt.plot(Cycles, M_abs/4, linestyle, label = "experimental")
    plt.hlines(M_abs_ana/4, 0, MCcycles, color = u'#A60628', linestyle = "--", label = "analytical")
    plt.xlabel("Cycles", fontsize = 14)
    plt.ylabel(r"$\langle |M| \rangle$", fontsize=14)
    plt.xscale("log")

    plt.subplot(235)
    plt.plot(Cycles, MM/4**2, linestyle, label = "experimental")
    plt.hlines(MM_ana/4**2, 0, MCcycles, color = u'#A60628', linestyle = "--", label = "analytical")
    plt.xlabel("Cycles", fontsize = 14)
    plt.ylabel(r"$\langle M^2 \rangle$")
    plt.xscale("log")

    plt.subplot(236)
    plt.plot(Cycles, Chi(Temp, MM, M_abs)/4, linestyle, label = "experimental")
    plt.hlines(Chi(Temp, MM_ana, M_abs_ana)/4, 0, MCcycles, color = u'#A60628', linestyle = "--", label = "analytical")
    plt.xlabel("Cycles", fontsize = 14)
    plt.ylabel("$\chi$ $[k_B^{-1}]$")
    plt.xscale("log")

    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    plt.legend(loc = "best", fontsize = 13)
    plt.savefig("../article/figures/equilibrium_benchmark.pdf", bbox_inches="tight")
    plt.show()


def phase_benchmark(Temp, E, EE, M, MM, M_abs):
    Z, Temp, E_ana, EE_ana, M_ana, MM_ana, M_abs_ana = analytical_results(Temp)

    plt.figure(num=None, figsize=(10, 5), dpi=80, facecolor='w', edgecolor='k')
    if len(Temp) > 100:
        linestyle = "-"
    else:
        linestyle = "-o"

    plt.subplot(231)
    plt.plot(Temp, E/4, linestyle, label = "experimental")
    plt.plot(Temp, E_ana/4, label = "analytical")
    plt.xlabel("T [k/J]")
    plt.ylabel(r"$ \langle E \rangle $ $[J]$", fontsize=14)

    plt.subplot(232)
    plt.plot(Temp, EE/4**2, linestyle, label = "experimental")
    plt.plot(Temp, EE_ana/4**2, label = "analytical")
    plt.ylabel(r"$\langle E^2 \rangle$ $[J^2]$", fontsize=14)

    plt.subplot(233)
    plt.plot(Temp, C_V(Temp, EE, E)/4, linestyle, label = "experimental")
    plt.plot(Temp, C_V(Temp, EE_ana, E_ana)/4, label = "analytical")
    plt.ylabel(r"$C_V$ $[k_B]$", fontsize=14)

    plt.subplot(234)
    plt.plot(Temp, M_abs/4, linestyle, label = "experimental")
    plt.plot(Temp, M_abs_ana/4, label = "analytical")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
    plt.ylabel(r"$\langle |M| \rangle$", fontsize=14)

    plt.subplot(235)
    plt.plot(Temp, MM/4**2, linestyle, label = "experimental")
    plt.plot(Temp, MM_ana/4**2, label = "analytical")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
    plt.ylabel(r"$\langle M^2 \rangle$")

    plt.subplot(236)
    plt.plot(Temp, Chi(Temp, MM, M_abs)/4, linestyle, label = "experimental")
    plt.plot(Temp, Chi(Temp, MM_ana, M_abs_ana)/4, label = "analytical")
    plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
    plt.ylabel("$\chi$ $[k_B^{-1}]$")

    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    plt.legend(loc = "best", fontsize = 13)
    plt.savefig("../article/figures/phase_benchmark.pdf", bbox_inches="tight")
    plt.show()



if __name__ == "__main__":

    # filename = "bm2x2_log.txt"
    # Info, data = read_log(filename)
    # equlibrium_benchmark(Info, data)

    filename = "bm2x2_dump.txt"
    Temp, E, EE, M, MM, M_abs = read_dump(filename)
    phase_benchmark(Temp, E, EE, M, MM, M_abs)





#
