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
//#include "Plugins.h"
#include "Warnings.h"


class FPPPulseMeshPlugin : public FPPPlugin, public MultiSyncPlugin {
    
public:
    FPPPulseMeshPlugin() : FPPPlugin("fpp-PulseMesh"), m_lastMediaHalfSecond(0) {
        LogInfo(VB_PLUGIN, "Initializing PulseMesh Connector Plugin\n");

        MultiSync::INSTANCE.addMultiSyncPlugin(this);

        if (!MultiSync::INSTANCE.isMultiSyncEnabled()) {
            WarningHolder::AddWarning("PulseMesh Connector Plugin enabled, but MultiSync is not enabled.  Please enable MultiSync to use PulseMesh Connector.");
        }

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
        int curTS = (seconds * 2.0f);
        if (m_lastMediaHalfSecond == curTS) {
            return;
        }
        m_lastMediaHalfSecond = curTS;
        std::string message = "SendMediaSyncPacket/" + filename + "/" + std::to_string(seconds);
        writeToSocket(message);
    }

private:
    int sockfd;
    struct sockaddr_un addr;
    int m_lastMediaHalfSecond;

    void initSocket() {
        sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            LogErr(VB_PLUGIN, "Socket creation error: %s\n", strerror(errno));
            return;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "/tmp/PULSE");
    }

    void closeSocket() {
        if (sockfd >= 0) {
            close(sockfd);
        }
    }

    void writeToSocket(const std::string &message) {
        if (sockfd < 0) {
            LogErr(VB_PLUGIN, "Socket not connected\n");
            return;
        }

        if (sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            LogErr(VB_PLUGIN, "Socket send error: %s\n", strerror(errno));
        }
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new FPPPulseMeshPlugin();
    }
}
