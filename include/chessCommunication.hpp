#include <boost/asio.hpp>

using boost::asio::ip::tcp;
class Communication
{
public:
    Communication(bool Server);
    ~Communication() {
        socket.close();
    }
    void send(std::string message);
    std::string receive();

private:
    boost::asio::io_context io_context;
    tcp::socket socket;
    bool isServer;
};

