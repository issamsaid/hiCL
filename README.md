# hiCL
hiCL is a C/C++ and Fortran wrapper that makes it easier to use OpenCL
for scientific computing. Writing an OpenCL code evolves including hundreds 
of lines of host code. In scientific computing, host code is usually cumbersome
and very verbose in such a matter scientists would spent more time putting 
together the host code rather than focusing on accelerating their workloads
on GPUs or other OpenCL capable hardware.<br/>
hiCL extensively reduces the need to focus on the host code and offers a set
of functionalities in C/C++ and Fortran to help efficiently exploit hardware
accelerators for scientific computing.

# Getting started
To get started, you can clone the hiCL repository and build it using cmake like
follows:
```
git clone https://github.com/issamsaid/hiCL.git
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
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH"YOUR_OPENCL_LIBRARY_PATH"
```
The repository contains two git branches: ##master## and ##develop##. 
The ##master## branch
only covers the tagged releases of the library, we recommend to clone from this 
branch if you would like to use the latest stable version. 
The current version of the software is 1.2 (which is inline with the highest 
version of OpenCL that we support).
If you want to work with the latest build, you are invited to fetch from the 
develop branch, which also includes a set of unit tests and performance tests
(on top of the @googletest Framework) to validate the behavior of the library.
# Testing

# Examples

# How to contribute

# Contact
For bug report, feature requests or if you willing to contribute please 
feel free to contact Issam SAID by dropping a line to said.issam@gmail.com.