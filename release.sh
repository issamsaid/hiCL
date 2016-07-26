#!/bin/bash
##
## @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
## All rights reserved.
##
## <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
## funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permetted provided that the following conditions
## are met:
##
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. Neither the name of the UPMC nor the names of its contributors
##    may be used to endorse or promote products derived from this software
##    without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
## INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
## ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
## LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
## NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
## SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
## @file release.sh
## @author Issam SAID
## @brief A shell scirpt to automate merging specific changes from the 
## develop branch into the master branch.
##
usage() {
	echo ""
	echo "usage: release.sh [tag_name]"
	echo ""
}

if [ $# -lt 1 ] ; then 
	usage
	exit
fi
git checkout master
git checkout develop cmake doc examples fortran_interface include src CMakeLists.txt CONTRIBUTING.md LICENSE.md README.md .gitignore .travis .travis.yml
##
## Adapt the CMakeLists.txt file to the master branch.
##
sed -i '/add_subdirectory(test)/d' CMakeLists.txt
sed -i '/Build the hiCL unit testing suit/d' CMakeLists.txt
##
## Adapt the README.md for the master branch.
##
sed -i 's/branch=develop/branch=master/d' README.md
##
## Adapt .travis.yml for the master branch.
##
sed -i '/- cd ..\/test/d'                  .travis.yml 
sed -i '/- cd .\/bin\/hiCL_test/d'         .travis.yml 
sed -i '/- cd .\/bin\/hiCL_test_fortran/d' .travis.yml 
git commit -am "commit the hiCL $1 release"
git tag --force -a $1 -m "tagging the hiCL $1 release"
git push origin master
git push origin --force --tags
git checkout develop