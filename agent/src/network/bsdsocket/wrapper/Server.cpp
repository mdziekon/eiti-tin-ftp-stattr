#include "Server.hpp"

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

#include <iostream>

#include "events/IncomingMessage.hpp"
#include "events/OutcomingMessage.hpp"
#include "events/Terminate.hpp"

using tin::network::bsdsocket::wrapper::Server;
namespace events = tin::network::bsdsocket::wrapper::events;

Server::Server(const unsigned int& portNo):
portNo(portNo),
connectionLock(this->connectionMtx),
receiverHelperThread(*this, this->receiverQueue),
transmitterHelperThread(*this, this->transmitterQueue),
visitor(*this)
{
    // Create socket
    this->socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketHandle == -1)
    {
        // Throw socket open error
    }

    //Binding Socket
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = ntohs(this->portNo);
    if (bind(this->socketHandle, (struct sockaddr *) &server, sizeof server) == -1)
    {
        // Throw socket bind error
    }

}

Server::~Server()
{
    this->terminate();
}

void Server::run()
{
    this->listenerThread = std::thread(
        &Server::listenerLoop,
        std::ref(*this)
    );
    this->receiverThread = this->receiverHelperThread.createThread();
    this->transmitterThread = this->transmitterHelperThread.createThread();

    this->listenerThread.detach();
    this->receiverThread.detach();
    this->transmitterThread.detach();
}

void Server::terminate()
{
    this->terminateConnection();

    this->receiverHelperThread.terminate();
    this->transmitterHelperThread.terminate();
    this->receiverQueue.push(
        EventPtr(new events::Terminate())
    );
    this->transmitterQueue.push(
        EventPtr(new events::Terminate())
    );
}

unsigned int Server::attachMessageReceivedHandler(
    std::function<void(const std::string&)>& handler
)
{
    return this->messageReceivedHandlers.insert(handler);
}

unsigned int Server::attachMessageReceivedHandler(
    std::function<void(const std::string&)>&& handler
)
{
    return this->messageReceivedHandlers.insert(
        std::forward<std::function<void(const std::string&)>>(handler)
    );
}

void Server::sendResponse(
    const std::string& message
)
{
    this->transmitterQueue.push(
        EventPtr(new events::OutcomingMessage(message))
    );
}

void Server::terminateConnection()
{
    this->unlockConnection();
}


void Server::runMessageReceivedHandlers(
    const std::string& message
)
{
    for (auto iter: this->messageReceivedHandlers)
    {
        iter.second(message);
    }
}


void Server::listenerLoop()
{
    listen(this->socketHandle, 5);

    int readBufSize = 1024;
    int rval;
    char buf[readBufSize];
    std::string temp;

    // Receive Connections - max 5 in queue
    while(true)
    {
        //Accept Connection till termination
        this->connectionHandle = accept(this->socketHandle, (struct sockaddr *) 0, (socklen_t *) 0);
        if (this->connectionHandle == -1)
        {
            // Throw message accept error
        }
        else
        {
            // Receiving a new message, clear temp
            temp.clear();
            while(true)
            {
                // Clear buf
                memset(buf, 0, sizeof buf);

                // FIXME: when message surpasses buflength, we truncate it
                //        try to read in chunks
                if ((rval = read(this->connectionHandle, buf, readBufSize)) == -1)
                {
                    // Throw message read error
                }
                if (rval == 0)
                {
                    // Reading completed, send message to queue
                    // do not end the connection, we might want to send a reply
                    // Lock itself instead, keeping connection for later
                    this->onMessageReceive(temp);

                    this->connectionActionAwaiting = true;
                    while(this->connectionActionAwaiting)
                    {
                        this->connectionLockCnd.wait(this->connectionLock);
                    }

                    break;
                }
                else
                {
                    // Reading completed, send message to queue
                    // do not end the connection, we might want to send a reply
                    // Lock itself instead, keeping connection for later

                    temp.append(buf);

                    this->onMessageReceive(temp);

                    this->connectionActionAwaiting = true;
                    while(this->connectionActionAwaiting)
                    {
                        this->connectionLockCnd.wait(this->connectionLock);
                    }

                    break;
                }
            }
        }
        close(this->connectionHandle);
        this->connectionHandle = 0;
    }
}

void Server::onMessageReceive(const std::string& message)
{
    this->receiverQueue.push(
        EventPtr(new events::IncomingMessage(message))
    );
}

void Server::onResponseRequest(const std::string& message)
{
    auto writeResult = write(this->connectionHandle, message.c_str(), message.length());

    if (writeResult == -1)
    {
        // Throw response send error
    }

    this->unlockConnection();
}

void Server::unlockConnection()
{
    // Allow listener's thread to close the connection
    this->connectionActionAwaiting = false;
    this->connectionLockCnd.notify_one();
}
