#ifndef TIN_UTILS_JSON_HPP
#define TIN_UTILS_JSON_HPP

#include <memory>
#include <json/src/json.hpp>

namespace tin { namespace utils { namespace json
{
    typedef std::shared_ptr<nlohmann::json> ptr;

    ptr makeSharedInstance();
    ptr makeSharedInstance(const std::string& str);
    ptr getSharedInstance(const nlohmann::json& json);
}}}

#endif /* TIN_UTILS_JSON_HPP */
