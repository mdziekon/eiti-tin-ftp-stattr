#include "MalformedJSONException.hpp"

using namespace tin::agent::exceptions;

const char* MalformedJSONException::what() const noexcept
{
    return this->info.c_str();
}

MalformedJSONException::MalformedJSONException(const std::string& info):
info(info)
{}
