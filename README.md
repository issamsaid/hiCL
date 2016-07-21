# hiCL
hiCL is a C/C++ and Fortran wrapper that makes it easier to use OpenCL
for scientific computing. Writing an OpenCL code evolves including hundreds 
of lines of host code. In scientific computing, host code is usually cumbersome
and very verbose in such a manner scientists would spend more time putting 
together the host code rather than focusing on accelerating their workloads
on GPUs or on any other OpenCL capable hardware.<br/>
hiCL extensively reduces the need to focus on the host code and offers a set
of functionalities in C/C++ and Fortran to help efficiently exploit hardware
accelerators for scientific computing.<br/>
hiCL offers a transparent way to manage memory objects on different hardware 
accelerators with different memory models thanks to a set of bitwise flags.
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
git checkout tags/1.0
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
To build the library you can run:
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
In order to install the build on the lib directory you can run:
```
make install
```
You can also classically run the following in order to build both interfaces
all at once (additionally to unit tests if the **develop** branch is used): 
```
make all install
```

## Generating the documentation
The documentation of the library can be generated with the help of doxygen
by simply running:
```
make doc
```
It is now up to you to read the documentation and check the examples in order 
to use hiCL to write your own OpenCL codes for scientific purposes.

## Using the library
In order to use the hiCL C/C++ link your code against libhiCL.a additionally to 
the OpenCL library (by adding 
*-lhiCL -lOpenCL* to your linker options), 
however if your code is based on Fortran the 
latter should linked against both the C/C++ library and the Fortran interface (
with the help of the options *-lhiCL_fortran -lhiCL -lOpenCL*).<br/>

# Examples
The library come with an [examples](https://github.com/issamsaid/hiCL/tree/master/examples)
directory which contains some C/C++ and Fortran samples.  
# Testing
If you want to work with the latest build, you are invited to fetch from the 
**develop** branch, which also includes a set of unit tests and performance 
tests (on top of the [googletest](https://github.com/google/googletest/) 
Framework) to validate the behavior of the library.<br/>
The testing framework is used to thoroughly test hiCL in C/C++ and Fortran. 
Tests should be written for any new code, and changes should be verified to not 
break existing tests before they are submitted for review. 
To perform the tests (which are automatically built if you are using the 
**develop** branch) you can run:
```
cd test
./bin/hiCL_test         // for C/C++
./bin/hiCL_test_fortran // for Fortran
```

# How to contribute
We believe that hiCL can be used by scientific programmers very efficiently.
We tend to extend the functionalities of the library. For this to do
we need your feedbacks and proposals of features and use cases.
If you are willing to contribute please visit the contributors guide
[CONTRIBUTING](https://github.com/issamsaid/hiCL/tree/master/CONTRIBUTING.md),
or feel free to contact us.

# License
hiCL is a free software licensed under [BSD](https://github.com/issamsaid/hiCL/tree/master/LICENSE.md) 
and was put together in a computer science laboratory at the [UPMC](www.upmc.fr). 
The project was initially funded by [Total](www.total.com) which is acknoledged.

# Contact
For bug report, feature requests or if you willing to contribute please 
feel free to contact Issam SAID by dropping a line to said.issam@gmail.com.