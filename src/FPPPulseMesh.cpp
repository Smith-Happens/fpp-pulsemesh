#include "fpp-pch.h"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

#include "Plugin.h"
#include "MultiSync.h"
#include "Plugin.h"
#include "Plugins.h"


class FPPPulseMeshPlugin : public FPPPlugin, public MultiSyncPlugin {
    
public:
    FPPPulseMeshPlugin() : FPPPlugin("fpp-PulseMesh") {
        LogInfo(VB_PLUGIN, "Initializing PulseMesh Connector Plugin\n");
        multiSync->addMultiSyncPlugin(this);
        initSocket();
    }
    virtual ~FPPPulseMeshPlugin() {
        closeSocket();
        MultiSync::INSTANCE.removeMultiSyncPlugin(this);
    }

    virtual void SendMediaOpenPacket(const std::string &filename) override {
        std::string message = "SendMediaOpenPacket/" + filename;
        writeToSocket(message);
    }

    virtual void SendMediaSyncStartPacket(const std::string &filename) override {
        std::string message = "SendMediaSyncStartPacket/" + filename;
        writeToSocket(message);
    }

    virtual void SendMediaSyncStopPacket(const std::string &filename) override {
        std::string message = "SendMediaSyncStopPacket/" + filename;
        writeToSocket(message);
    }

    virtual void SendMediaSyncPacket(const std::string &filename, float seconds) override {
        std::string message = "SendMediaSyncPacket/" + filename + "/" + std::to_string(seconds);
        writeToSocket(message);
    }

private:
    int sockfd;
    struct sockaddr_un addr;

    void initSocket() {
        sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
            LogInfo(VB_PLUGIN, "Socket creation error:\n");
            LogInfo(VB_PLUGIN, strerror(errno));
            return;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "/var/run/fppd/PULSE");

        LogInfo(VB_PLUGIN, "Init socket is complete\n");

        // unlink(addr.sun_path);
        // if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        //     std::cerr << "Socket bind error: " << strerror(errno) << std::endl;
        //     close(sockfd);
        //     sockfd = -1;
        // }
    }

    void closeSocket() {
                LogInfo(VB_PLUGIN, "Closing socket\n");

        if (sockfd >= 0) {
            close(sockfd);
        }
    }

    void writeToSocket(const std::string &message) {
        if (sockfd < 0) {
            std::cerr << "Socket not connected" << std::endl;
            LogInfo(VB_PLUGIN, "Socket not connected\n");
            return;
        }
                LogInfo(VB_PLUGIN, "Writing to socket\n");
                LogInfo(VB_PLUGIN, message);


        // if (send(sockfd, message.c_str(), message.size(), 0) < 0) {
        //     std::cerr << "Socket send error: " << strerror(errno) << std::endl;
        // }
        if (sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Socket send error: " << strerror(errno) << std::endl;
            LogInfo(VB_PLUGIN, "Socket set error\n");
            LogInfo(VB_PLUGIN, strerror(errno));
        }
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new FPPPulseMeshPlugin();
    }
}
