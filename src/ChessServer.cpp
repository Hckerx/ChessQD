#include <iostream>
#include <boost/asio.hpp>
#include "chessServer.hpp"

using boost::asio::ip::tcp;

Server::Server() 
try : acceptor(io_context, tcp::endpoint(tcp::v4(), 12345)), socket(io_context) {

        std::cout << "Server started. Waiting for incoming connections..." << std::endl;

        acceptor.accept(socket);
    }
catch (std::exception& e)
{
    std::cerr << "Exception: " << e.what() << std::endl;
}

void Server::send(std::string message) {
    boost::asio::write(socket, boost::asio::buffer(message));
}

std::string Server::receive() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '\n');
    std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
    return data;
}
