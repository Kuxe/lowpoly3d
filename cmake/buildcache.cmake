
# Download buildcache and use it (Windows only).

# Could probably use this in Linux too but ccache works good there.
# Might be a reason to use only buildcache anyway to keep
# the build-configuration simple

# CMake finds it's compiler in a specific order:
# cc, gcc, cl, bcc, xlc, clang
# See https://cmake.org/pipermail/cmake/2013-March/053819.html.
# This is PROBLEMATIC! buildcache does not understand cc or c++.
# Solution: Set CC and CXX environment-variables to point to gcc and g++.

if(WIN32)
	set(buildcache_archive "${CMAKE_SOURCE_DIR}/download/buildcache-windows.zip")
	set(buildcache_bindir "${CMAKE_SOURCE_DIR}/buildcache/bin")
	set(buildcache_expected_binfile "${CMAKE_SOURCE_DIR}/buildcache/bin/buildcache.exe")
	set(buildcache_download_url "https://gitlab.com/bits-n-bites/buildcache/-/releases/v0.30.2/downloads/buildcache-windows.zip")

	if(NOT EXISTS ${buildcache_expected_binfile})
		message(STATUS "buildcache.cmake: buildcache not found, downloading and extracting...")
		file(DOWNLOAD ${buildcache_download_url} ${buildcache_archive})
		message(STATUS "buildcache.cmake: buildcache downloaded to ${buildcache_archive}")
		file(ARCHIVE_EXTRACT INPUT ${buildcache_archive} DESTINATION ${CMAKE_SOURCE_DIR})
		message(STATUS "buildcache.cmake: buildcache extracted to ${CMAKE_SOURCE_DIR}")
	endif()

	find_program(buildcache buildcache ${buildcache_bindir})
	if(buildcache)
		message(STATUS "Found \"buildcache\" at \"${buildcache}\"")
		set(CMAKE_C_COMPILER_LAUNCHER ${buildcache})
		set(CMAKE_CXX_COMPILER_LAUNCHER ${buildcache})
	endif()
endif()