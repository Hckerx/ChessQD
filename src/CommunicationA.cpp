#include "CommunicationA.hpp"

Communication::Communication(boost::asio::io_context& ioContext, const std::string& serverAddress, const std::string& serverPort)
    : ioContext(ioContext), socket(ioContext), resolver(ioContext), serverAddress(serverAddress), serverPort(serverPort) {}

Communication::~Communication() {
    socket.close();
}

void Communication::connect() {
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(serverAddress, serverPort);
    boost::asio::async_connect(socket, endpoints,
        [](const boost::system::error_code& error, boost::asio::ip::tcp::endpoint) {
            if (!error) {
                // Connection successful
                // You can perform additional setup here if needed
            }
        });
}

void Communication::send(const std::string& message) {
    boost::asio::async_write(socket, boost::asio::buffer(message),
        [](const boost::system::error_code& error, size_t bytesTransferred) {
            if (!error) {
                // Data sent successfully
            }
        });
}

void Communication::receiveAsync(std::function<void(const std::string&)> callback) {
    boost::asio::async_read_until(socket, boost::asio::dynamic_buffer(incomingData), '\n',
        [this, callback](const boost::system::error_code& error, size_t bytesTransferred) {
            if (!error) {
                // Data received successfully
                callback(incomingData);
                incomingData.clear();
            }
        });
}

void Communication::handleConnect(const boost::system::error_code& error) {
    if (!error) {
        // Connection successful
        // You can perform additional setup here if needed
    }
}

void Communication::handleSend(const boost::system::error_code& error, size_t bytesTransferred) {
    if (!error) {
        // Data sent successfully
    }
}

void Communication::handleReceive(const boost::system::error_code& error, size_t bytesTransferred) {
    if (!error) {
        // Data received successfully
        // Process the received data
    }
}
