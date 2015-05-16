#ifndef TIN_AGENT_ANALYTICS_CONNECTIONDATA_HPP
#define TIN_AGENT_ANALYTICS_CONNECTIONDATA_HPP

#include <sys/types.h>
#include <memory>

#include "../exceptions/TimeFormatException.hpp"

namespace tin { namespace agent { namespace analytics 
{
	struct TcpData
	{
		const u_int32_t port;
		
		TcpData(const u_int32_t& port);
	};
	
	struct ConnectionData
	{
		const u_int32_t clientAddress;
		const u_int32_t serverAddress;
		
		const u_int32_t beginningSeconds;
		const u_int32_t beginningMicroseconds;
		const u_int32_t endingSeconds;
		const u_int32_t endingMicroseconds;
		
		const u_int32_t totalLengthBytes;
		const u_int32_t totalLengthPackets;
		
		const std::unique_ptr<TcpData> tcpData;
		
		const std::pair<int, int> getDuration() const;
		const u_int32_t getAverageSpeed() const;

		ConnectionData(
			u_int32_t clientAddress,
			u_int32_t serverAddress,
			u_int32_t beginningSeconds,
			u_int32_t beginningMicroseconds,
			u_int32_t endingSeconds,
			u_int32_t endingMicroseconds,
			u_int32_t totalLengthBytes,
			u_int32_t totalLengthPackets,
			u_int32_t tcpPort = 0
		);
	};
}}}

#endif /* TIN_AGENT_ANALYTICS_CONNECTIONDATA_HPP */