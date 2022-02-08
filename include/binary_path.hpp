#ifndef BINARY_PATH_HPP
#define BINARY_PATH_HPP

#include <filesystem>

namespace lowpoly3d
{
	// Returns the absolute path of the calling binary
	std::filesystem::path get_current_binary_absolute_path();
}

#endif // BINARY_PATH_HPP