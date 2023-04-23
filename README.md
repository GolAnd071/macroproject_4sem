# Project "Snowflake"

This is the final project of a programming course at MIPT.

The aim of the project: to simulate the process of snowflake formation.

***

## Installation and running

<ins>**0. Requirements:**</ins>

Our project requires [VTK](https://vtk.org) to make snapshots.

You can find installation instructions [for Linux](https://github.com/GolAnd071/inf_4sem/tree/master/lib_instrustion#vtk) and [for Windows](https://gitlab.kitware.com/vtk/vtk/-/blob/bb416fcc20ddffdbde9d92f8a372ce9215087c13/Documentation/dev/build_windows_vs.md).

Besides, we used [ParaView](https://www.paraview.org) to watch snapshots.

You can install it [from offisial website](https://www.paraview.org/download/) or to use your package manager (for Linux).

<ins>**1. Download the repository:**</ins>

```
git clone https://github.com/GolAnd071/macroproject_4sem
```

<ins>**2. Build the project:**</ins>

### Linux

```
mkdir build
cd build
cmake ..
make
```

### Windows

You may use CMake for Windows to configure project for Visual Studio and then build it from IDE.

<ins>**3. Run the executable with:**</ins>

### Linux

```
./snowflake <number_of_iterations>
```

### Windows

```
snowflake.exe <number_of_iterations>
```

<ins>**4. View generated snapshots:**</ins>

To do this follow the instruction below:

1) Import `crystal-step-_.vtu` into ParaView
2) Apply filter "Glyph"
3) Set "Sphere" glyph type
4) Set glyph mode to "All points"
5) Set appropriate scale (e.g. 0.5)
6) Set "state" as scale array if you want to view only frozen nodes
7) Set coloring array to view the distribution of concentration or tempurature

Finally it should look like this:

![alt text](https://github.com/GolAnd071/macroproject_4sem/blob/master/Resources/Images/Mesh.png?raw=true)

***
