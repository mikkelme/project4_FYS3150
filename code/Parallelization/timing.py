import numpy as np
import matplotlib.pyplot as plt
import subprocess
import seaborn as sns
import statsmodels.api as sm

plt.style.use("bmh")
sns.color_palette("hls", 1)

import matplotlib
matplotlib.rc('xtick', labelsize=14)
matplotlib.rc('ytick', labelsize=14)
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'

def clear_file(name):
    filename = name + "_timing.txt"
    with open(filename, "w") as outfile:
        outfile.write("")


def run_timing(name, cores, NSpins, MCcycles, InitialTemp, FinalTemp, TempStep):
    exe_file = "./main.exe"
    subprocess.call(["mpirun", "-n", str(cores), exe_file, name, str(NSpins), str(MCcycles), str(InitialTemp), str(FinalTemp), str(TempStep)])

def read_file(name):
    filename = name + "_timing.txt"
    total_NSpins = []
    MCcycles = []
    Temp = []
    timeused = []
    with open(filename, "r") as infile:
        for line in infile:
            data = line.split()
            total_NSpins.append(int(data[0]))
            MCcycles.append(int(data[1]))
            Temp.append(float(data[2]))
            timeused.append(float(data[3]))
    return np.array(total_NSpins), np.array(MCcycles), np.array(Temp), np.array(timeused)


def RunIncreasingNSpins(NSpins_start, NSpins_end, numRuns, cores):
    name = "tmSpins"
    name += str(cores)
    InitialTemp = FinalTemp = TempStep = 1
    MCcycles = 1000000
    clear_file(name)
    for NSpins in np.linspace(NSpins_start, NSpins_end, numRuns):
        print(f"Running: NSpins = {NSpins}")
        run_timing(name, cores, NSpins, MCcycles, InitialTemp, FinalTemp, TempStep)
        print("")

def RunIncreasingMCcycles(MCcycles_start, MCcycles_end, numRuns, cores):
    name = "tmMCcycles"
    name += str(cores)
    InitialTemp = FinalTemp = TempStep = 1
    NSpins = 20
    clear_file(name)
    for MCcycles in np.linspace(MCcycles_start, MCcycles_end, numRuns):
        print(f"Running: MCcycles = {MCcycles}")
        run_timing(name, cores, NSpins, MCcycles, InitialTemp, FinalTemp, TempStep)
        print("")

def RunIncreasingTemp(InitialTemp, FinalTemp, TempStep, cores):
    name = "tmTemp"
    name += str(cores)

    NSpins = 20
    MCcycles = 1000000
    clear_file(name)
    print(f"Running: InitialTemp = {InitialTemp}, FinalTemp = {FinalTemp}, TempStep = {TempStep}")
    run_timing(name, cores, NSpins, MCcycles, InitialTemp, FinalTemp, TempStep)
    print("")



def PlotResults(names):
    for name in names:
        list = read_file(name)
        varname = ["Total Spins", "MC cycles", "$T$ $[k_B/J]$"]

        var_idx = np.nan
        for i in range(len(list)-1):
            if all(np.equal(list[i], list[i][0])) == False:
                var_idx = i

        if name[-1] == "1":
            label = "Not Parallelized"
        elif name[-1] == "4":
            label = "Parallelized"


        plt.figure(num=var_idx, dpi=80, facecolor='w', edgecolor='k')
        plotTime = plt.plot(list[var_idx], list[-1], "o", label = label)

        if var_idx != 2:
            y = list[-1]
            x = list[var_idx]
            x = sm.add_constant(x)
            model = sm.OLS(y, x)
            res = model.fit()
            b, a = res.params
            b_err, a_err = res.bse
            plt.plot(x, x*a + b, "--", alpha = 0.7, color = plotTime[0].get_color())

        # if var_idx != 2:
        #     y = list[-1]
        #     x = list[var_idx]
        #     if var_idx == 1:
        #         # y = np.log(y)
        #         # x = np.log(x)
        #         x = sm.add_constant(x)
        #         model = sm.OLS(y, x)
        #         res = model.fit()
        #         b, a = res.params
        #         b_err, a_err = res.bse
        #         plt.plot(x, x*a + b, "--", alpha = 0.7, color = plotTime[0].get_color())
        #         #plt.plot(np.exp(x), np.exp(x*a + b), "--", alpha = 0.7, color = plotTime[0].get_color())
        #     else:
        #         x = sm.add_constant(x)
        #         model = sm.OLS(y, x)
        #         res = model.fit()
        #         b, a = res.params
        #         b_err, a_err = res.bse
        #         plt.plot(x, x*a + b, "--", alpha = 0.7, color = plotTime[0].get_color())


        print(var_idx, label, a, b)
        plt.xlabel(varname[var_idx], fontsize = 14)
        plt.ylabel("Time used [s]", fontsize = 14)
        # if var_idx == 1:
        #     plt.xscale("log")
        #     plt.yscale("log")
        #     plt.xlim(left = 50)
        #     plt.ylim(bottom = 0.00025)

        plt.tight_layout(pad=1.1, w_pad=0.7, h_pad=0.2)
        plt.legend(loc = "best", fontsize = 13)

    figname = ["tmSpins", "tmMCcycles", "tmTemp"]
    for i in range(3):
        plt.figure(num=i, dpi=80, facecolor='w', edgecolor='k')
        plt.savefig("../../article/figures/" + figname[i] + ".pdf", bbox_inches="tight")






if __name__ == "__main__":
    #RunIncreasingNSpins(NSpins_start = 20, NSpins_end = 100, numRuns = 5, cores = 4)
    #RunIncreasingNSpins(NSpins_start = 20, NSpins_end = 100, numRuns = 5, cores = 1)
    #RunIncreasingMCcycles(MCcycles_start = 100, MCcycles_end = 1e6, numRuns = 10, cores = 1)
    #RunIncreasingTemp(InitialTemp = 0.5, FinalTemp = 4, TempStep = 0.1, cores = 1)

    names = ["tmSpins1", "tmSpins4", "tmMCcycles1", "tmMCcycles4", "tmTemp1", "tmTemp4"]
    PlotResults(names)
    plt.show()








    #
