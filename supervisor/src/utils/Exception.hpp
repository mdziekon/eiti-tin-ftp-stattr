#ifndef TIN_UTILS_EXCEPTION_HPP
#define	TIN_UTILS_EXCEPTION_HPP

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#include <exception>

namespace tin { namespace utils
{
    class Exception: public std::exception
    {};
}}

#endif	/* TIN_UTILS_EXCEPTION_HPP */

