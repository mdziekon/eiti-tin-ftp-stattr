#include "Client.hpp"

#include <functional>
#include <utility>
#include <exception>
#include <string>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <iostream>

#include "events/IncomingMessage.hpp"
#include "events/OutcomingMessage.hpp"
#include "events/Terminate.hpp"
#include "../../../utils/JSON.hpp"

using tin::network::bsdsocket::wrapper::Client;
using nlohmann::json;
namespace events = tin::network::bsdsocket::wrapper::events;

Client::Client():
receiverHelperThread(*this, this->receiverQueue),
transmitterHelperThread(*this, this->transmitterQueue),
visitor(*this)
{
    this->run();
}

Client::~Client()
{
    this->terminate();
}

void Client::run()
{
    this->receiverThread = this->receiverHelperThread.createThread();
    this->transmitterThread = this->transmitterHelperThread.createThread();

    this->receiverThread.detach();
    this->transmitterThread.detach();
}

void Client::terminate()
{
    this->receiverHelperThread.terminate();
    this->transmitterHelperThread.terminate();
    this->receiverQueue.push(
        EventPtr(new events::Terminate())
    );
    this->transmitterQueue.push(
        EventPtr(new events::Terminate())
    );
}

unsigned int Client::attachResponseReceivedHandler(
    std::function<void(const std::string&, const unsigned int&, const std::string&)>& handler
)
{
    return this->messageReceivedHandlers.insert(handler);
}

unsigned int Client::attachResponseReceivedHandler(
    std::function<void(const std::string&, const unsigned int&, const std::string&)>&& handler
)
{
    return this->messageReceivedHandlers.insert(
        std::forward<std::function<void(const std::string&, const unsigned int&, const std::string&)>>(handler)
    );
}

void Client::sendMessage(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message,
    const bool& waitForResponse
)
{
    this->transmitterQueue.push(
        EventPtr(new events::OutcomingMessage(ip, port, message, waitForResponse))
    );
}


void Client::runMessageReceivedHandlers(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message
)
{
    for (auto iter: this->messageReceivedHandlers)
    {
        iter.second(ip, port, message);
    }
}


void Client::onMessageRequest(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message,
    const bool& waitForResponse
)
{
    std::string temp;
    int readBufSize = 1024;
    char buf[readBufSize + 1];
    int rval;

    struct sockaddr_in server;
    struct hostent *hp;

    // Setup
    server.sin_family = AF_INET;

    // Get IP from name
    hp = gethostbyname(ip.c_str());
    if (hp == (struct hostent *) 0)
    {
        return;
        // Throw unknown host error
    }
    memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);

    // Get port
    server.sin_port = htons(port);

    // Create socket
    this->socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketHandle == -1)
    {

        return;
        // Throw socket open error
    }

    // Connect to server with created socket
    if (connect(this->socketHandle, (struct sockaddr *) &server, sizeof server) == -1)
    {
        auto j = json::parse(message);
        j["error"] = {{"notConnected", true}};
        std::string temp3 = j.dump();
        this->onResponseReceive(ip, port, temp3);
        return;
        // Throw connection error
    }

    if (write(this->socketHandle, message.c_str(), message.length() + 1) == -1)
    {
        return;
        // Throw write error
    }

    if (!waitForResponse)
    {
        close(this->socketHandle);
        return;
    }

    // Receiving a new message, clear temp
    temp.clear();
    while(true)
    {
        // Clear buf
        memset(buf, 0, sizeof buf);

        // FIXME: when message surpasses buflength, we truncate it
        //        try to read in chunks
        if ((rval = read(this->socketHandle, buf, readBufSize)) == -1)
        {
            return;
            // Throw message read error
        }
        if (rval == 0)
        {
            // Reading completed, send message to queue
            this->onResponseReceive(ip, port, temp);

            break;
        }
        else
        {
            // Reading completed, send message to queue
            temp.append(buf);

            if (buf[rval - 1] != 0)
            {
                continue;
            }

            this->onResponseReceive(ip, port, temp);

            break;
        }
    }

    close(this->socketHandle);
    return;
}

void Client::onResponseReceive(
    const std::string& ip,
    const unsigned int& port,
    const std::string& message
)
{
    this->receiverQueue.push(
        EventPtr(new events::IncomingMessage(ip, port, message))
    );
}
