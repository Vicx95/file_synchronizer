#include "..//inc/file_streamer.hpp"

#include <boost/asio.hpp>
#include <iostream>



TxtFileStreamer::TxtFileStreamer(const std::string& ip, const int port, std::vector<std::string>& fileList) :
    m_ip(ip), m_port(port), m_fileList(fileList)
{
};
void TxtFileStreamer::startStreaming() 
{

std::cout<<"Started streaming to endpoint: " << m_ip << ":" << m_port << '\n';
std::cout << "Files: \n";
for(const auto& file : m_fileList)
{
        std::cout << file << '\n';
}

// boost::asio::ip::udp::endpoint remote_endpoint{boost::asio::ip::endpoint("192.168.0.1", 9999)};
// boost::asio::io_service contex;
// boost::asio::ip::udp::socket socket{context};

// while (auto data = getData()) {
//     boost::system::error_code err;
//     auto sent = socket_.send_to(boost::asio::mutable_buffer(data.data(), data.size()), remote_endpoint, 0, err);
//     Logger(INF) << std::format("send {} bytes of data", sent);
//     if (err.failed()) {
//         Logger(ERR) << std::format("Error during sending data to address ({}:{}): {}",
//                         remote_endpoint.address()
//                         remote_endpoint.port()
//                         err.message());
//         break;
//     }
// }
};

void TxtFileStreamer::stopStreaming() 
{
    std::cout<<"Stopped streaming to endpoint: " << m_ip << ":" << m_port << '\n';
};

void TxtFileStreamer::updateNetworkConfig(const std::string& ip, const int port) 
{
    std::cout<<"Updating network config with: " << ip << ":" << port << '\n';
};

void TxtFileStreamer::updateFileList(std::vector<std::string>& fileList) 
{
    std::cout << "Updating network config with: \n";
    for(const auto& file : fileList)
    {
        std::cout << file << '\n';
    }
};

