#include <boost/asio.hpp>
#include "Communication.hpp"

using boost::asio::ip::tcp;

Communication::Communication(boost::asio::io_context& io_context) :io_context(io_context), socket(io_context) {
    try {
        // Connect to existing game
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
        std::cout << "socket Open in connecting if the following non zero: "<< socket.is_open() << std::endl;
        isConnected = true;
        send("white");
        isWhite = false;
        asyncReceive(); // Start asynchronous receive operation
        isServer = false;
    } catch (std::exception& e) {
        std::cout << "in server" << std::endl;
        // Else create game
        //std::cout << socket.is_open() << std::endl;
        init(); // Start server initialization
    }
}

void Communication::send(std::string message) {
    message += '\n';
    std::cout << "Sending: " << message << std::endl;
    boost::asio::async_write(
        socket, boost::asio::buffer(message),
        [](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                std::cerr << "Error sending message: " << ec.message() << std::endl;
            }
        });
    }

void Communication::init() {
    // create a new socket
    // check if socket is open
    socket.close();
    acceptor = new tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
    acceptor->async_accept(socket, [this](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Connection accepted" << std::endl;
            isConnected = true;
            asyncReceive(); // Start asynchronous receive operation
        } else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
    });
    std::cout << "Waiting for connection" << std::endl;
    std::cout << "io_context is running:" << !io_context.stopped() << std::endl;
}

    void Communication::asyncReceive() {
        boost::asio::async_read_until(
            socket, receiveBuffer, '\n',
            [this](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
                if (!ec) {
                    processData(); // Process the received data
                asyncReceive(); // Continue asynchronous receive operation
            } else {
                std::cerr << "Error receiving data: " << ec.message() << std::endl;
            }
        });
}

void Communication::processData() {
    std::istream is(&receiveBuffer);
    std::getline(is, data, '\n');
    std::cout << "Received: " << data << std::endl;
    received = true;

    if (data == "close") {
        socket.close();
    } else if (data == "black") {
        isWhite = false;
        data.clear();
    } else if (data == "white") {
        isWhite = true;
        data.clear();
    }
}

std::string Communication::read() {
    if (received) {
        received = false;
        std::string tmp = data;
        data.clear();
        return tmp;
    } else {
        return "";
    }
}
