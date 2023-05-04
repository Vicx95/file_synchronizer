#pragma once

#include <vector>
#include <string>

using DirsAndFiles = std::pair<std::string, std::vector<std::string>>;

class i_FileStreamer{
public:
    virtual ~i_FileStreamer() = default;
    virtual void startStreaming() = 0;
    virtual void stopStreaming() = 0;
    virtual void updateNetworkConfig(const std::string& ip, const int port) = 0;
    virtual void updateFileList(std::vector<std::string>& fileList)  = 0;
};

class TxtFileStreamer : public i_FileStreamer{
public:
    TxtFileStreamer(const std::string& ip, const int port, std::vector<std::string>& fileList);
    void startStreaming() override;
    void stopStreaming() override;
    void updateNetworkConfig(const std::string& ip, const int port) override;
    void updateFileList(std::vector<std::string>& fileList) override;

private:
    std::string m_ip; 
    int m_port;
    std::vector<std::string> m_fileList;
};
