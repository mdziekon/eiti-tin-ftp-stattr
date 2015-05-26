#ifndef TIN_CONTROLLERS_TERMINAL_EVENTS_SENDMESSAGE_HPP
#define TIN_CONTROLLERS_TERMINAL_EVENTS_SENDMESSAGE_HPP

#include "../Event.hpp"
#include "../../../utils/JSON.hpp"

namespace tin { namespace controllers { namespace terminal { namespace events 
{
	struct SendMessage : public tin::controllers::terminal::Event
	{
		const tin::utils::json::ptr jsonPtr;

		SendMessage(
            const tin::utils::json::ptr& jsonPtr
        );

		virtual void accept(tin::controllers::terminal::TerminalVisitor& visitor);
	};
}}}}

#endif