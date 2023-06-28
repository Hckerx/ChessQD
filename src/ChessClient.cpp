#include <iostream>
#include <boost/asio.hpp>
#include "chessClient.hpp"

using boost::asio::ip::tcp;


Client::Client() 
try : socket(io_context) {

        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));

}
catch (std::exception& e)
{
    std::cerr << "Exception: " << e.what() << std::endl;
}

void Client::send(std::string message) {
    boost::asio::write(socket, boost::asio::buffer(message));
}

std::string Client::receive() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '\n');
    std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
    return data;
}




