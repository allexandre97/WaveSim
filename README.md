# Welcome to WaveSim, a package to perform simulations of 2D Waves!

## Introduction.

This is a small package able to numerically solve the wave equation in a 2D plane. The philosophy behind its usage is loosely inspired by programs like [GROMACS](https://www.gromacs.org/) or [Gaussian](https://gaussian.com/), where you give an input file to the software to then obtain simulation results.

For now, the program is able to perform simulations on flat, square surfaces. The program builds user-defined kernels as starting conditions and solves the [differential equation describing simple waves](https://en.wikipedia.org/wiki/Wave_equation) for a specified number of steps. The solver is based on the [Finite Difference Method](https://en.wikipedia.org/wiki/Finite_difference_method), more specifically using the centered difference scheme. The only boundary conditions that the solver accepts so far are [Periodic](https://en.wikipedia.org/wiki/Periodic_boundary_conditions) ones, but most surely at least a reflective case like [Neumann conditions](https://en.wikipedia.org/wiki/Neumann_boundary_condition) will be added.

Here is an example usage of the program:
```WaveSim -f input.inp -t 6```
This command will take the input parameters defined on ```input.inp``` and perform the simulation using 6 OpenMP threads. The input file can have any name and extension yu want, but should follow some writing conventions so the program can read them, namely: ```;``` symbols are interpreted as comments and therefore anything after them in the same line are not parsed to the program, and the parameters are defined as ```<paramenter> = <value>```. If a parameter can accept more than one value, these should be separated by just blank spaces. The available parameters are:

- DIMENSION (int)    --> The lateral size in points for the simulation space.
- KERNELS   (string) --> The type of kernel function to use for the initial condition. Accepts more than one value. So far the only available kernels are a modified version of the [sinc function](https://en.wikipedia.org/wiki/Sinc_function) and the radial [sine function](https://en.wikipedia.org/wiki/Sine_and_cosine). 
- X0        (float)  --> The center in X for the kernel function. Accepts more than one value.
- Y0        (float)  --> The center in Y for the kernel function. Accepts more than one value.
- FREQUENCY (float)  --> The frequency of the kernel. Accepts more than one value.
- DECAY     (float)  --> The decay for the kernel. Accepts more than one value.
- COURANT   (float)  --> The [Courant number](https://en.wikipedia.org/wiki/Courant%E2%80%93Friedrichs%E2%80%93Lewy_condition), governs the "speed" of the simulation. Should not be bigger than 1 to ensure numerical stability and if set that way it will be downscaled to 0.95.
- NSTEPS    (int)    --> The number of steps to perform the simulation for.
- NOUT      (int)    --> The number of steps between file saves.

Some of this parameters can also be passed to the program at run time by providing it with some flags. The flags **always** have priority over what was defined in the input file. In order to get an explanation on the available flags and how to use them you can call ```WaveSim -h``` or ```WaveSim --help```.

As explained before, the program saves its progress by creating some files. So far, the output files are .bmp files with a terrible colormap depicting the wave as well as ~~totally not .txt~~ .dat files containing the values of the wave separated by commas. An auxiliary python script is also provided to make prettier pictures using said .dat files. By running ```python BuildGraphs.py -h``` you can get info on how to use the script. The files are saved to a folder called FRAMES in the execution directory and their base name can be changed by setting the ```-w <filename>``` or ```--writefile <filename>``` flag at run time. In the case this is not set, it defaults to "out".

A sample file is provided so you can get a feel of how to use this program.
## Compiling and Building.

### Dependencies:

This software was built on a **linux** machine and I cannot guarantee it will work on other OSs, in fact, I believe it probably won't. The program was made to be compiled using [**CMake**](https://cmake.org/). You also need [**a c++ compiler**](https://gcc.gnu.org/) that can work with c++ 17 standard. The only external library that this software uses is [**fmt**](https://github.com/fmtlib/fmt).

### Compile instructions:

Once you have installed the needed dependencies you can clone this repository to your compute either by using
```git clone <project_link>``` or by downloading the .zip and extracting it. Once downloaded enter the file and you can follow the typical build sequence for CMake:

```
mkdir build && cd build
cmake ..
make
sudo make install
```

Note that depending on the compiler you use the program might run faster or slower. I'd recommend setting their compiler flags by passing ```cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++``` which are the ones that gave me the best results. You can also specify the installation path by setting the ```-DCMAKE_INSTALL_PREFIX=<path>``` flag.

Finally, and if you don't want the inconvinience of typing the full path to the executable every time to run the program, make sure to [add an alias](https://linuxhint.com/bash_alias/) to your .bashrc file.

## Disclaimer.

This is a project I'm working on during my free time and therefore cannot have all the attention that I'd like to give to it. It is quite literally my first C++ project, so if you have any criticism of it or any tip/recommendation don't hesitate to tell me. And of course, fell free to modify, change and butcher the code in any way you'd see fit, as I'm sure there is a lot of room for improvement!
