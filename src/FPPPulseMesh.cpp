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


class FPPPulseMeshPlugin : public FPPPlugin, public MultiSyncPlugin {
    
public:
    FPPPulseMeshPlugin() : FPPPlugin("fpp-PulseMesh") {
        LogInfo(VB_PLUGIN, "Initializing PulseMesh Plugin\n");
        initSocket();
    }
    virtual ~FPPPulseMeshPlugin() {
        closeSocket();
        MultiSync::INSTANCE.removeMultiSyncPlugin(this);
    }

    virtual void SendMediaOpenPacket(const std::string &filename) override {
        std::string message = "SendMediaOpenPacket/" + mediaFilename;
        writeToSocket(message);
    }

    virtual void SendMediaSyncStartPacket(const std::string &filename) override {
        std::string message = "SendMediaSyncStartPacket/" + mediaFilename;
        writeToSocket(message);
    }

    virtual void SendMediaSyncStopPacket(const std::string &filename) override {
        std::string message = "SendMediaSyncStopPacket/" + mediaFilename;
        writeToSocket(message);
    }

    virtual void SendMediaSyncPacket(const std::string &filename, float seconds) override {
        std::string message = "SendMediaSyncPacket/" + mediaFilename + "/" + mediaType + "/" + std::to_string(seconds);
        writeToSocket(message);
    }

private:
    int sockfd;
    struct sockaddr_un addr;

    void initSocket() {
        sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
            return;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "/tmp/fpp_pulse_mesh_socket");

        if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Socket connect error: " << strerror(errno) << std::endl;
            close(sockfd);
            sockfd = -1;
        }
    }

    void closeSocket() {
        if (sockfd >= 0) {
            close(sockfd);
        }
    }

    void writeToSocket(const std::string &message) {
        if (sockfd < 0) {
            std::cerr << "Socket not connected" << std::endl;
            return;
        }

        if (send(sockfd, message.c_str(), message.size(), 0) < 0) {
            std::cerr << "Socket send error: " << strerror(errno) << std::endl;
        }
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new FPPPulseMeshPlugin();
    }
}