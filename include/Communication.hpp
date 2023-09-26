#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
class Communication
{
public:
    bool received = false;
    std::string receivedString;
    Communication();
    ~Communication() {
        socket.close();
    }
    bool isWhite;
    void send(std::string message);
    void receive();
    std::string noAsyncReceive();
    boost::asio::io_context io_context;
private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    boost::asio::streambuf receiveBuffer;  
    tcp::socket socket;
    bool isServer = false;
};

