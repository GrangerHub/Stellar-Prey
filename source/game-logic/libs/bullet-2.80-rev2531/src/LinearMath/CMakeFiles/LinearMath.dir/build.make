# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_SOURCE_DIR = /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531

# Include any dependencies generated for this target.
include src/LinearMath/CMakeFiles/LinearMath.dir/depend.make

# Include the progress variables for this target.
include src/LinearMath/CMakeFiles/LinearMath.dir/progress.make

# Include the compile flags for this target's objects.
include src/LinearMath/CMakeFiles/LinearMath.dir/flags.make

src/LinearMath/CMakeFiles/LinearMath.dir/btAlignedAllocator.o: src/LinearMath/CMakeFiles/LinearMath.dir/flags.make
src/LinearMath/CMakeFiles/LinearMath.dir/btAlignedAllocator.o: src/LinearMath/btAlignedAllocator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/LinearMath/CMakeFiles/LinearMath.dir/btAlignedAllocator.o"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearMath.dir/btAlignedAllocator.o -c /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btAlignedAllocator.cpp

src/LinearMath/CMakeFiles/LinearMath.dir/btAlignedAllocator.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearMath.dir/btAlignedAllocator.i"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btAlignedAllocator.cpp > CMakeFiles/LinearMath.dir/btAlignedAllocator.i

src/LinearMath/CMakeFiles/LinearMath.dir/btAlignedAllocator.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearMath.dir/btAlignedAllocator.s"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btAlignedAllocator.cpp -o CMakeFiles/LinearMath.dir/btAlignedAllocator.s

src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHull.o: src/LinearMath/CMakeFiles/LinearMath.dir/flags.make
src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHull.o: src/LinearMath/btConvexHull.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHull.o"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearMath.dir/btConvexHull.o -c /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btConvexHull.cpp

src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHull.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearMath.dir/btConvexHull.i"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btConvexHull.cpp > CMakeFiles/LinearMath.dir/btConvexHull.i

src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHull.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearMath.dir/btConvexHull.s"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btConvexHull.cpp -o CMakeFiles/LinearMath.dir/btConvexHull.s

src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHullComputer.o: src/LinearMath/CMakeFiles/LinearMath.dir/flags.make
src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHullComputer.o: src/LinearMath/btConvexHullComputer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHullComputer.o"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearMath.dir/btConvexHullComputer.o -c /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btConvexHullComputer.cpp

src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHullComputer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearMath.dir/btConvexHullComputer.i"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btConvexHullComputer.cpp > CMakeFiles/LinearMath.dir/btConvexHullComputer.i

src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHullComputer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearMath.dir/btConvexHullComputer.s"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btConvexHullComputer.cpp -o CMakeFiles/LinearMath.dir/btConvexHullComputer.s

src/LinearMath/CMakeFiles/LinearMath.dir/btGeometryUtil.o: src/LinearMath/CMakeFiles/LinearMath.dir/flags.make
src/LinearMath/CMakeFiles/LinearMath.dir/btGeometryUtil.o: src/LinearMath/btGeometryUtil.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/LinearMath/CMakeFiles/LinearMath.dir/btGeometryUtil.o"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearMath.dir/btGeometryUtil.o -c /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btGeometryUtil.cpp

src/LinearMath/CMakeFiles/LinearMath.dir/btGeometryUtil.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearMath.dir/btGeometryUtil.i"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btGeometryUtil.cpp > CMakeFiles/LinearMath.dir/btGeometryUtil.i

src/LinearMath/CMakeFiles/LinearMath.dir/btGeometryUtil.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearMath.dir/btGeometryUtil.s"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btGeometryUtil.cpp -o CMakeFiles/LinearMath.dir/btGeometryUtil.s

src/LinearMath/CMakeFiles/LinearMath.dir/btQuickprof.o: src/LinearMath/CMakeFiles/LinearMath.dir/flags.make
src/LinearMath/CMakeFiles/LinearMath.dir/btQuickprof.o: src/LinearMath/btQuickprof.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/LinearMath/CMakeFiles/LinearMath.dir/btQuickprof.o"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearMath.dir/btQuickprof.o -c /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btQuickprof.cpp

src/LinearMath/CMakeFiles/LinearMath.dir/btQuickprof.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearMath.dir/btQuickprof.i"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btQuickprof.cpp > CMakeFiles/LinearMath.dir/btQuickprof.i

src/LinearMath/CMakeFiles/LinearMath.dir/btQuickprof.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearMath.dir/btQuickprof.s"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btQuickprof.cpp -o CMakeFiles/LinearMath.dir/btQuickprof.s

src/LinearMath/CMakeFiles/LinearMath.dir/btSerializer.o: src/LinearMath/CMakeFiles/LinearMath.dir/flags.make
src/LinearMath/CMakeFiles/LinearMath.dir/btSerializer.o: src/LinearMath/btSerializer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/LinearMath/CMakeFiles/LinearMath.dir/btSerializer.o"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearMath.dir/btSerializer.o -c /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btSerializer.cpp

src/LinearMath/CMakeFiles/LinearMath.dir/btSerializer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearMath.dir/btSerializer.i"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btSerializer.cpp > CMakeFiles/LinearMath.dir/btSerializer.i

src/LinearMath/CMakeFiles/LinearMath.dir/btSerializer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearMath.dir/btSerializer.s"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/btSerializer.cpp -o CMakeFiles/LinearMath.dir/btSerializer.s

# Object files for target LinearMath
LinearMath_OBJECTS = \
"CMakeFiles/LinearMath.dir/btAlignedAllocator.o" \
"CMakeFiles/LinearMath.dir/btConvexHull.o" \
"CMakeFiles/LinearMath.dir/btConvexHullComputer.o" \
"CMakeFiles/LinearMath.dir/btGeometryUtil.o" \
"CMakeFiles/LinearMath.dir/btQuickprof.o" \
"CMakeFiles/LinearMath.dir/btSerializer.o"

# External object files for target LinearMath
LinearMath_EXTERNAL_OBJECTS =

src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/btAlignedAllocator.o
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHull.o
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/btConvexHullComputer.o
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/btGeometryUtil.o
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/btQuickprof.o
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/btSerializer.o
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/build.make
src/LinearMath/libLinearMath.a: src/LinearMath/CMakeFiles/LinearMath.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libLinearMath.a"
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && $(CMAKE_COMMAND) -P CMakeFiles/LinearMath.dir/cmake_clean_target.cmake
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LinearMath.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/LinearMath/CMakeFiles/LinearMath.dir/build: src/LinearMath/libLinearMath.a

.PHONY : src/LinearMath/CMakeFiles/LinearMath.dir/build

src/LinearMath/CMakeFiles/LinearMath.dir/clean:
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath && $(CMAKE_COMMAND) -P CMakeFiles/LinearMath.dir/cmake_clean.cmake
.PHONY : src/LinearMath/CMakeFiles/LinearMath.dir/clean

src/LinearMath/CMakeFiles/LinearMath.dir/depend:
	cd /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531 /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531 /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath /media/thedushan/A0A2E82DA2E80A1C/openwolf/src/libs/bullet-2.80-rev2531/src/LinearMath/CMakeFiles/LinearMath.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/LinearMath/CMakeFiles/LinearMath.dir/depend

