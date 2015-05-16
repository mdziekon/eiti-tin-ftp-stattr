#ifndef TIN_AGENT_EXCEPTIONS_TIMEFORMATEXCEPTION_HPP
#define TIN_AGENT_EXCEPTIONS_TIMEFORMATEXCEPTION_HPP

#include <exception>
#include <string>

namespace tin { namespace agent { namespace exceptions 
{
	class TimeFormatException : std::exception
	{
	public:
		const std::string info;
		
		virtual const char* what() const noexcept;
		
		TimeFormatException(const std::string& info);
	};
}}}

#endif