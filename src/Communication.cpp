#include <boost/asio.hpp>
#include "communication.hpp"

using boost::asio::ip::tcp;

Communication::Communication(boost::asio::io_context &io_context, std::string ip) : io_context(io_context), socket(io_context) {
    try {
        // Connect to existing game
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(ip), 12345));
        isConnected = true;
        send("white");
        isWhite = false;
        asyncReceive(); // Start asynchronous receive operation
        isServer = false;
    } catch (std::exception &e) {
        // Else create game
        init(); // Start server initialization
    }
}

void Communication::send(std::string message) {
    message += '\n';
    boost::asio::async_write(
            socket, boost::asio::buffer(message),
            [](const boost::system::error_code &ec, std::size_t /*bytes_transferred*/) {
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
    acceptor->async_accept(socket, [this](const boost::system::error_code &ec) {
        if (!ec) {
            isConnected = true;
            asyncReceive(); // Start asynchronous receive operation
        } else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
    });
}

void Communication::asyncReceive() {
    boost::asio::async_read_until(
            socket, receiveBuffer, '\n',
            [this](const boost::system::error_code &ec, std::size_t /*bytes_transferred*/) {
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
    received = true;

    std::cout << data << std::endl;
    if (data == "close") {
        close();
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
