#include "JSON.hpp"

tin::utils::json::ptr tin::utils::json::makeSharedInstance()
{
    return tin::utils::json::ptr(new nlohmann::json());
}

tin::utils::json::ptr tin::utils::json::makeSharedInstance(const std::string& str)
{
    return tin::utils::json::ptr(new nlohmann::json(nlohmann::json::parse(str)));
}

tin::utils::json::ptr tin::utils::json::getSharedInstance(const nlohmann::json& json)
{
    return tin::utils::json::ptr(new nlohmann::json(json));
}
