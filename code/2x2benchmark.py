from dump_reader import read_file
import numpy as np
import matplotlib.pyplot as plt


Temp, E, C_V, M, Chi, M_abs = read_file("test_run2x2")







Z = 4*np.cosh((8/Temp)) + 12
E_analytical = -32*np.sinh(8/Temp)/Z/4
M_analytical = 0*Temp
M_abs_analytical = (8*np.exp(8/Temp) + 4)/Z/4
sus_analytical = (32*np.exp(8/Temp) + 8)/(Z*Temp)/4
C_V_analytical = 32*(8*np.cosh(8/Temp)*Z -32*np.sinh(8/Temp)**2)/Z**2/4

fig, ax = plt.subplots(ncols=2, nrows=3, figsize=(8,8))
fig.tight_layout()

if len(Temp) > 100:
    linestyle = "-"
else:
    linestyle = "-o"

plt.subplot(321)
plt.title("Energy")
plt.plot(Temp, E, linestyle, label = "experimental")
plt.plot(Temp, E_analytical, linestyle, label = "analytical")

plt.subplot(322)
plt.title("M")
plt.plot(Temp, M, linestyle, label = "experimental")
plt.plot(Temp, M_analytical, linestyle, label = "analytical")


plt.subplot(323)
plt.title("M_abs")
plt.plot(Temp, M_abs, linestyle, label = "experimental")
plt.plot(Temp, M_abs_analytical, linestyle, label = "analytical")


plt.subplot(324)
plt.title("Susceptibility")
plt.plot(Temp, Chi, linestyle, label = "experimental")
plt.plot(Temp, sus_analytical, linestyle, label = "analytical")


plt.subplot(325)
plt.title("C_V")
plt.plot(Temp, C_V, linestyle, label = "experimental")
plt.plot(Temp, C_V_analytical, linestyle, label = "analytical")
plt.legend()




plt.show()
