#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
class Communication
{
public:
    bool received = false;
    Communication();
    ~Communication() {
        socket.close();
    }
    bool isWhite;
    void send(std::string message);
    void receive();
    [[nodiscard]] std::string read();
    std::string noAsyncReceive();
    boost::asio::io_context io_context;


private:
    std::thread receiveThread;
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    tcp::socket socket;
    bool isServer = false;
    std::string data;
};

