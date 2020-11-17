from log_reader import read_log
import numpy as np
import matplotlib.pyplot as plt



def benchmark(Info, data):
    Temp = Info[2]
    MCcycles = Info[0]
    Cycles, E, C_V, M, Chi, M_abs = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4], data[:,5]

    Z = 4*np.cosh((8/Temp)) + 12
    E_analytical = -32*np.sinh(8/Temp)/Z/4
    M_analytical = 0*Temp
    M_abs_analytical = (8*np.exp(8/Temp) + 4)/Z/4
    sus_analytical = (32*np.exp(8/Temp) + 8)/(Z*Temp)/4
    C_V_analytical = 32*(8*np.cosh(8/Temp)*Z -32*np.sinh(8/Temp)**2)/Z**2/4

    fig, ax = plt.subplots(ncols=2, nrows=3, figsize=(8,8))
    fig.tight_layout()
    plt.subplots_adjust(hspace = 0.40)

    if MCcycles > 100:
        linestyle = "-"
    else:
        linestyle = "-o"

    plt.subplot(321)
    plt.title("Energy")
    plt.plot(Cycles, E, linestyle, label = "experimental")
    plt.xlabel("Cycles")
    plt.hlines(E_analytical, 0, MCcycles, label = "analytical")


    plt.subplot(322)
    plt.title("M")
    plt.plot(Cycles, M, linestyle, label = "experimental")
    plt.hlines(M_analytical, 0, MCcycles, label = "analytical")
    plt.xlabel("Cycles")


    plt.subplot(323)
    plt.title("M_abs")
    plt.plot(Cycles, M_abs, linestyle, label = "experimental")
    plt.hlines( M_abs_analytical, 0, MCcycles, label = "analytical")
    plt.xlabel("Cycles")


    plt.subplot(324)
    plt.title("Susceptibility")
    plt.plot(Cycles, Chi, linestyle, label = "experimental")
    plt.hlines(sus_analytical, 0, MCcycles, label = "analytical")
    plt.xlabel("Cycles")


    plt.subplot(325)
    plt.title("C_V")
    plt.plot(Cycles, C_V, linestyle, label = "experimental")
    plt.hlines(C_V_analytical, 0, MCcycles, label = "analytical")
    plt.legend()
    plt.xlabel("Cycles")


    plt.show()





if __name__ == "__main__":

    filename = "Sampling_log"
    Info, data = read_log(filename)
    benchmark(Info, data)
