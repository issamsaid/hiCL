# Guidelines for repository contributions
First of all thank you for taking the time or considering to contribute.
**hiCL** is a dynamic project since it is lying on top of OpenCL
whose standard is in a continuous evolution. Therefor 
we are actively looking for volunteers to receive feedbacks and feature 
requests and more importantly to help us improve the library.
any help from the  scientific community is more than welcome.</br>
The following is a set of guidelines for contributing to **hiCL**. These
are not strict rules, use your best judgment and feel free to propose changes
in a pull request.

## Possible rooms to improvement
We are planning to improve the following features of the **hiCL** library:
* OpenCL 2.0 compliance.
* The lookup performance of the hiCL descriptors (kernels, devices, 
  objects).
* The Fortran calls to run and execute kernels.
* Coverage tests and functional tests.
* A C++ templated version of the library: hiCL++.
This is a non-exhaustive list of features and it is subject to changes constantly.

## How can I contribute
You are invited to fork the **develop**  branch and put together your
modifications, suggestions and bug fixes into pull requests (requests to merge
your code from your own branch into the develop branch).
The unit tests of all your modifications should be also 
provided. Each of your pull requests should pass the build 
on [Travis CI](https://travis-ci.org/issamsaid/hiCL), before getting merged
into the **develop** branch. Once we believe that we reached a milestone a new
release will be merged on the **master** branch from the **develop** branch.