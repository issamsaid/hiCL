# hiCL 
[![Build Status](https://travis-ci.org/issamsaid/hiCL.svg?branch=master)](https://travis-ci.org/issamsaid/hiCL)

<b>hiCL</b> is a C/C++ and Fortran wrapper that makes it easier to use OpenCL
for scientific computing. Writing an OpenCL code evolves including hundreds 
of lines of host code. In scientific computing, host code is usually cumbersome
and very verbose in such a manner scientists would spend more time putting 
together the host code rather than focusing on accelerating their workloads
on GPUs or on any other OpenCL capable hardware.<br/>
<b>hiCL</b> extensively reduces the need to focus on the host code and offers 
a set of functionalities in C/C++ and Fortran to help efficiently exploit 
hardware accelerators for scientific computing.<br/>
<b>hiCL</b> offers a transparent way to manage memory objects on different
hardware accelerators with different memory models thanks to a set of bitwise 
flags.
A [paper](http://dl.acm.org/citation.cfm?id=2909453) about <b>hiCL</b> was 
published at the 
[International OpenCL Workshop](http://www.iwocl.org/) in Vienna, Austria 
on late April 2016.

# Getting started
The following section is a step by step guide that will take you from fetching
the source code from the repository branches to running your <b>hiCL</b> first 
examples on your machine.

## Branches and cloning
The project contains two git main branches: **master** and **develop**. 
The **master** branch only contains the major releases, and 
is intended to use the library as is.
We recommend to clone from this branch if you would like to use 
the latest stable version. 
The releases are tagged on the master branch and each version has a major
number and a minor number which are used as the tagging string (.e.g. the 
first release is tagged 1.0 on the master branch).
Cloning the master branch and checking out the latest release can
be done as follows:
```
git clone --recursive -b master https://github.com/issamsaid/hiCL.git
```
Note that the `--recursive` option is added to include all the submodules used
by hiCL to the cloned repository. 
If you wish to clone a specific release (here we use the 1.0 release as
an example) you may add:
```
pushd hiCL
git checkout tags/1.0
popd
``` 
The following table summarizes the different details about all the 
releases of the <b>hiCL</b> library:</br>

Release number (tag)  | Date         | Description                                    
--------------------- | ------------ | -----------------------------------------------
1.0                   | 06/01/2016   | The initial release of the <b>hiCL</b> library

On the other hand, the **develop** branch contains the latest builds and is
intended to be used by the developers who are willing to contribute or improve 
the library. To get started, you can clone this branch as follows:
```
git clone --recursive -b develop https://github.com/issamsaid/hiCL.git
```

## Setting up and building
The <b>hiCL</b> project has multiple components, each in a subdirectory of the
root directory (hiCL). The [src](https://github.com/issamsaid/hiCL/tree/master/src)
subdirectory is the C/C++ interface, the 
[fortran_interface](https://github.com/issamsaid/hiCL/tree/master/fortran_interface)
subdirectory is the Fortran interface, the 
[test](https://github.com/issamsaid/hiCL/tree/master/test) subdirectory contains
 the unit tests of the library, 
 the [doc](https://github.com/issamsaid/hiCL/tree/master/doc) subdirectory is 
 where the documentation of the library is to be generated,
 and the [examples](https://github.com/issamsaid/hiCL/tree/master/examples) includes a set of examples of how to use the library.
The project can be compiled using the [cmake](https://cmake.org/) utility
 as follows:
```
pushd hiCL
mkdir build
pushd build
cmake -G"Unix Makefiles" ../
popd
```
The current version of the <b>hiCL</b> library had been tested on various Linux 
workstations with the GNU, Cray and Intel compilers. Nevertheless, if you face issues 
with other compilers you are kindly invited to report them.
Note that if you are using Cray compilers you have to specify where is the 
Fortran compiler is wrapped. For example if you are using `ftn` you have to add:
```
pushd hiCL
mkdir build
pushd build
cmake -DCMAKE_Fortran_COMPILER=ftn -G"Unix Makefiles" ../
popd
```
If your installed OpenCL implementation is not found by our `cmake` 
[script](https://github.com/issamsaid/hiCL/tree/master/cmake/FindOpenCL.cmake), 
where we try to look for your implementation in the usual install directories, 
you can help setting it manually as follows:
```
export OPENCL_INCLUDE_DIR="YOUR_OPENCL_HEADERS_PATH"
export OPENCL_LIBRARY_DIR="YOUR_OPENCL_LIBRARY_PATH"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$OPENCL_LIBRARY_DIR
```
To build the library you can run the default target which compiles the C/C++ 
interface only:
```
pushd build
make hicl
popd 
```
This Makefile target will build the static library `libhiCL.a` from the C/C++ 
source files in the [src](https://github.com/issamsaid/hiCL/tree/master/src)
subdirectory. 
Besides, if you would like to build the Fortran interface additionally, 
you can do so as follows:
```
pushd build
make hicl_fortran
popd
```
This target will build another static library `libhiCL_fortran.a` from the
Fortran source files present in the 
[fortran_interface](https://github.com/issamsaid/hiCL/tree/master/fortran_interface)
subdirectory.
In order to install the libraries on the `lib` subdirectory you can run:
```
pushd build
make install
make install -C fortran_interface // This is needed due to a cmake bug
popd
```
You can also classically run the following in order to build only the 
C/C++ interface and install it:
```
pushd build
make all install
popd
```

## Generating the documentation
The documentation of the library can be generated, in the [doc](https://github.com/issamsaid/hiCL/tree/master/doc) subdirectory,
with the help of [doxygen](http://www.stack.nl/~dimitri/doxygen/) by simply running:
```
pushd build
make doc
popd
```
It is now up to you to read the documentation and check the examples in order 
to use <b>hiCL</b> to write your own OpenCL codes for scientific purposes.

## Using the library
In order to use the <b>hiCL</b> C/C++ link your code against libhiCL.a 
additionally to the OpenCL library (by adding 
`-lhiCL -lOpenCL` to your linker options), 
however if your code is based on Fortran the 
latter should linked against both the C/C++ library and the Fortran interface (
with the help of the options `-lhiCL_fortran -lhiCL -lOpenCL`).<br/>

## Testing
If you want to work with the latest build, you are invited to fetch from the 
**develop** branch. The library comes with a set of unit tests and performance 
tests (on top of the [googletest](https://github.com/google/googletest/) 
Framework) to validate the new features. You can check the unit testing 
directory [here](https://github.com/issamsaid/hiCL/tree/master/test).
The testing framework is used to thoroughly test <b>hiCL</b> in C/C++ 
([test/src](https://github.com/issamsaid/hiCL/tree/master/test/src)) and 
Fortran ([test/fortran](https://github.com/issamsaid/hiCL/tree/master/test/fortran_interface)). 
```
pushd build
make build_tests
make install -C test // This is needed due to a cmake bug
popd
```
Alternatively `make hicl_test && make install -C test/src` will only build and 
install the test suit for the C/C++ interface, and `make hicl_test_fortran && make install -C test/fortran_interface` will build and install the unit tests for the
Fortran interface.
Tests should be written for any new code, and changes should be verified to not 
break existing tests before they are submitted for review. 
To perform the tests (which are automatically built when you compile
the library) you can run:
```
pushd test
./bin/hicl_test         // for C/C++
./bin/hicl_test_fortran // for Fortran
popd
```

## Examples
The library comes with an 
[examples](https://github.com/issamsaid/hiCL/tree/master/examples)
subdirectory which contains some C/C++ and Fortran samples. Those can be built
and installed as follows:
```
pushd build
make examples
make install -C examples // This is needed due to a cmake bug
pupd
```
Alternatively `make c_examples && make install -C examples/src` will only build and 
install the C/C++ examples, and `make fortran_examples && make install -C examples/fortran_interface` will build and install the Fortran examples.
The examples binaries can be browsed in the `test/bin` subdirectory.

## Continuous integration
We use [Travis CI](https://travis-ci.org/issamsaid/hiCL) for the continuous 
integration of the <b>hiCL</b> library. The image on the top of the page is a
link to the latest build of each branch.
A build is launched after each pull request with respect to the Travis CI 
configuration file (.travis.yml).

# How to contribute
We believe that <b>hiCL</b> can be used by scientific programmers very 
efficiently. We tend to extend the functionalities of the library. For this to 
do we need your feedbacks and proposals of features and use cases.
If you are willing to contribute please visit the contributors guide
[CONTRIBUTING](https://github.com/issamsaid/hiCL/tree/master/CONTRIBUTING.md),
or feel free to contact us.

# License
<b>hiCL</b> is a free software licensed under 
[BSD](https://github.com/issamsaid/hiCL/tree/master/LICENSE.md) 
and was put together in a computer science laboratory at the [UPMC](http://www.upmc.fr). 
The project was initially funded by [Total](http://www.total.com) which is acknowledged.

# Contact
For bug report, feature requests or if you willing to contribute please 
feel free to contact Issam SAID by dropping a line to said.issam@gmail.com.