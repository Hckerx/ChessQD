#include <iostream>
#include <boost/asio.hpp>
#include "chessCommunication.hpp"

using boost::asio::ip::tcp;


Communication::Communication(bool Server) 
try : socket(io_context), isServer(Server) {
    if (isServer) {
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        acceptor.accept(socket);
    } else {
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
    }
}
catch (std::exception& e)
{
    std::cerr << "Exception: " << e.what() << std::endl;
}

void Communication::send(std::string message) {
    boost::asio::write(socket, boost::asio::buffer(message));
}

std::string Communication::receive() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '\n');
    std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
    if (!data.empty() && data.back() == '\n') {
        data.pop_back();
    }
    return data;
}




