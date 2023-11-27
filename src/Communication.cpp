#include <boost/asio.hpp>
#include "Communication.hpp"

using boost::asio::ip::tcp;

Communication::Communication() : socket(io_context) {
    try {
        // Connect to existing game
        std::cout << socket.is_open() << std::endl;
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
        isConnected = true;
        send("white");
        isWhite = false;
        asyncReceive(); // Start asynchronous receive operation
        isServer = false;
    } catch (std::exception& e) {
        // Else create game
        std::cout << socket.is_open() << std::endl;
        std::cout << "socket connected" << std::endl;
    } 
    catch (std::exception& e) {
        socket.close();
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
        acceptor = new tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << socket.is_open() << std::endl;
        acceptor->async_accept(socket, [this](const boost::system::error_code& ec) {
            if (!ec) {
                std::cout << "Connection accepted" << std::endl;
                asyncReceive(); // Start asynchronous receive operation
            } else {
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
            }
        });
    }

    void Communication::asyncReceive() {
        std::cout << "socket.is_open()" << std::endl;
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

    if (data == "close") {
        socket.close();
    } else if (data == "black") {
        isWhite = false;
        received = true;
    } else if (data == "white") {
        isWhite = true;
        received = true;
    }

    data.clear(); // Clear the buffer for the next read
}

std::string Communication::read() {
    if (received) {
        received = false;
        return data;
    } else {
        return "";
    }
}
