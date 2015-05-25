#include "SnifferStatus.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::SnifferStatus;

SnifferStatus::SnifferStatus(const bool is):
is(is)
{}

void SnifferStatus::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}
