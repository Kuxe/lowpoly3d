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

# Utility rule file for subber_project.

# Include the progress variables for this target.
include CMakeFiles/subber_project.dir/progress.make

CMakeFiles/subber_project: CMakeFiles/subber_project-complete


CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-install
CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-mkdir
CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-download
CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-patch
CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-configure
CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-build
CMakeFiles/subber_project-complete: ../contrib/Stamp/subber_project/subber_project-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'subber_project'"
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/example/CMakeFiles
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/example/CMakeFiles/subber_project-complete
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-done

../contrib/Stamp/subber_project/subber_project-install: ../contrib/Stamp/subber_project/subber_project-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'subber_project'"
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-install

../contrib/Stamp/subber_project/subber_project-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'subber_project'"
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Source/subber_project
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Source/subber_project
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Install/subber_project
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/tmp/subber_project
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E make_directory C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Download/subber_project
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-mkdir

../contrib/Stamp/subber_project/subber_project-download: ../contrib/Stamp/subber_project/subber_project-gitinfo.txt
../contrib/Stamp/subber_project/subber_project-download: ../contrib/Stamp/subber_project/subber_project-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'subber_project'"
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source && "C:\Program Files (x86)\CMake\bin\cmake.exe" -P C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/tmp/subber_project/subber_project-gitclone.cmake
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-download

../contrib/Stamp/subber_project/subber_project-patch: ../contrib/Stamp/subber_project/subber_project-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No patch step for 'subber_project'"
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E echo_append
	"C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-patch

../contrib/Stamp/subber_project/subber_project-configure: ../contrib/tmp/subber_project/subber_project-cfgcmd.txt
../contrib/Stamp/subber_project/subber_project-configure: ../contrib/Stamp/subber_project/subber_project-skip-update
../contrib/Stamp/subber_project/subber_project-configure: ../contrib/Stamp/subber_project/subber_project-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No configure step for 'subber_project'"
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-configure

../contrib/Stamp/subber_project/subber_project-build: ../contrib/Stamp/subber_project/subber_project-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No build step for 'subber_project'"
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E touch C:/Users/Kuxe/programming/gitrepos/lowpoly3d/contrib/Stamp/subber_project/subber_project-build

../contrib/Stamp/subber_project/subber_project-skip-update: ../contrib/Stamp/subber_project/subber_project-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Skipping update step for 'subber_project'"
	cd /d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\contrib\Source\subber_project && "C:\Program Files (x86)\CMake\bin\cmake.exe" -E echo_append

subber_project: CMakeFiles/subber_project
subber_project: CMakeFiles/subber_project-complete
subber_project: ../contrib/Stamp/subber_project/subber_project-install
subber_project: ../contrib/Stamp/subber_project/subber_project-mkdir
subber_project: ../contrib/Stamp/subber_project/subber_project-download
subber_project: ../contrib/Stamp/subber_project/subber_project-patch
subber_project: ../contrib/Stamp/subber_project/subber_project-configure
subber_project: ../contrib/Stamp/subber_project/subber_project-build
subber_project: ../contrib/Stamp/subber_project/subber_project-skip-update
subber_project: CMakeFiles/subber_project.dir/build.make

.PHONY : subber_project

# Rule to build all files generated by this target.
CMakeFiles/subber_project.dir/build: subber_project

.PHONY : CMakeFiles/subber_project.dir/build

CMakeFiles/subber_project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\subber_project.dir\cmake_clean.cmake
.PHONY : CMakeFiles/subber_project.dir/clean

CMakeFiles/subber_project.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Kuxe\programming\gitrepos\lowpoly3d C:\Users\Kuxe\programming\gitrepos\lowpoly3d C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example C:\Users\Kuxe\programming\gitrepos\lowpoly3d\example\CMakeFiles\subber_project.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/subber_project.dir/depend

