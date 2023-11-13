#include <boost/asio/buffer.hpp>
#include <cstdio>
#include <iostream>
#include <boost/asio.hpp>
#include "Communication.hpp"



using boost::asio::ip::tcp;


Communication::Communication()
try : socket(io_context) {

    try {
        std::cout << socket.is_open() << std::endl;
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
        isConnected = true;
        send("white");
        isWhite = false;
        std::thread receiveThread(&Communication::receive, this);
        receiveThread.detach();
        isServer = false;
    } 
    catch (std::exception& e) {
        std::cout << socket.is_open() << std::endl;
        socket.close();
        std::thread init(&Communication::init, this);
        init.detach();
    }      
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
void Communication::init(){
    acceptor = new tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
    std::cout << socket.is_open() << std::endl;
    acceptor->async_accept(socket,[this](const boost::system::error_code& error) {
    // if (!error) {
    // std::cout << "how did we get here?" << std::endl;
    // isConnected = true;
    // isServer = true;
    // isWhite = std::rand() % 2 == 0;
    // send(isWhite ? "black" : "white");       
    // received = true;
    // std::thread receiveThread(&Communication::receive, this);
    // receiveThread.detach();
    // } else {
         std::cerr << "Error accepting connection: " << error.message() << std::endl;
    // }
});  
std::thread receiveThread(&Communication::receive, this);
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
    std::cout << "socket.is_open()" << std::endl;
    boost::asio::streambuf receiveBuffer;  
    std::cout << "How did we really get here" << std::endl;
    while (true) {
        std::cout << socket.is_open() << std::endl;
        if (socket.is_open()) {
        boost::asio::read_until(socket, receiveBuffer, '\n');
        data = boost::asio::buffer_cast<const char*>(receiveBuffer.data());
        std::cout << "Received: " << data << std::endl;
        if (!data.empty() && data.back() == '\n') {
            data.pop_back();
        }
        if (data == "close") {
            socket.close();
            return;
        }
        if (data == "black") {
            isWhite = false; 
            data = "";
            received = true;
        } 
        else if (data == "white") {
            isWhite = true;
            data = "";
            received = true;
        }
    } }
}

std::string Communication::read(){
    
    if (received) {
        std::string tempdata = data;
        data = "";
        return tempdata;
    }
    else {
        return "";
    }
}

//void Communication::receive() {
//    received = false;
//    std::cout << "is receiving in async" << std::endl;
//    boost::asio::async_read_until(socket, receiveBuffer, '\n', boost::bind(&Communication::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//}
// We need to implement the handle_read function, which is called when the asynchronous read operation initiated by async_read_until has finished.
// void Communication::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
//     std::cout << "is handling" << std::endl;
//     if (!error) {

//         //std::string data = boost::asio::buffer_cast<const char*>(receiveBuffer.data());
//         std::istream is(&receiveBuffer);
//         std::string line;
//         std::getline(is, line);

//         if (!line.empty() && line.back() == '\n') {
//             line.pop_back();
//         }
//         received = true;
//         // receivedString = line;
//         std::cout << "Received: Async" << line << std::endl;
//     } else {
//         std::cout << "Error: " << error.message() << std::endl;
//     }
// }



