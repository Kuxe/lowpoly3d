
# Modify interface include directories of the given target to be
# 'SYSTEM'-headers. This tells CMake to ignore warnings for the target.
function (target_disable_warnings target)
	get_target_property(interface_include_directories ${target} INTERFACE_INCLUDE_DIRECTORIES)
	target_include_directories(${target} SYSTEM INTERFACE ${interface_include_directories})
endfunction()