#include <boost/asio/buffer.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include "Communication.hpp"



using boost::asio::ip::tcp;


Communication::Communication()
try : socket(io_context) {

    try {
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
        isServer = false;
        std::cout << "1"<< std::endl;
    } 
    catch (std::exception& e) {
        socket.close();
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        acceptor.accept(socket);
        isServer = true;
        isWhite = std::rand() % 2 == 0;
        send(isWhite ? "black" : "white");       
    }      
    std::thread receiveThread(&Communication::receive, this);
    receiveThread.detach();
}
catch (std::exception& e)
{
    std::cerr << "Exception: " << e.what() << std::endl;    
} 



void Communication::send(std::string message) {
    message += '\n';
    std::cout << "Sending: " << message << std::endl;
    int bytes = boost::asio::write(socket, boost::asio::buffer(message));
    std::cout << "bytes_transferred:" << bytes << std::endl;
}


// std::string Communication::receive() {
//     boost::asio::streambuf buffer;
//     boost::asio::read_until(socket, buffer, '\n');
//     std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
//     if (!data.empty() && data.back() == '\n') {
//         data.pop_back();
//     }
//     return data;
// }
//

void Communication::receive() {
    while (true) {
        boost::asio::read_until(socket, receiveBuffer, '\n');
        data = boost::asio::buffer_cast<const char*>(receiveBuffer.data());
        if (!data.empty() && data.back() == '\n') {
            data.pop_back();
            received = true;
        }
        std::cout << data << std::endl;
        if (data == "black") {
            std::cout << "is black" << std::endl;
            isWhite = false; 
            data = "";
        } 
        else if (data == "white") {
            std::cout << "is white" << std::endl;
            isWhite = true;
            data = "";
        }
    }
}

std::string Communication::read(){
    std::string tempdata = data;
    data = "";
    return tempdata;
}

//void Communication::receive() {
//    received = false;
//    std::cout << "is receiving in async" << std::endl;
//    boost::asio::async_read_until(socket, receiveBuffer, '\n', boost::bind(&Communication::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//}
// We need to implement the handle_read function, which is called when the asynchronous read operation initiated by async_read_until has finished.
void Communication::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    std::cout << "is handling" << std::endl;
    if (!error) {

        //std::string data = boost::asio::buffer_cast<const char*>(receiveBuffer.data());
        std::istream is(&receiveBuffer);
        std::string line;
        std::getline(is, line);

        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }
        received = true;
        // receivedString = line;
        std::cout << "Received: Async" << line << std::endl;
    } else {
        std::cout << "Error: " << error.message() << std::endl;
    }
}



