#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

#define defaultFen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
class Communication {
public:
    bool received = false;
    boost::asio::io_context &io_context;

    Communication(boost::asio::io_context &io_context, const std::string& ip);

    void close() {
        if (acceptor != nullptr && isConnected) {
            acceptor->close();
            delete acceptor;
        }
        socket.close();
    }

    bool isWhite = true;

    void send(std::string message);

    void receive();

    [[nodiscard]] std::string read();

    std::string noAsyncReceive();

    bool isConnected = false;

    void asyncReceive();


    tcp::acceptor *acceptor = nullptr;

    ~Communication() {
        close();
    }

private:

    void processData();

    boost::asio::streambuf receiveBuffer;
    std::thread receiveThread;

    void handle_read(const boost::system::error_code &error, size_t bytes_transferred);

    tcp::socket socket;
    bool isServer = false;
    std::string data;

    void init();
};

