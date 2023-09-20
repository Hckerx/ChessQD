#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
class Communication
{
public:
    bool received;
    std::string receivedString;
    Communication(bool Server);
    ~Communication() {
        socket.close();
    }
    void send(std::string message);
    std::string receive();

private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    boost::asio::streambuf receiveBuffer;  
    boost::asio::io_context io_context;
    tcp::socket socket;
    bool isServer;
};

