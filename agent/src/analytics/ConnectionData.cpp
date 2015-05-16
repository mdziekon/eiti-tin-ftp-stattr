#include "ConnectionData.hpp"

using namespace tin::agent::analytics;

TcpData::TcpData(const u_int32_t& port):
port(port)
{}

const std::pair<int,int> ConnectionData::getDuration() const
{
	if((this->beginningSeconds > this->endingSeconds)
		|| ((this->beginningSeconds == this->endingSeconds) 
			&& (this->beginningMicroseconds > this->endingMicroseconds))) {
		
		throw tin::agent::exceptions::TimeFormatException(
			"Connection start timestamp is greater than end timestamp\n"
		);
	}
	
	u_int32_t durationSeconds = this->endingSeconds - this->beginningSeconds;
	u_int32_t durationMilliseconds;
	
	if(this->beginningSeconds > this->endingMicroseconds) {
		durationMilliseconds = this->beginningMicroseconds - this->endingMicroseconds;
		durationSeconds += 1;
	}
	else {
		durationMilliseconds = this->endingMicroseconds - this->beginningMicroseconds;
	}
	
	return std::make_pair(durationSeconds, durationMilliseconds);
}

const u_int32_t ConnectionData::getAverageSpeed() const
{
	u_int32_t connectionDuration = this->getDuration().first;
	
	if(connectionDuration > 0) {
		return static_cast<u_int32_t>(this->totalLengthBytes / connectionDuration);
	}
	else {
		return 0;
	}
}

ConnectionData::ConnectionData(
	u_int32_t clientAddress, 
	u_int32_t serverAddress,
	u_int32_t beginningSeconds, 
	u_int32_t beginningMicroseconds,
	u_int32_t endingSeconds,
	u_int32_t endingMicroseconds, 
	u_int32_t totalLengthBytes, 
	u_int32_t totalLengthPackets, 
	u_int32_t tcpPort):
clientAddress(clientAddress),
serverAddress(serverAddress),
beginningSeconds(beginningSeconds),
beginningMicroseconds(beginningMicroseconds),
endingSeconds(endingSeconds),
endingMicroseconds(endingMicroseconds),
totalLengthBytes(totalLengthBytes),
totalLengthPackets(totalLengthPackets),
tcpData((tcpPort)? new TcpData(tcpPort): nullptr)
{}
