# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/isaid/code/hiCL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/isaid/code/hiCL/build

# Include any dependencies generated for this target.
include CMakeFiles/hiCL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hiCL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hiCL.dir/flags.make

CMakeFiles/hiCL.dir/src/hicl_base.c.o: CMakeFiles/hiCL.dir/flags.make
CMakeFiles/hiCL.dir/src/hicl_base.c.o: ../src/hicl_base.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/isaid/code/hiCL/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/hiCL.dir/src/hicl_base.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiCL.dir/src/hicl_base.c.o   -c /home/isaid/code/hiCL/src/hicl_base.c

CMakeFiles/hiCL.dir/src/hicl_base.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiCL.dir/src/hicl_base.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/isaid/code/hiCL/src/hicl_base.c > CMakeFiles/hiCL.dir/src/hicl_base.c.i

CMakeFiles/hiCL.dir/src/hicl_base.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiCL.dir/src/hicl_base.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/isaid/code/hiCL/src/hicl_base.c -o CMakeFiles/hiCL.dir/src/hicl_base.c.s

CMakeFiles/hiCL.dir/src/hicl_base.c.o.requires:
.PHONY : CMakeFiles/hiCL.dir/src/hicl_base.c.o.requires

CMakeFiles/hiCL.dir/src/hicl_base.c.o.provides: CMakeFiles/hiCL.dir/src/hicl_base.c.o.requires
	$(MAKE) -f CMakeFiles/hiCL.dir/build.make CMakeFiles/hiCL.dir/src/hicl_base.c.o.provides.build
.PHONY : CMakeFiles/hiCL.dir/src/hicl_base.c.o.provides

CMakeFiles/hiCL.dir/src/hicl_base.c.o.provides.build: CMakeFiles/hiCL.dir/src/hicl_base.c.o

CMakeFiles/hiCL.dir/src/hicl_timer.c.o: CMakeFiles/hiCL.dir/flags.make
CMakeFiles/hiCL.dir/src/hicl_timer.c.o: ../src/hicl_timer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/isaid/code/hiCL/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/hiCL.dir/src/hicl_timer.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiCL.dir/src/hicl_timer.c.o   -c /home/isaid/code/hiCL/src/hicl_timer.c

CMakeFiles/hiCL.dir/src/hicl_timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiCL.dir/src/hicl_timer.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/isaid/code/hiCL/src/hicl_timer.c > CMakeFiles/hiCL.dir/src/hicl_timer.c.i

CMakeFiles/hiCL.dir/src/hicl_timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiCL.dir/src/hicl_timer.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/isaid/code/hiCL/src/hicl_timer.c -o CMakeFiles/hiCL.dir/src/hicl_timer.c.s

CMakeFiles/hiCL.dir/src/hicl_timer.c.o.requires:
.PHONY : CMakeFiles/hiCL.dir/src/hicl_timer.c.o.requires

CMakeFiles/hiCL.dir/src/hicl_timer.c.o.provides: CMakeFiles/hiCL.dir/src/hicl_timer.c.o.requires
	$(MAKE) -f CMakeFiles/hiCL.dir/build.make CMakeFiles/hiCL.dir/src/hicl_timer.c.o.provides.build
.PHONY : CMakeFiles/hiCL.dir/src/hicl_timer.c.o.provides

CMakeFiles/hiCL.dir/src/hicl_timer.c.o.provides.build: CMakeFiles/hiCL.dir/src/hicl_timer.c.o

CMakeFiles/hiCL.dir/src/hicl_util.c.o: CMakeFiles/hiCL.dir/flags.make
CMakeFiles/hiCL.dir/src/hicl_util.c.o: ../src/hicl_util.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/isaid/code/hiCL/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/hiCL.dir/src/hicl_util.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiCL.dir/src/hicl_util.c.o   -c /home/isaid/code/hiCL/src/hicl_util.c

CMakeFiles/hiCL.dir/src/hicl_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiCL.dir/src/hicl_util.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/isaid/code/hiCL/src/hicl_util.c > CMakeFiles/hiCL.dir/src/hicl_util.c.i

CMakeFiles/hiCL.dir/src/hicl_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiCL.dir/src/hicl_util.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/isaid/code/hiCL/src/hicl_util.c -o CMakeFiles/hiCL.dir/src/hicl_util.c.s

