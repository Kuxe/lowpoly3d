#ifndef NOT_IMPLEMENTED_EXCEPTION_HPP
#define NOT_IMPLEMENTED_EXCEPTION_HPP

#include <stdexcept> // std::logic_error

struct NotImplementedException : public std::logic_error {
	NotImplementedException();
};

#endif // NOT_IMPLEMENTED_EXCEPTION_HPP