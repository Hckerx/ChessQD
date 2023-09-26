#include <boost/asio/buffer.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include "Communication.hpp"


using boost::asio::ip::tcp;


Communication::Communication()


try : socket(io_context) {

     std::cout << "Connecting to server..." << std::endl;
        try {
         socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
         isServer = true;
         isWhite = std::rand() % 2 == 0;
         send(isWhite ? "black" : "white");
        
        } 
        catch (std::exception& e) {
         tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
         acceptor.accept(socket);

        receive();
            std::string color = receivedString;
            // std::string color = communication->noAsyncReceive();
            if (color == "black") {
                std::cout << "is black" << std::endl;
                isWhite = false; 
            } else {
                std::cout << "is white" << std::endl;
                isWhite = true;
            }
        
        if (!isWhite) {
            // This is some weird ass code (it's basically shit)
            //futurerecv = std::async(std::launch::async, std::bind(&Communication::receive, &(*communication)));
            // communication->receive(); when it was async 
            std::thread t2 (&Communication::receive, this);
            t2.detach();
        }
        }
       
        std::cout << "connected to server" << std::endl;
        std::cout << "isServer" << isServer << std::endl;
        std::cout << "Waiting for client..." << std::endl;
       
        std::cout << "connected to client" << std::endl;
        

    
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
    boost::asio::read_until(socket, receiveBuffer, '\n');
    std::string data = boost::asio::buffer_cast<const char*>(receiveBuffer.data());
    if (!data.empty() && data.back() == '\n') {
        data.pop_back();
    }
    receivedString = data;
    received = true;
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
        receivedString = line;
        std::cout << "Received: Async" << line << std::endl;
    } else {
        std::cout << "Error: " << error.message() << std::endl;
    }
}



