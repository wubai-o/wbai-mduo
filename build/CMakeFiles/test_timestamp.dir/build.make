# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/study_shared_ptr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/study_shared_ptr/build

# Include any dependencies generated for this target.
include CMakeFiles/test_timestamp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_timestamp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_timestamp.dir/flags.make

CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o: CMakeFiles/test_timestamp.dir/flags.make
CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o: ../tests/test_timestamp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/study_shared_ptr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o -c /root/study_shared_ptr/tests/test_timestamp.cc

CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/study_shared_ptr/tests/test_timestamp.cc > CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.i

CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/study_shared_ptr/tests/test_timestamp.cc -o CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.s

CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.requires:

.PHONY : CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.requires

CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.provides: CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.requires
	$(MAKE) -f CMakeFiles/test_timestamp.dir/build.make CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.provides.build
.PHONY : CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.provides

CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.provides.build: CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o


# Object files for target test_timestamp
test_timestamp_OBJECTS = \
"CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o"

# External object files for target test_timestamp
test_timestamp_EXTERNAL_OBJECTS =

../bin/test_timestamp: CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o
../bin/test_timestamp: CMakeFiles/test_timestamp.dir/build.make
../bin/test_timestamp: ../lib/libstudy.so
../bin/test_timestamp: CMakeFiles/test_timestamp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/study_shared_ptr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_timestamp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_timestamp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_timestamp.dir/build: ../bin/test_timestamp

.PHONY : CMakeFiles/test_timestamp.dir/build

CMakeFiles/test_timestamp.dir/requires: CMakeFiles/test_timestamp.dir/tests/test_timestamp.cc.o.requires

.PHONY : CMakeFiles/test_timestamp.dir/requires

CMakeFiles/test_timestamp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_timestamp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_timestamp.dir/clean

CMakeFiles/test_timestamp.dir/depend:
	cd /root/study_shared_ptr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/study_shared_ptr /root/study_shared_ptr /root/study_shared_ptr/build /root/study_shared_ptr/build /root/study_shared_ptr/build/CMakeFiles/test_timestamp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_timestamp.dir/depend