CMakeFiles/hiCL.dir/src/hicl_util.c.o.requires:
.PHONY : CMakeFiles/hiCL.dir/src/hicl_util.c.o.requires

CMakeFiles/hiCL.dir/src/hicl_util.c.o.provides: CMakeFiles/hiCL.dir/src/hicl_util.c.o.requires
	$(MAKE) -f CMakeFiles/hiCL.dir/build.make CMakeFiles/hiCL.dir/src/hicl_util.c.o.provides.build
.PHONY : CMakeFiles/hiCL.dir/src/hicl_util.c.o.provides

CMakeFiles/hiCL.dir/src/hicl_util.c.o.provides.build: CMakeFiles/hiCL.dir/src/hicl_util.c.o

CMakeFiles/hiCL.dir/src/hicl_mem.c.o: CMakeFiles/hiCL.dir/flags.make
CMakeFiles/hiCL.dir/src/hicl_mem.c.o: ../src/hicl_mem.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/isaid/code/hiCL/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/hiCL.dir/src/hicl_mem.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiCL.dir/src/hicl_mem.c.o   -c /home/isaid/code/hiCL/src/hicl_mem.c

CMakeFiles/hiCL.dir/src/hicl_mem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiCL.dir/src/hicl_mem.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/isaid/code/hiCL/src/hicl_mem.c > CMakeFiles/hiCL.dir/src/hicl_mem.c.i

CMakeFiles/hiCL.dir/src/hicl_mem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiCL.dir/src/hicl_mem.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/isaid/code/hiCL/src/hicl_mem.c -o CMakeFiles/hiCL.dir/src/hicl_mem.c.s

CMakeFiles/hiCL.dir/src/hicl_mem.c.o.requires:
.PHONY : CMakeFiles/hiCL.dir/src/hicl_mem.c.o.requires

CMakeFiles/hiCL.dir/src/hicl_mem.c.o.provides: CMakeFiles/hiCL.dir/src/hicl_mem.c.o.requires
	$(MAKE) -f CMakeFiles/hiCL.dir/build.make CMakeFiles/hiCL.dir/src/hicl_mem.c.o.provides.build
.PHONY : CMakeFiles/hiCL.dir/src/hicl_mem.c.o.provides

CMakeFiles/hiCL.dir/src/hicl_mem.c.o.provides.build: CMakeFiles/hiCL.dir/src/hicl_mem.c.o

CMakeFiles/hiCL.dir/src/hicl_knl.c.o: CMakeFiles/hiCL.dir/flags.make
CMakeFiles/hiCL.dir/src/hicl_knl.c.o: ../src/hicl_knl.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/isaid/code/hiCL/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/hiCL.dir/src/hicl_knl.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiCL.dir/src/hicl_knl.c.o   -c /home/isaid/code/hiCL/src/hicl_knl.c

CMakeFiles/hiCL.dir/src/hicl_knl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiCL.dir/src/hicl_knl.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/isaid/code/hiCL/src/hicl_knl.c > CMakeFiles/hiCL.dir/src/hicl_knl.c.i

CMakeFiles/hiCL.dir/src/hicl_knl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiCL.dir/src/hicl_knl.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/isaid/code/hiCL/src/hicl_knl.c -o CMakeFiles/hiCL.dir/src/hicl_knl.c.s

CMakeFiles/hiCL.dir/src/hicl_knl.c.o.requires:
.PHONY : CMakeFiles/hiCL.dir/src/hicl_knl.c.o.requires

CMakeFiles/hiCL.dir/src/hicl_knl.c.o.provides: CMakeFiles/hiCL.dir/src/hicl_knl.c.o.requires
	$(MAKE) -f CMakeFiles/hiCL.dir/build.make CMakeFiles/hiCL.dir/src/hicl_knl.c.o.provides.build
.PHONY : CMakeFiles/hiCL.dir/src/hicl_knl.c.o.provides

CMakeFiles/hiCL.dir/src/hicl_knl.c.o.provides.build: CMakeFiles/hiCL.dir/src/hicl_knl.c.o

