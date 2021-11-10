# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.18

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

# Suppress display of executed commands.
$$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/C/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "/C/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator

# Include any dependencies generated for this target.
include demo/CMakeFiles/DungeonGeneratorDemo.dir/depend.make

# Include the progress variables for this target.
include demo/CMakeFiles/DungeonGeneratorDemo.dir/progress.make

# Include the compile flags for this target's objects.
include demo/CMakeFiles/DungeonGeneratorDemo.dir/flags.make

demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj: demo/CMakeFiles/DungeonGeneratorDemo.dir/flags.make
demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj: demo/CMakeFiles/DungeonGeneratorDemo.dir/includes_CXX.rsp
demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj: demo/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj"
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && /C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj -c /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo/main.cpp

demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.i"
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && /C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo/main.cpp > CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.i

demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.s"
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && /C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo/main.cpp -o CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.s

# Object files for target DungeonGeneratorDemo
DungeonGeneratorDemo_OBJECTS = \
"CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj"

# External object files for target DungeonGeneratorDemo
DungeonGeneratorDemo_EXTERNAL_OBJECTS =

demo/DungeonGeneratorDemo.exe: demo/CMakeFiles/DungeonGeneratorDemo.dir/main.cpp.obj
demo/DungeonGeneratorDemo.exe: demo/CMakeFiles/DungeonGeneratorDemo.dir/build.make
demo/DungeonGeneratorDemo.exe: lib/libMapGenerator.a
demo/DungeonGeneratorDemo.exe: lib/libGrid.a
demo/DungeonGeneratorDemo.exe: demo/CMakeFiles/DungeonGeneratorDemo.dir/linklibs.rsp
demo/DungeonGeneratorDemo.exe: demo/CMakeFiles/DungeonGeneratorDemo.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DungeonGeneratorDemo.exe"
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && "/C/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/DungeonGeneratorDemo.dir/objects.a
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && /C/msys64/mingw64/bin/ar.exe cr CMakeFiles/DungeonGeneratorDemo.dir/objects.a @CMakeFiles/DungeonGeneratorDemo.dir/objects1.rsp
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && /C/msys64/mingw64/bin/g++.exe -Wl,--whole-archive CMakeFiles/DungeonGeneratorDemo.dir/objects.a -Wl,--no-whole-archive -o DungeonGeneratorDemo.exe -Wl,--out-implib,../lib/libDungeonGeneratorDemo.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/DungeonGeneratorDemo.dir/linklibs.rsp

# Rule to build all files generated by this target.
demo/CMakeFiles/DungeonGeneratorDemo.dir/build: demo/DungeonGeneratorDemo.exe

.PHONY : demo/CMakeFiles/DungeonGeneratorDemo.dir/build

demo/CMakeFiles/DungeonGeneratorDemo.dir/clean:
	cd /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo && $(CMAKE_COMMAND) -P CMakeFiles/DungeonGeneratorDemo.dir/cmake_clean.cmake
.PHONY : demo/CMakeFiles/DungeonGeneratorDemo.dir/clean

demo/CMakeFiles/DungeonGeneratorDemo.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo /C/Users/ferrohound/Documents/gamedev/generators/DungeonGenerator/demo/CMakeFiles/DungeonGeneratorDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : demo/CMakeFiles/DungeonGeneratorDemo.dir/depend

