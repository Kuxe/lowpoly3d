#include "utils/not_implemented_exception.hpp"

NotImplementedException::NotImplementedException()
	: std::logic_error("Function is not implemented") { }