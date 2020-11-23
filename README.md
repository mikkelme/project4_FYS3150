# FYS 3150 - Project 4
This repository contains the work of Fredrik Hoftun and Mikkel Metzsch Jensen for project 4 in Computational Physics FYS3150. 

The repository is divided into two folders: article and code. In the article folder we have the article it self and all the figures included in it. In the code folder we have the 
...

To use the compiler just type `make` in the commandline in the `/code` directory.
The compiler will generate a file `main.exe` that can be used in the command line to generate a file `system.data` which contains positions, velocities, kinetic-, potential-, mechanical energy and time.

Usage of `main.exe` in Bash, the arguments are optional: `$ ./main.exe dt numTimesteps B`, where dt defines the time-step in years, numTimesteps is the number of said timesteps so that the total time becomes `T = dt * numTimesteps` and B is the value between 2.0 and 3.0 that defines the inverse beta force.

Example: `$ ./main.exe` runs the program with `dt = 0.001`, `numTimesteps = 1000` and `B = 2.0`.

Another example: `$ ./main.exe 0.001 10000 2.5` runs the program with `dt = 0.001`, `numTimesteps = 10000` and `B = 2.5`.

We have included compiled files which were used to calculate our results below.

# Use of testfiles
| .exe-files       | input variables     |  description | Compiler used |
| ------------- |:-------------:|:-------------:|:-------------:|
| EarthSun_Euler.exe | 1: dt, 2: numTimesteps | Earth-Sun system, circular orbit, sun fixed, Euler | Native macOS (Clang) |
| EarthSun_Verlet.exe | 1: dt, 2: numTimesteps | Earth-Sun system, circular orbit, sun fixed, Velocity Verlet | Clang |
| Euler_timing.exe | 1: dt, 2: numTimesteps | Earth-Sun system, circular orbit, sun fixed, Euler, no data output other than timing | Clang |
| Verlet_timing.exe | 1: dt, 2: numTimesteps| Earth-Sun system, circular orbit, sun fixed, Euler, no data output other than timing | Clang |
| All_planets.exe  | 1: dt, 2: numTimesteps | All planets system, elliptical orbit, sun unfixed, Velocity Verlet | GCC |
| Earth_sun_Jupiter.exe | 1: dt, 2: numTimesteps | Earth-Sun-Jupiter system, elliptical orbit, sun unfixed, Velocity Verlet | GCC |
| Earth.exe| 1: dt, 2: numTimesteps | Earth-Sun system, elliptical orbit, sun fixed, Velocity Verlet | GCC |
| EarthSun_escape.exe | 1: dt, 2: numTimesteps, 3: test velocity | Simplified Earth-Sun system to find escape velocity | Clang |
| EarthSun_InverseBeta_ellipse.exe | 1: dt, 2: numTimesteps, 3: Beta | Earth-Sun system, elliptical orbit, inverse beta force | Clang |
| Jupiter.exe | 1: dt, 2: numTimesteps | Earth-Sun-Jupiter system, elliptical orbit, sun fixed, Velocity Verlet | GCC |
| Jupiter10.exe | 1: dt, 2: numTimesteps | Earth-Sun-Jupiter_with_10x_mass system, elliptical orbit, sun fixed, Velocity Verlet | GCC |
| Jupiter1000.exe | 1: dt, 2: numTimesteps | Earth-Sun-Jupiter_with_1000x_mass system, elliptical orbit, sun fixed, Velocity Verlet | GCC |
| MercurySun_precession_correction.exe | 1: dt, 2: numTimesteps | Mercury-Sun system, corrected gravitational force (general relativity) | Clang |
| MercurySun_precession_newton.exe | 1: dt, 2: numTimesteps | Mercury-Sun system, newtonian gravitational force | Clang |

