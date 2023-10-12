#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <string>

class Communication {
public:
    Communication(boost::asio::io_context& ioContext, const std::string& serverAddress, const std::string& serverPort);
    ~Communication();

    void connect();
    void send(const std::string& message);
    void receiveAsync(std::function<void(const std::string&)> callback);

private:
    void handleConnect(const boost::system::error_code& error);
    void handleSend(const boost::system::error_code& error, size_t bytesTransferred);
    void handleReceive(const boost::system::error_code& error, size_t bytesTransferred);

    boost::asio::io_context& ioContext;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;
    std::string serverAddress;
    std::string serverPort;
    std::string incomingData;
};
