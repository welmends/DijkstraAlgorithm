# Dijkstra Algorithm
Implementation of Dijstra Algorithm

## Getting Started
This repository provides an C/C++ implementation of Dijstra Algorithm

### Prerequisites
  - GCC (any version)

### Compilation
- Enter into the repository folder and type the following commands:

```shell
make clean
make
```

- All the executables files will be generated

### Running
Here we tell how to run this program

- Call the executable by typing at terminal on project folder

```shell
./ep1.o '<path_to_txtFile>/filename.txt'
```

- The .txt file must have the following pattern

**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; n &nbsp;&nbsp;m &nbsp;&nbsp;s &nbsp;&nbsp;t  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; u<sub>1</sub> &nbsp;v<sub>1</sub> &nbsp;c<sub>1</sub>  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; u<sub>2</sub> &nbsp;v<sub>2</sub> &nbsp;c<sub>2</sub>  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; . . .   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; u<sub>m</sub> v<sub>m</sub> c<sub>m</sub>**

where,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **n** is the vertices number;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **m** is the arrows number;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **s** is the origin vertice;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **t** is the destination vertice;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **u<sub>i</sub>** and **v<sub>1</sub>** are the origin and destination of the arrow **i**;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **c<sub>1</sub>** is the cost of the arrow **i**.


#### Behavior
For an input as follows:

```shell
./ep1.o 'example1.txt'
```
![Ilustration](https://user-images.githubusercontent.com/19287934/55241837-c177ec00-521a-11e9-8dcb-d02834f915e7.png)

We have the following output:

```
> Digraph:
5 7
1 2 5
1 5 10
2 3 4
2 4 13
2 5 2
3 4 7
5 4 6

> Minimum Path [1 to 4]: [1]->[2]->[5]->[4]
```
