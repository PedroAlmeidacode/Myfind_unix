# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /snap/clion/112/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/112/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pedro/Desktop/Myfind_unix

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pedro/Desktop/Myfind_unix/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Myfind.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Myfind.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Myfind.dir/flags.make

CMakeFiles/Myfind.dir/main.c.o: CMakeFiles/Myfind.dir/flags.make
CMakeFiles/Myfind.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pedro/Desktop/Myfind_unix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Myfind.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Myfind.dir/main.c.o   -c /home/pedro/Desktop/Myfind_unix/main.c

CMakeFiles/Myfind.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Myfind.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pedro/Desktop/Myfind_unix/main.c > CMakeFiles/Myfind.dir/main.c.i

CMakeFiles/Myfind.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Myfind.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pedro/Desktop/Myfind_unix/main.c -o CMakeFiles/Myfind.dir/main.c.s

CMakeFiles/Myfind.dir/lib_util.c.o: CMakeFiles/Myfind.dir/flags.make
CMakeFiles/Myfind.dir/lib_util.c.o: ../lib_util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pedro/Desktop/Myfind_unix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Myfind.dir/lib_util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Myfind.dir/lib_util.c.o   -c /home/pedro/Desktop/Myfind_unix/lib_util.c

CMakeFiles/Myfind.dir/lib_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Myfind.dir/lib_util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pedro/Desktop/Myfind_unix/lib_util.c > CMakeFiles/Myfind.dir/lib_util.c.i

CMakeFiles/Myfind.dir/lib_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Myfind.dir/lib_util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pedro/Desktop/Myfind_unix/lib_util.c -o CMakeFiles/Myfind.dir/lib_util.c.s

# Object files for target Myfind
Myfind_OBJECTS = \
"CMakeFiles/Myfind.dir/main.c.o" \
"CMakeFiles/Myfind.dir/lib_util.c.o"

# External object files for target Myfind
Myfind_EXTERNAL_OBJECTS =

Myfind: CMakeFiles/Myfind.dir/main.c.o
Myfind: CMakeFiles/Myfind.dir/lib_util.c.o
Myfind: CMakeFiles/Myfind.dir/build.make
Myfind: CMakeFiles/Myfind.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pedro/Desktop/Myfind_unix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Myfind"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Myfind.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Myfind.dir/build: Myfind

.PHONY : CMakeFiles/Myfind.dir/build

CMakeFiles/Myfind.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Myfind.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Myfind.dir/clean

CMakeFiles/Myfind.dir/depend:
	cd /home/pedro/Desktop/Myfind_unix/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pedro/Desktop/Myfind_unix /home/pedro/Desktop/Myfind_unix /home/pedro/Desktop/Myfind_unix/cmake-build-debug /home/pedro/Desktop/Myfind_unix/cmake-build-debug /home/pedro/Desktop/Myfind_unix/cmake-build-debug/CMakeFiles/Myfind.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Myfind.dir/depend

