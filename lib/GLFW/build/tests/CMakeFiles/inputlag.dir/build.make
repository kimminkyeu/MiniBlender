# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Users/minkyeki/.brew/Cellar/cmake/3.26.4/bin/cmake

# The command to remove a file.
RM = /Users/minkyeki/.brew/Cellar/cmake/3.26.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /goinfre/minkyeki/scoop/GLFW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /goinfre/minkyeki/scoop/lib/GLFW/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/inputlag.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/inputlag.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/inputlag.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/inputlag.dir/flags.make

tests/CMakeFiles/inputlag.dir/inputlag.c.o: tests/CMakeFiles/inputlag.dir/flags.make
tests/CMakeFiles/inputlag.dir/inputlag.c.o: /goinfre/minkyeki/scoop/GLFW/tests/inputlag.c
tests/CMakeFiles/inputlag.dir/inputlag.c.o: tests/CMakeFiles/inputlag.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/goinfre/minkyeki/scoop/lib/GLFW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/inputlag.dir/inputlag.c.o"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/inputlag.dir/inputlag.c.o -MF CMakeFiles/inputlag.dir/inputlag.c.o.d -o CMakeFiles/inputlag.dir/inputlag.c.o -c /goinfre/minkyeki/scoop/GLFW/tests/inputlag.c

tests/CMakeFiles/inputlag.dir/inputlag.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/inputlag.dir/inputlag.c.i"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /goinfre/minkyeki/scoop/GLFW/tests/inputlag.c > CMakeFiles/inputlag.dir/inputlag.c.i

tests/CMakeFiles/inputlag.dir/inputlag.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/inputlag.dir/inputlag.c.s"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /goinfre/minkyeki/scoop/GLFW/tests/inputlag.c -o CMakeFiles/inputlag.dir/inputlag.c.s

tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.o: tests/CMakeFiles/inputlag.dir/flags.make
tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.o: /goinfre/minkyeki/scoop/GLFW/deps/getopt.c
tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.o: tests/CMakeFiles/inputlag.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/goinfre/minkyeki/scoop/lib/GLFW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.o"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.o -MF CMakeFiles/inputlag.dir/__/deps/getopt.c.o.d -o CMakeFiles/inputlag.dir/__/deps/getopt.c.o -c /goinfre/minkyeki/scoop/GLFW/deps/getopt.c

tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/inputlag.dir/__/deps/getopt.c.i"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /goinfre/minkyeki/scoop/GLFW/deps/getopt.c > CMakeFiles/inputlag.dir/__/deps/getopt.c.i

tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/inputlag.dir/__/deps/getopt.c.s"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /goinfre/minkyeki/scoop/GLFW/deps/getopt.c -o CMakeFiles/inputlag.dir/__/deps/getopt.c.s

tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o: tests/CMakeFiles/inputlag.dir/flags.make
tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o: /goinfre/minkyeki/scoop/GLFW/deps/glad_gl.c
tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o: tests/CMakeFiles/inputlag.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/goinfre/minkyeki/scoop/lib/GLFW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o -MF CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o.d -o CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o -c /goinfre/minkyeki/scoop/GLFW/deps/glad_gl.c

tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/inputlag.dir/__/deps/glad_gl.c.i"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /goinfre/minkyeki/scoop/GLFW/deps/glad_gl.c > CMakeFiles/inputlag.dir/__/deps/glad_gl.c.i

tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/inputlag.dir/__/deps/glad_gl.c.s"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /goinfre/minkyeki/scoop/GLFW/deps/glad_gl.c -o CMakeFiles/inputlag.dir/__/deps/glad_gl.c.s

# Object files for target inputlag
inputlag_OBJECTS = \
"CMakeFiles/inputlag.dir/inputlag.c.o" \
"CMakeFiles/inputlag.dir/__/deps/getopt.c.o" \
"CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o"

# External object files for target inputlag
inputlag_EXTERNAL_OBJECTS =

tests/inputlag.app/Contents/MacOS/inputlag: tests/CMakeFiles/inputlag.dir/inputlag.c.o
tests/inputlag.app/Contents/MacOS/inputlag: tests/CMakeFiles/inputlag.dir/__/deps/getopt.c.o
tests/inputlag.app/Contents/MacOS/inputlag: tests/CMakeFiles/inputlag.dir/__/deps/glad_gl.c.o
tests/inputlag.app/Contents/MacOS/inputlag: tests/CMakeFiles/inputlag.dir/build.make
tests/inputlag.app/Contents/MacOS/inputlag: src/libglfw3.a
tests/inputlag.app/Contents/MacOS/inputlag: tests/CMakeFiles/inputlag.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/goinfre/minkyeki/scoop/lib/GLFW/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable inputlag.app/Contents/MacOS/inputlag"
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/inputlag.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/inputlag.dir/build: tests/inputlag.app/Contents/MacOS/inputlag
.PHONY : tests/CMakeFiles/inputlag.dir/build

tests/CMakeFiles/inputlag.dir/clean:
	cd /goinfre/minkyeki/scoop/lib/GLFW/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/inputlag.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/inputlag.dir/clean

tests/CMakeFiles/inputlag.dir/depend:
	cd /goinfre/minkyeki/scoop/lib/GLFW/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /goinfre/minkyeki/scoop/GLFW /goinfre/minkyeki/scoop/GLFW/tests /goinfre/minkyeki/scoop/lib/GLFW/build /goinfre/minkyeki/scoop/lib/GLFW/build/tests /goinfre/minkyeki/scoop/lib/GLFW/build/tests/CMakeFiles/inputlag.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/inputlag.dir/depend

