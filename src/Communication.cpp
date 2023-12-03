#include <boost/asio.hpp>
#include "communication.hpp"

using boost::asio::ip::tcp;
using std::string;

/** 
* Communication is based on a client a server architecture
* Additionally, to ensure that the games renders while communicating this class uses asynchronous operations
* Similar to threading asynchronous operations allow to execute multiple operations at the same time
* For further information: https://www.baeldung.com/cs/async-vs-multi-threading
* In simple terms asynchronous operations are functions that don't block the execution of the program, e.g. a web request so
* the program can continue to execute while the request is being processed
*/



/*
* The constructor of the communication class
* It first tries to connect to an existing game
* If this fails it creates a new game
* @param io_context The io_context of the main program
* @param ip The ip address of the server
*/
Communication::Communication(boost::asio::io_context &io_context, const string& ip)
    : io_context(io_context), socket(io_context) {
  try {
    // Connect to existing game
    std::cout << "Trying to connect to " << ip << std::endl;
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
/**
* Sends a message to the other player
* @param message The message to be sent
*/
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
/**
* Creates an acceptor and waits for a connection(server)
* @param void
*/
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

/**
* Function that starts asynchronous receive operation
* Instead of returning the received data it stores it in a variable
* when the data has been received. This is done because asynchronous operations are non-blocking.
* @param void
*/
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

/**
* Processes the received data for special cases
* @param void
* @return void
*/
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
/**
* Function that is called every iteration of the main loop
* Checks if data has been received and if so returns it
* @param void
* @return std::string The received data
*/
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
