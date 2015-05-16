#include "TimeFormatException.hpp"

using namespace tin::agent::exceptions;

const char* TimeFormatException::what() const noexcept
{
	return this->info.c_str();
}

TimeFormatException::TimeFormatException(const std::string& info):
info(info)
{}
