import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import subprocess
from scipy import signal
import statsmodels.api as sm
from matplotlib.legend_handler import HandlerLine2D, HandlerTuple
import copy
import sys
sys.path.append("..")
from dump_reader import*


def plot_phase(filenames):

    smooth = False
    window_length = 47
    polyorder = 3
    for filename in filenames:
        Temp, E, EE, M, MM, M_abs = read_dump(filename)
        NSpins = int(filename.split("x")[1].split("_")[0])
        linestyle = "o"


        figE = plt.figure(num=0, dpi=80, facecolor='w', edgecolor='k')
        plotE = plt.plot(Temp, E/NSpins**2, linestyle, label = f"L = {NSpins}")
        plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
        plt.ylabel(r"$ \langle E \rangle $ $[J]$", fontsize=14)
        plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
        if smooth:
            plt.plot(Temp, smooth_curve(E/NSpins**2, window_length, polyorder), alpha = 0.5, color = plotE[0].get_color())
        plt.legend(loc = "best", fontsize = 13)

        figCV = plt.figure(num=1, dpi=80, facecolor='w', edgecolor='k')
        plotCV = plt.plot(Temp, C_V(Temp, EE, E)/NSpins**2, linestyle, label = f"L = {NSpins}")
        plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
        plt.ylabel(r"$C_V$ $[k_B]$", fontsize=14)
        plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
        if smooth:
            plt.plot(Temp, smooth_curve(C_V(Temp, EE, E)/NSpins**2, window_length, polyorder), alpha = 0.5, color = plotCV[0].get_color())
        plt.legend(loc = "best", fontsize = 13)

        figM = plt.figure(num=2, dpi=80, facecolor='w', edgecolor='k')
        plotM = plt.plot(Temp, M_abs/NSpins**2, linestyle, label = f"L = {NSpins}")
        plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
        plt.ylabel(r"$\langle |M| \rangle$", fontsize=14)
        plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
        if smooth:
            plt.plot(Temp, smooth_curve(M_abs/NSpins**2, window_length, polyorder), alpha = 0.5, color = plotM[0].get_color())
        plt.legend(loc = "best", fontsize = 13)

        figCHI = plt.figure(num=3, dpi=80, facecolor='w', edgecolor='k')
        plotCHI = plt.plot(Temp, Chi(Temp, MM, M_abs)/NSpins**2, linestyle, label = f"L = {NSpins}")
        plt.xlabel(r"$T$ $[k_B/J]$", fontsize=14)
        plt.ylabel("$\chi$ $[k_B^{-1}]$")
        plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
        plt.legend(loc = "best", fontsize = 13)
        if smooth:
            plt.plot(Temp, smooth_curve(Chi(Temp, MM, M_abs)/NSpins**2, window_length, polyorder), alpha = 0.5, color = plotCHI[0].get_color())

    """
    figE.savefig("../../article/figures/phase_E.pdf", bbox_inches="tight")
    figCV.savefig("../../article/figures/phase_CV.pdf", bbox_inches="tight")
    figM.savefig("../../article/figures/phase_M.pdf", bbox_inches="tight")
    figCHI.savefig("../../article/figures/phase_CHI.pdf", bbox_inches="tight")
    """
    plt.show()


def smooth_curve(input, window_length = 47, polyorder = 3):

    output = signal.savgol_filter(input, window_length, polyorder)
    # plt.plot(input, "o")
    # plt.plot(output)
    # plt.show()
    return output


def findTc(filenames):
    numFiles = len(filenames)
    T_numerical = np.zeros((numFiles,2))
    L_reci = np.zeros(numFiles)
    for i in range(numFiles):
        filename = filenames[i]
        Temp, E, EE, M, MM, M_abs = read_dump(filename)
        NSpins = int(filename.split("x")[1].split("_")[0])
        C_V_array = C_V(Temp, EE, E)/NSpins**2
        Chi_array = Chi(Temp, MM, M_abs)/NSpins**2
        max_idx_C_V = np.argmax(smooth_curve(C_V_array))
        max_idx_Chi = np.argmax(smooth_curve(Chi_array))

        T_numerical[i,0] = Temp[max_idx_C_V]
        T_numerical[i,1] = Temp[max_idx_Chi]
        L_reci[i] = 1/NSpins



    value_list = ["Heat Capacity", "Susceptibility"]
    plt.figure(num=0, dpi=80, facecolor='w', edgecolor='k')
    x_plot = np.array([0, np.max(L_reci[0])])
    ax = plt.gca()
    b_arr = np.zeros(2)
    b_err_arr = np.zeros(2)

    for j in range(2):
        y = T_numerical[:,j]
        x = sm.add_constant(L_reci)
        model = sm.OLS(y, x)
        res = model.fit()

        b, a = res.params
        b_err, a_err = res.bse
        #print(value_list[j])
        #print("a = %g\na_err = %g\nb = %g\nb_err = %g\n" %(a, a_err, b, b_err))
        b_arr[j] = b
        b_err_arr[j] = b_err

        plot_data, = plt.plot(L_reci, y, "o", label = "Data points")
        plot_fit, = plt.plot(x_plot, x_plot*a + b, linestyle = "--", color = plot_data.get_color(), alpha = 0.8, label = "Linear fit")
        plot_b, = plt.plot(0, b, linestyle = "None", marker = "*", color = plot_data.get_color(), label = "Intercept")

        plt.xlabel("1/L", fontsize = 14)
        plt.ylabel(r"$T_C$ $[k_B/J]$")

        if j == 0:
            handles = [copy.copy(plot_data), copy.copy(plot_fit), copy.copy(plot_b)]
            for h in handles:
                h.set_color("grey")
            leg0 = ax.legend(loc = "lower right", fontsize = 13, handles = handles)
            leg1 = ax.legend([(plot_data, plot_fit)], [value_list[j]], numpoints = 1, handler_map ={tuple: HandlerTuple(ndivide=None)}, fontsize = 13,  bbox_to_anchor = (0.35, 1))
        else:
            leg2 = ax.legend([(plot_data, plot_fit)], [value_list[j]], numpoints = 1, handler_map ={tuple: HandlerTuple(ndivide=None)}, fontsize = 13,  bbox_to_anchor = (0.34, 0.9) )


    print(f"Mean result\n b = {np.mean(b_arr):.4f} +- {np.sqrt(b_err_arr[1]**2 + b_err_arr[1]**2):.4f}")
    ax.add_artist(leg1)
    ax.add_artist(leg0)
    plt.xlabel("1/L", fontsize = 14)
    plt.ylabel(r"$T_C$ $[k_B/J]$")
    plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
    #plt.savefig("../../article/figures/findTc.pdf", bbox_inches="tight")
    plt.show()










if __name__ == "__main__":
    filenames = [   "final40x40_dump.txt",
                    "final60x60_dump.txt",
                    "final80x80_dump.txt",
                    "final100x100_dump.txt"]

    plot_phase(filenames)
    findTc(filenames)
