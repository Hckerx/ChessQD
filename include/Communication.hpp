#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;
class Communication
{
public:
    bool received = false;
    Communication();
    void close();
    bool isWhite = true;
    void send(std::string message);
    void receive();
    [[nodiscard]] std::string read();
    std::string noAsyncReceive();
    boost::asio::io_context io_context;
    bool isConnected = false;
    void asyncReceive();

    
    tcp::acceptor* acceptor = nullptr;

    ~Communication() {
        std::cout << "Communication destructor" << std::endl;
        if (isConnected) {
            send("closse");
            acceptor->close();
        }
        if (acceptor != nullptr) {
            std::cout << "closing acceptor" << std::endl;
            
            delete acceptor;
        }
        socket.close();
    }
private:
    
    void processData();
    boost::asio::streambuf receiveBuffer;
    std::thread receiveThread;
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    tcp::socket socket;
    bool isServer = false;
    std::string data;
    void init();
};

