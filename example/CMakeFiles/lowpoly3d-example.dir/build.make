# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.4

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Kuxe\programming\gitrepos\lowpoly3d

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example

# Include any dependencies generated for this target.
include CMakeFiles/lowpoly3d-example.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lowpoly3d-example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lowpoly3d-example.dir/flags.make

CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj: ../src/renderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\renderer.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\renderer.cpp

CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\renderer.cpp > CMakeFiles\lowpoly3d-example.dir\src\renderer.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\renderer.cpp -o CMakeFiles\lowpoly3d-example.dir\src\renderer.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj: ../src/renderdata.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\renderdata.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\renderdata.cpp

CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\renderdata.cpp > CMakeFiles\lowpoly3d-example.dir\src\renderdata.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\renderdata.cpp -o CMakeFiles\lowpoly3d-example.dir\src\renderdata.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj: ../src/model.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\model.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\model.cpp

CMakeFiles/lowpoly3d-example.dir/src/model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/model.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\model.cpp > CMakeFiles\lowpoly3d-example.dir\src\model.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/model.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\model.cpp -o CMakeFiles\lowpoly3d-example.dir\src\model.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj: ../src/shaderprogram.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\shaderprogram.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\shaderprogram.cpp

CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\shaderprogram.cpp > CMakeFiles\lowpoly3d-example.dir\src\shaderprogram.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\shaderprogram.cpp -o CMakeFiles\lowpoly3d-example.dir\src\shaderprogram.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj: ../src/camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\camera.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\camera.cpp

CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\camera.cpp > CMakeFiles\lowpoly3d-example.dir\src\camera.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\camera.cpp -o CMakeFiles\lowpoly3d-example.dir\src\camera.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj: ../src/deltatime.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\deltatime.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\deltatime.cpp

CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\deltatime.cpp > CMakeFiles\lowpoly3d-example.dir\src\deltatime.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\deltatime.cpp -o CMakeFiles\lowpoly3d-example.dir\src\deltatime.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj: ../src/terraingenerator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\terraingenerator.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\terraingenerator.cpp

CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\terraingenerator.cpp > CMakeFiles\lowpoly3d-example.dir\src\terraingenerator.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\terraingenerator.cpp -o CMakeFiles\lowpoly3d-example.dir\src\terraingenerator.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj: ../src/cubegenerator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\cubegenerator.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\cubegenerator.cpp

CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\cubegenerator.cpp > CMakeFiles\lowpoly3d-example.dir\src\cubegenerator.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\cubegenerator.cpp -o CMakeFiles\lowpoly3d-example.dir\src\cubegenerator.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj: ../src/spheregenerator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\spheregenerator.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\spheregenerator.cpp

CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\spheregenerator.cpp > CMakeFiles\lowpoly3d-example.dir\src\spheregenerator.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\spheregenerator.cpp -o CMakeFiles\lowpoly3d-example.dir\src\spheregenerator.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj: ../src/example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\example.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\example.cpp

CMakeFiles/lowpoly3d-example.dir/src/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/example.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\example.cpp > CMakeFiles\lowpoly3d-example.dir\src\example.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/example.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\example.cpp -o CMakeFiles\lowpoly3d-example.dir\src\example.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj: ../src/framebuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\framebuffer.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\framebuffer.cpp

CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\framebuffer.cpp > CMakeFiles\lowpoly3d-example.dir\src\framebuffer.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\framebuffer.cpp -o CMakeFiles\lowpoly3d-example.dir\src\framebuffer.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj: ../src/depthframebuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\depthframebuffer.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\depthframebuffer.cpp

CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\depthframebuffer.cpp > CMakeFiles\lowpoly3d-example.dir\src\depthframebuffer.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\depthframebuffer.cpp -o CMakeFiles\lowpoly3d-example.dir\src\depthframebuffer.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj


CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/flags.make
CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj: CMakeFiles/lowpoly3d-example.dir/includes_CXX.rsp
CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj: ../src/uniformbuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj"
	C:\tools\mingw64\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\lowpoly3d-example.dir\src\uniformbuffer.cpp.obj -c C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\uniformbuffer.cpp

CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.i"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\uniformbuffer.cpp > CMakeFiles\lowpoly3d-example.dir\src\uniformbuffer.cpp.i

CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.s"
	C:\tools\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kuxe\programming\gitrepos\lowpoly3d\src\uniformbuffer.cpp -o CMakeFiles\lowpoly3d-example.dir\src\uniformbuffer.cpp.s

CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.requires:

.PHONY : CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.requires

CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.provides: CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.requires
	$(MAKE) -f CMakeFiles\lowpoly3d-example.dir\build.make CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.provides.build
.PHONY : CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.provides

CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.provides.build: CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj


# Object files for target lowpoly3d-example
lowpoly3d__example_OBJECTS = \
"CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj" \
"CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj"

# External object files for target lowpoly3d-example
lowpoly3d__example_EXTERNAL_OBJECTS =

lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/build.make
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/linklibs.rsp
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/objects1.rsp
lowpoly3d-example.exe: CMakeFiles/lowpoly3d-example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable lowpoly3d-example.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lowpoly3d-example.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lowpoly3d-example.dir/build: lowpoly3d-example.exe

.PHONY : CMakeFiles/lowpoly3d-example.dir/build

CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/renderer.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/renderdata.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/model.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/shaderprogram.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/camera.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/deltatime.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/terraingenerator.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/cubegenerator.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/spheregenerator.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/example.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/framebuffer.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/depthframebuffer.cpp.obj.requires
CMakeFiles/lowpoly3d-example.dir/requires: CMakeFiles/lowpoly3d-example.dir/src/uniformbuffer.cpp.obj.requires

.PHONY : CMakeFiles/lowpoly3d-example.dir/requires

CMakeFiles/lowpoly3d-example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lowpoly3d-example.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lowpoly3d-example.dir/clean

CMakeFiles/lowpoly3d-example.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Kuxe\programming\gitrepos\lowpoly3d C:\Users\Kuxe\programming\gitrepos\lowpoly3d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles\lowpoly3d-example.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lowpoly3d-example.dir/depend

