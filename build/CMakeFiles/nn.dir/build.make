# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rafael/Documents/workspace/neural_net

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rafael/Documents/workspace/neural_net/build

# Include any dependencies generated for this target.
include CMakeFiles/nn.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/nn.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nn.dir/flags.make

CMakeFiles/nn.dir/src/data.c.o: CMakeFiles/nn.dir/flags.make
CMakeFiles/nn.dir/src/data.c.o: ../src/data.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/nn.dir/src/data.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nn.dir/src/data.c.o   -c /home/rafael/Documents/workspace/neural_net/src/data.c

CMakeFiles/nn.dir/src/data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nn.dir/src/data.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rafael/Documents/workspace/neural_net/src/data.c > CMakeFiles/nn.dir/src/data.c.i

CMakeFiles/nn.dir/src/data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nn.dir/src/data.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rafael/Documents/workspace/neural_net/src/data.c -o CMakeFiles/nn.dir/src/data.c.s

CMakeFiles/nn.dir/src/data.c.o.requires:

.PHONY : CMakeFiles/nn.dir/src/data.c.o.requires

CMakeFiles/nn.dir/src/data.c.o.provides: CMakeFiles/nn.dir/src/data.c.o.requires
	$(MAKE) -f CMakeFiles/nn.dir/build.make CMakeFiles/nn.dir/src/data.c.o.provides.build
.PHONY : CMakeFiles/nn.dir/src/data.c.o.provides

CMakeFiles/nn.dir/src/data.c.o.provides.build: CMakeFiles/nn.dir/src/data.c.o


CMakeFiles/nn.dir/src/io.c.o: CMakeFiles/nn.dir/flags.make
CMakeFiles/nn.dir/src/io.c.o: ../src/io.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/nn.dir/src/io.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nn.dir/src/io.c.o   -c /home/rafael/Documents/workspace/neural_net/src/io.c

CMakeFiles/nn.dir/src/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nn.dir/src/io.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rafael/Documents/workspace/neural_net/src/io.c > CMakeFiles/nn.dir/src/io.c.i

CMakeFiles/nn.dir/src/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nn.dir/src/io.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rafael/Documents/workspace/neural_net/src/io.c -o CMakeFiles/nn.dir/src/io.c.s

CMakeFiles/nn.dir/src/io.c.o.requires:

.PHONY : CMakeFiles/nn.dir/src/io.c.o.requires

CMakeFiles/nn.dir/src/io.c.o.provides: CMakeFiles/nn.dir/src/io.c.o.requires
	$(MAKE) -f CMakeFiles/nn.dir/build.make CMakeFiles/nn.dir/src/io.c.o.provides.build
.PHONY : CMakeFiles/nn.dir/src/io.c.o.provides

CMakeFiles/nn.dir/src/io.c.o.provides.build: CMakeFiles/nn.dir/src/io.c.o


CMakeFiles/nn.dir/src/text.c.o: CMakeFiles/nn.dir/flags.make
CMakeFiles/nn.dir/src/text.c.o: ../src/text.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/nn.dir/src/text.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nn.dir/src/text.c.o   -c /home/rafael/Documents/workspace/neural_net/src/text.c

CMakeFiles/nn.dir/src/text.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nn.dir/src/text.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rafael/Documents/workspace/neural_net/src/text.c > CMakeFiles/nn.dir/src/text.c.i

CMakeFiles/nn.dir/src/text.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nn.dir/src/text.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rafael/Documents/workspace/neural_net/src/text.c -o CMakeFiles/nn.dir/src/text.c.s

CMakeFiles/nn.dir/src/text.c.o.requires:

.PHONY : CMakeFiles/nn.dir/src/text.c.o.requires

CMakeFiles/nn.dir/src/text.c.o.provides: CMakeFiles/nn.dir/src/text.c.o.requires
	$(MAKE) -f CMakeFiles/nn.dir/build.make CMakeFiles/nn.dir/src/text.c.o.provides.build
.PHONY : CMakeFiles/nn.dir/src/text.c.o.provides

CMakeFiles/nn.dir/src/text.c.o.provides.build: CMakeFiles/nn.dir/src/text.c.o


CMakeFiles/nn.dir/src/linalg.c.o: CMakeFiles/nn.dir/flags.make
CMakeFiles/nn.dir/src/linalg.c.o: ../src/linalg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/nn.dir/src/linalg.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nn.dir/src/linalg.c.o   -c /home/rafael/Documents/workspace/neural_net/src/linalg.c

CMakeFiles/nn.dir/src/linalg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nn.dir/src/linalg.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rafael/Documents/workspace/neural_net/src/linalg.c > CMakeFiles/nn.dir/src/linalg.c.i

CMakeFiles/nn.dir/src/linalg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nn.dir/src/linalg.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rafael/Documents/workspace/neural_net/src/linalg.c -o CMakeFiles/nn.dir/src/linalg.c.s

CMakeFiles/nn.dir/src/linalg.c.o.requires:

.PHONY : CMakeFiles/nn.dir/src/linalg.c.o.requires

