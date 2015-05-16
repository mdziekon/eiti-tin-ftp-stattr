#ifndef TIN_AGENT_ANALYTICS_ANALYTICS_HPP
#define TIN_AGENT_ANALYTICS_ANALYTICS_HPP

#include <unordered_map>
#include <sys/types.h>

namespace tin { namespace agent { namespace analytics 
{
	class Analytics
	{
		std::unordered_map<u_int32_t, ConnectionData> statistics;
	public:
		
		Analytics();
		Analytics(const Analytics& analytics) = delete;
		Analytics(const Analytics&& analytics) = default;
		
		// update state methods
		void 
		
		const std::vector<ConnectionData> getDataPerClient(const u_int32_t& netaddr) const;
		
	};
}}}

#endif /* TIN_AGENT_ANALYTICS_ANALYTICS_HPP */