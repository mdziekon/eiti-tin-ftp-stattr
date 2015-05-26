#ifndef TIN_AGENT_SRC_EXCEPTIONS_MALFORMEDJSONEXCEPTION_HPP
#define TIN_AGENT_SRC_EXCEPTIONS_MALFORMEDJSONEXCEPTION_HPP

#include <string>

#include "../utils/Exception.hpp"

namespace tin { namespace agent { namespace exceptions 
{
    class MalformedJSONException : tin::utils::Exception
    {
        std::string info;
    public:
        const char* what() const noexcept;
        
        MalformedJSONException(const std::string& info);
    };
}}}

#endif