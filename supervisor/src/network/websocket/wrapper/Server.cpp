#include "Server.hpp"

#include <functional>
#include <utility>
#include <exception>

#include "events/NewConnection.hpp"
#include "events/Disconnection.hpp"
#include "events/Message.hpp"
#include "events/Terminate.hpp"
#include "events/OutputMessage.hpp"
#include "events/OutputMultiMessage.hpp"
#include "events/BroadcastMessage.hpp"

using tin::network::websocket::wrapper::Server;
namespace events = tin::network::websocket::wrapper::events;

Server::Server(const unsigned int& portNo):
portNo(portNo),
receiverHelperThread(*this, this->receiverQueue),
transmitterHelperThread(*this, this->transmitterQueue),
visitor(*this)
{
    this->server.init_asio();
    this->server.set_reuse_addr(true);

    this->server.clear_access_channels(websocketpp::log::alevel::all);

    this->server.set_open_handler(
        std::bind(
            &Server::onConnectionOpen,
            this,
            std::placeholders::_1
        )
    );
    this->server.set_close_handler(
        std::bind(
            &Server::onConnectionClose,
            this,
            std::placeholders::_1
        )
    );
    this->server.set_message_handler(
        std::bind(
            &Server::onMessageReceive,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    this->server.listen(this->portNo);
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
    this->server.stop();
    this->receiverHelperThread.terminate();
    this->transmitterHelperThread.terminate();
    this->receiverQueue.push(
        EventPtr(new events::Terminate())
    );
    this->transmitterQueue.push(
        EventPtr(new events::Terminate())
    );
}

unsigned int Server::attachConnectionOpenedHandler(
    std::function<void(const unsigned int&)>& handler
)
{
    return this->connectionOpenedHandlers.insert(handler);
}

unsigned int Server::attachConnectionClosedHandler(
    std::function<void(const unsigned int&)>& handler
)
{
    return this->connectionClosedHandlers.insert(handler);
}

unsigned int Server::attachMessageReceivedHandler(
    std::function<void(const unsigned int&, const std::string&)>& handler
)
{
    return this->messageReceivedHandlers.insert(handler);
}

unsigned int Server::attachConnectionOpenedHandler(
    std::function<void(const unsigned int&)>&& handler
)
{
    return this->connectionOpenedHandlers.insert(
        std::forward<std::function<void(const unsigned int&)>>(handler)
    );
}

unsigned int Server::attachConnectionClosedHandler(
    std::function<void(const unsigned int&)>&& handler
)
{
    return this->connectionClosedHandlers.insert(
        std::forward<std::function<void(const unsigned int&)>>(handler)
    );
}

unsigned int Server::attachMessageReceivedHandler(
    std::function<void(const unsigned int&, const std::string&)>&& handler
)
{
    return this->messageReceivedHandlers.insert(
        std::forward<std::function<void(const unsigned int&, const std::string&)>>(handler)
    );
}

void Server::sendMessage(
    unsigned int& connectionID,
    const std::string& message,
    const websocketpp::frame::opcode::value& method
)
{
    this->transmitterQueue.push(
        EventPtr(
            new events::OutputMessage(
                connectionID,
                message,
                method
            )
        )
    );
}

void Server::sendMessage(
    const std::vector<unsigned int>& connectionIDs,
    const std::string& message,
    const websocketpp::frame::opcode::value& method
)
{
    this->transmitterQueue.push(
        EventPtr(
            new events::OutputMultiMessage(
                connectionIDs,
                message,
                method
            )
        )
    );
}

void Server::broadcastMessage(
    const std::string& message,
    const websocketpp::frame::opcode::value& method
)
{
    this->transmitterQueue.push(
        EventPtr(
            new events::BroadcastMessage(
                message,
                method
            )
        )
    );
}


unsigned int Server::insertNewConnection(
    websocketpp::connection_hdl& connectionHandler
)
{
    std::lock_guard<std::recursive_mutex> lock(this->accountingMtx);

    this->connections.insert(
        std::make_pair(
            this->nextConnectionID,
            connectionHandler
        )
    );
    this->connectionIDs.insert(
        std::make_pair(
            connectionHandler,
            this->nextConnectionID
        )
    );

    return this->nextConnectionID++;
}

void Server::runConnectionOpenedHandlers(const unsigned int& idx)
{
    for (auto iter: this->connectionOpenedHandlers)
    {
        iter.second(idx);
    }
}

void Server::runConnectionClosedHandlers(const unsigned int& idx)
{
    for (auto iter: this->connectionClosedHandlers)
    {
        iter.second(idx);
    }
}

void Server::runMessageReceivedHandlers(
    const unsigned int& idx,
    const std::string& message
)
{
    for (auto iter: this->messageReceivedHandlers)
    {
        iter.second(idx, message);
    }
}

unsigned int Server::removeConnection(
    websocketpp::connection_hdl& connectionHandler
)
{
    std::lock_guard<std::recursive_mutex> lock(this->accountingMtx);

    auto connectionID = this->getConnectionID(connectionHandler);

    this->connectionIDs.erase(connectionHandler);
    this->connections.erase(connectionID);

    return connectionID;
}


void Server::listenerLoop()
{
    this->server.start_accept();
    this->server.run();
}

void Server::onConnectionOpen(
    websocketpp::connection_hdl connectionHandler
)
{
    this->receiverQueue.push(
        EventPtr(new events::NewConnection(connectionHandler))
    );
}

void Server::onConnectionClose(
    websocketpp::connection_hdl connectionHandler
)
{
    this->receiverQueue.push(
        EventPtr(new events::Disconnection(connectionHandler))
    );
}

void Server::onMessageReceive(
    websocketpp::connection_hdl connectionHandler,
    websocketpp::server<websocketpp::config::asio>::message_ptr message
)
{
    this->receiverQueue.push(
        EventPtr(new events::Message(connectionHandler, message))
    );
}

unsigned int Server::getConnectionID(
    websocketpp::connection_hdl& connectionHandler
)
{
    std::lock_guard<std::recursive_mutex> lock(this->accountingMtx);

    auto iter =  this->connectionIDs.find(connectionHandler);
    return iter->second;
}

websocketpp::connection_hdl& Server::getConnectionHandler(
    const unsigned int& connectionID
)
{
    std::lock_guard<std::recursive_mutex> lock(this->accountingMtx);

    return this->connections.at(connectionID);
}

