# hiCL
hiCL is a C/C++ and Fortran wrapper that makes it easier to use OpenCL
for scientific computing. Writing an OpenCL code evolves including hundreds 
of lines of host code. In scientific computing, host code is usually cumbersome
and very verbose in such a matter scientists would spent more time putting 
together the host code rather than focusing on accelerating their workloads
on GPUs or other OpenCL capable hardware.<br/>
hiCL extensively reduces the need to focus on the host code and offers a set
of functionalities in C/C++ and Fortran to help efficiently exploit hardware
accelerators for scientific computing.<br/>
hiCL offers a transparent way to manage memory objects on different hardware 
accelerators with different memory models thanks to a set of bitwise 
flags.
A [paper](http://dl.acm.org/citation.cfm?id=2909453) about hiCL was published
at the International OpenCL Workshop (IWOCL 2016) in Vienna, Austria on late
April 2016.

# Getting started
The following section is a step by step guide that will take you from fetching
the source code from the repository branches to running your hiCL first examples
on your machine.

## Branches and cloning
The project contains two git main branches: **master** and **develop**. 
The **master** branch only contains the major releases, and 
is intended to use the library as is.
We recommend to clone from this branch if you would like to use 
the latest stable version. 
The current version of the software is 1.0.
The releases are tagged on the master branch and each version has a major
number and a minor number which are used as the tagging string (.e.g. the 
first release is tagged 1.0 on the master branch).
Cloning the master branch and checking out the first release can
be done as follows:
```
git clone -b master https://github.com/issamsaid/hiCL.git
git checkout 1.0
```

On the other hand, the **develop** branch contains the latest builds and is
intended to be used by the developers who are willing to contribute or improve 
the library. To get started, you can clone this branch as follows:
```
git clone -b develop https://github.com/issamsaid/hiCL.git
```
The **develop** branch has a unit testing directory for both the C/C++ and the
Fortran interfaces of the library.

## Setting up and compiling
The library can be compiled using cmake as follows:
```
cd hiCL
mkdir build
cd build
cmake -G"Unix Makefiles" ../
```
The current version of the hiCL library had been tested on various Linux 
workstations.
If your installed OpenCL implementation is not found by the cmake script you 
can help setting it manually as follows:
```
export OPENCL_INCLUDE_DIR="YOUR_OPENCL_HEADERS_PATH"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"YOUR_OPENCL_LIBRARY_PATH"
```

```
make hiCL
```
This Makefile target will build the static library libhiCL.a from the C/C++ 
source files in the [src](https://github.com/issamsaid/hiCL/tree/master/src)
directory. 
If instead you would like to build the Fortran interface, you can do so as
 follows:
```
make hiCL_fortran
```
This target will build another static library libhiCL_fortran.a from the Fortran
source files present in the 
[fortran_interface](https://github.com/issamsaid/hiCL/tree/master/fortran_interface)
directory.<br/>
In order to use the hiCL C/C++ link your code against libhiCL.a (by adding 
*-lhiCL -lOpenCL* to your linker options), however if your code is based on Fortran the 
latter should linked against both the C/C++ library and the Fortran interface (
with the help of the options *-lhiCL_fortran -lhiCL -lOpenCL*).<br/>

```
make install
```


```
make all install
```

# Examples

# Testing
If you want to work with the latest build, you are invited to fetch from the 
**develop** branch, which also includes a set of unit tests and performance 
tests (on top of the [googletest](https://github.com/google/googletest/) 
Framework) to validate the behavior of the library.<br/>
In order to build the core C/C++ library of hiCL you may run:

A testing framework is of no good if itself is not thoroughly tested. 
Tests should be written for any new code, and changes should be verified to not 
break existing tests before they are submitted for review. 
To perform the tests, follow the instructions in README and verify that 
there are no failures.
```
make install
```

# How to contribute

# Contact
For bug report, feature requests or if you willing to contribute please 
feel free to contact Issam SAID by dropping a line to said.issam@gmail.com.