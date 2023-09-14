#include "Log.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <cstdlib>

namespace bobo
{
    class Sock
    {
        const static int gbacklog = 20;

    public:
        int Socket()
        {
            int listenSock = socket(AF_INET, SOCK_STREAM, 0);

            int opt = 1;
            setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

            if (listenSock < 0)
            {
                logMessage(FATAL, "socket fail[%d]: %s\n", errno, strerror(errno));
                exit(2);
            }
            logMessage(NORMAL, "create socket success, listensock: %d\n", listenSock);
            return listenSock;
        }
        void Bind(int listenSock, const uint16_t &port, std::string ip = "")
        {
            struct sockaddr_in local;
            memset(&local, 0, sizeof local);
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());
            local.sin_port = htons(port);
            if (bind(listenSock, (struct sockaddr *)&local, sizeof local) < 0)
            {
                logMessage(FATAL, "bind fail[%d]: %s\n", errno, strerror(errno));
                exit(3);
            }
        }
        void Listen(int listenSock)
        {
            if (listen(listenSock, gbacklog) < 0)
            {
                logMessage(FATAL, "listen fail[%d]: %s\n", errno, strerror(errno));
                exit(4);
            }
            logMessage(NORMAL, "init tcp server done...%s\n", strerror(errno));
        }
        int Accept(int listenSock, std::string *clientIp, uint16_t *clientPort)
        {
            struct sockaddr_in src;
            memset(&src, 0, sizeof src);
            socklen_t len = sizeof src;
            int serviceSock = accept(listenSock, (sockaddr *)&src, &len);
            if (serviceSock < 0)
            {
                logMessage(ERROR, "accept fail[%d]: %s\n", errno, strerror(errno));
                return -1;
            }
            if (clientPort)
                *clientPort = ntohs(src.sin_port); //*************
            if (clientIp)
                *clientIp = inet_ntoa(src.sin_addr); //*************
            return serviceSock;
        }
        bool Connect(int sock, const uint16_t &port, const std::string &ip)
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(ip.c_str());
            server.sin_port = htons(port);
            if (connect(sock, (struct sockaddr *)&server, sizeof server) == 0)
                return true;
            else
                return false;
        }
    };
}