#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class Client
{
public:
    Client();
    ~Client() {
        // Close the socket
        socket.close();
    }

    std::string receive();
    void send(std::string message);
private:
    boost::asio::io_context io_context;
    tcp::socket socket;

};
