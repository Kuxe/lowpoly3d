#include "binary_path.hpp"
#include <cassert>
#include <cstdlib>
#include <filesystem>

#if defined(_WIN64)
#include <libloaderapi.h>
#undef max // culprit: minwindef.h
#include <limits>

namespace {
	std::filesystem::path get_current_binary_absolute_path_platform()
	{
		char path[MAX_PATH];
		GetModuleFileNameA(NULL, path, MAX_PATH);
		return std::filesystem::path(path);
	}
}

#elif defined(unix) || defined(__unix__) || defined(__unix)
#include <libgen.h>
#include <unistd.h> 
#include <linux/limits.h>

namespace {
	std::filesystem::path get_current_binary_absolute_path_platform()
	{
		char path[PATH_MAX];
		readlink("/proc/self/exe", path, PATH_MAX);
		return std::filesystem::path(path);
	}
}

#else
	static_assert(false, "Unknown platform");
#endif

namespace lowpoly3d
{
	// Function is common to all platforms and wraps the platform-specific
	// functions with some sanity checks.
	std::filesystem::path get_current_binary_absolute_path()
	{
		auto const path = get_current_binary_absolute_path_platform();
		assert(std::filesystem::exists(path));
		return path;
	}
}