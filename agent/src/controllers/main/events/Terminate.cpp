#include "Terminate.hpp"

#include "../MainVisitor.hpp"

using tin::controllers::main::events::Terminate;

void Terminate::accept(tin::controllers::main::MainVisitor& visitor)
{
    visitor.visit(*this);
}