CMakeFiles/hiCL.dir/src/hicl_dev.c.o: CMakeFiles/hiCL.dir/flags.make
CMakeFiles/hiCL.dir/src/hicl_dev.c.o: ../src/hicl_dev.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/isaid/code/hiCL/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/hiCL.dir/src/hicl_dev.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiCL.dir/src/hicl_dev.c.o   -c /home/isaid/code/hiCL/src/hicl_dev.c

CMakeFiles/hiCL.dir/src/hicl_dev.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiCL.dir/src/hicl_dev.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/isaid/code/hiCL/src/hicl_dev.c > CMakeFiles/hiCL.dir/src/hicl_dev.c.i

CMakeFiles/hiCL.dir/src/hicl_dev.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiCL.dir/src/hicl_dev.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/isaid/code/hiCL/src/hicl_dev.c -o CMakeFiles/hiCL.dir/src/hicl_dev.c.s

CMakeFiles/hiCL.dir/src/hicl_dev.c.o.requires:
.PHONY : CMakeFiles/hiCL.dir/src/hicl_dev.c.o.requires

CMakeFiles/hiCL.dir/src/hicl_dev.c.o.provides: CMakeFiles/hiCL.dir/src/hicl_dev.c.o.requires
	$(MAKE) -f CMakeFiles/hiCL.dir/build.make CMakeFiles/hiCL.dir/src/hicl_dev.c.o.provides.build
.PHONY : CMakeFiles/hiCL.dir/src/hicl_dev.c.o.provides

CMakeFiles/hiCL.dir/src/hicl_dev.c.o.provides.build: CMakeFiles/hiCL.dir/src/hicl_dev.c.o

# Object files for target hiCL
hiCL_OBJECTS = \
"CMakeFiles/hiCL.dir/src/hicl_base.c.o" \
"CMakeFiles/hiCL.dir/src/hicl_timer.c.o" \
"CMakeFiles/hiCL.dir/src/hicl_util.c.o" \
"CMakeFiles/hiCL.dir/src/hicl_mem.c.o" \
"CMakeFiles/hiCL.dir/src/hicl_knl.c.o" \
"CMakeFiles/hiCL.dir/src/hicl_dev.c.o"

# External object files for target hiCL
hiCL_EXTERNAL_OBJECTS =

libhiCL.a: CMakeFiles/hiCL.dir/src/hicl_base.c.o
libhiCL.a: CMakeFiles/hiCL.dir/src/hicl_timer.c.o
libhiCL.a: CMakeFiles/hiCL.dir/src/hicl_util.c.o
libhiCL.a: CMakeFiles/hiCL.dir/src/hicl_mem.c.o
libhiCL.a: CMakeFiles/hiCL.dir/src/hicl_knl.c.o
libhiCL.a: CMakeFiles/hiCL.dir/src/hicl_dev.c.o
libhiCL.a: CMakeFiles/hiCL.dir/build.make
libhiCL.a: CMakeFiles/hiCL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libhiCL.a"
	$(CMAKE_COMMAND) -P CMakeFiles/hiCL.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hiCL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hiCL.dir/build: libhiCL.a
.PHONY : CMakeFiles/hiCL.dir/build

CMakeFiles/hiCL.dir/requires: CMakeFiles/hiCL.dir/src/hicl_base.c.o.requires
CMakeFiles/hiCL.dir/requires: CMakeFiles/hiCL.dir/src/hicl_timer.c.o.requires
CMakeFiles/hiCL.dir/requires: CMakeFiles/hiCL.dir/src/hicl_util.c.o.requires
CMakeFiles/hiCL.dir/requires: CMakeFiles/hiCL.dir/src/hicl_mem.c.o.requires
CMakeFiles/hiCL.dir/requires: CMakeFiles/hiCL.dir/src/hicl_knl.c.o.requires
CMakeFiles/hiCL.dir/requires: CMakeFiles/hiCL.dir/src/hicl_dev.c.o.requires
.PHONY : CMakeFiles/hiCL.dir/requires

CMakeFiles/hiCL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hiCL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hiCL.dir/clean

CMakeFiles/hiCL.dir/depend:
	cd /home/isaid/code/hiCL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/isaid/code/hiCL /home/isaid/code/hiCL /home/isaid/code/hiCL/build /home/isaid/code/hiCL/build /home/isaid/code/hiCL/build/CMakeFiles/hiCL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hiCL.dir/depend

