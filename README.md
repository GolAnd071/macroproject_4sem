# Project "Snowflake"

This is the final project of a programming course at MIPT.

The aim of the project: to simulate the process of snowflake formation.

***

## Installation and running

<ins>**1. Download the repository:**</ins>

```
git clone https://github.com/GolAnd071/macroproject_4sem
```

<ins>**2. Build the project:**</ins>

```
mkdir build
cd build
cmake ..
make
```

<ins>**3. Run the executable with:**</ins>

```
./snowflake <number_of_iterations>
```

<ins>**4. View generated snapshots:**</ins>

To do this follow the instruction below:

1) Import `crystal-step-_.vtu` into ParaView
2) Apply filter "Glyph"
3) Set "Sphere" glyph type
4) Set appropriate scale (e.g. 0.5)
5) Set "state" as scale array if you want to view only frozen nodes.
6) Set coloring array to view the distribution of concentration or tempurature

Finally it should look like this:

![alt text](https://github.com/GolAnd071/macroproject_4sem/blob/master/Resources/Images/Mesh.png?raw=true)

***