CMakeFiles/nn.dir/src/linalg.c.o.provides: CMakeFiles/nn.dir/src/linalg.c.o.requires
	$(MAKE) -f CMakeFiles/nn.dir/build.make CMakeFiles/nn.dir/src/linalg.c.o.provides.build
.PHONY : CMakeFiles/nn.dir/src/linalg.c.o.provides

CMakeFiles/nn.dir/src/linalg.c.o.provides.build: CMakeFiles/nn.dir/src/linalg.c.o


CMakeFiles/nn.dir/src/util.c.o: CMakeFiles/nn.dir/flags.make
CMakeFiles/nn.dir/src/util.c.o: ../src/util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/nn.dir/src/util.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nn.dir/src/util.c.o   -c /home/rafael/Documents/workspace/neural_net/src/util.c

CMakeFiles/nn.dir/src/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nn.dir/src/util.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rafael/Documents/workspace/neural_net/src/util.c > CMakeFiles/nn.dir/src/util.c.i

CMakeFiles/nn.dir/src/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nn.dir/src/util.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rafael/Documents/workspace/neural_net/src/util.c -o CMakeFiles/nn.dir/src/util.c.s

CMakeFiles/nn.dir/src/util.c.o.requires:

.PHONY : CMakeFiles/nn.dir/src/util.c.o.requires

CMakeFiles/nn.dir/src/util.c.o.provides: CMakeFiles/nn.dir/src/util.c.o.requires
	$(MAKE) -f CMakeFiles/nn.dir/build.make CMakeFiles/nn.dir/src/util.c.o.provides.build
.PHONY : CMakeFiles/nn.dir/src/util.c.o.provides

CMakeFiles/nn.dir/src/util.c.o.provides.build: CMakeFiles/nn.dir/src/util.c.o


CMakeFiles/nn.dir/src/main.c.o: CMakeFiles/nn.dir/flags.make
CMakeFiles/nn.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/nn.dir/src/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nn.dir/src/main.c.o   -c /home/rafael/Documents/workspace/neural_net/src/main.c

CMakeFiles/nn.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nn.dir/src/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rafael/Documents/workspace/neural_net/src/main.c > CMakeFiles/nn.dir/src/main.c.i

CMakeFiles/nn.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nn.dir/src/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rafael/Documents/workspace/neural_net/src/main.c -o CMakeFiles/nn.dir/src/main.c.s

CMakeFiles/nn.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/nn.dir/src/main.c.o.requires

CMakeFiles/nn.dir/src/main.c.o.provides: CMakeFiles/nn.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/nn.dir/build.make CMakeFiles/nn.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/nn.dir/src/main.c.o.provides

CMakeFiles/nn.dir/src/main.c.o.provides.build: CMakeFiles/nn.dir/src/main.c.o


# Object files for target nn
nn_OBJECTS = \
"CMakeFiles/nn.dir/src/data.c.o" \
"CMakeFiles/nn.dir/src/io.c.o" \
"CMakeFiles/nn.dir/src/text.c.o" \
"CMakeFiles/nn.dir/src/linalg.c.o" \
"CMakeFiles/nn.dir/src/util.c.o" \
"CMakeFiles/nn.dir/src/main.c.o"

# External object files for target nn
nn_EXTERNAL_OBJECTS =

nn: CMakeFiles/nn.dir/src/data.c.o
nn: CMakeFiles/nn.dir/src/io.c.o
nn: CMakeFiles/nn.dir/src/text.c.o
nn: CMakeFiles/nn.dir/src/linalg.c.o
nn: CMakeFiles/nn.dir/src/util.c.o
nn: CMakeFiles/nn.dir/src/main.c.o
nn: CMakeFiles/nn.dir/build.make
nn: CMakeFiles/nn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rafael/Documents/workspace/neural_net/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable nn"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nn.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nn.dir/build: nn

.PHONY : CMakeFiles/nn.dir/build

CMakeFiles/nn.dir/requires: CMakeFiles/nn.dir/src/data.c.o.requires
CMakeFiles/nn.dir/requires: CMakeFiles/nn.dir/src/io.c.o.requires
CMakeFiles/nn.dir/requires: CMakeFiles/nn.dir/src/text.c.o.requires
CMakeFiles/nn.dir/requires: CMakeFiles/nn.dir/src/linalg.c.o.requires
CMakeFiles/nn.dir/requires: CMakeFiles/nn.dir/src/util.c.o.requires
CMakeFiles/nn.dir/requires: CMakeFiles/nn.dir/src/main.c.o.requires

.PHONY : CMakeFiles/nn.dir/requires

CMakeFiles/nn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/nn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/nn.dir/clean

CMakeFiles/nn.dir/depend:
	cd /home/rafael/Documents/workspace/neural_net/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rafael/Documents/workspace/neural_net /home/rafael/Documents/workspace/neural_net /home/rafael/Documents/workspace/neural_net/build /home/rafael/Documents/workspace/neural_net/build /home/rafael/Documents/workspace/neural_net/build/CMakeFiles/nn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/nn.dir/depend

