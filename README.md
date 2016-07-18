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
# Testing

# Examples

# How to contribute

# Contact
Issam SAID: said.issam@gmail.com