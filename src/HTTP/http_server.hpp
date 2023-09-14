#include"Log.hpp"
#include"Sock.hpp"

#include<unistd.h>
#include<signal.h>
#include<functional>

namespace bobo
{
    class HttpServer
    {
        using Func = std::function<void(int)>;
    public:
        HttpServer(uint16_t port, Func func)
            :_listenSock(Sock().Socket())
            ,_port(port)
            ,_func(func)
        {
            Sock().Bind(_listenSock, _port);
            Sock().Listen(_listenSock);
        }
        void start()
        {
            signal(SIGCHLD, SIG_IGN);
            while(true)
            {
                std::string clientIp;
                uint16_t clientPort;
                int serviceSock = Sock().Accept(_listenSock, &clientIp, &clientPort);
                if(serviceSock < 0) continue;
                if(fork() == 0)
                {
                    // 子进程
                    close(_listenSock);
                    _func(serviceSock);
                    close(serviceSock);
                    exit(0);
                }
                // 父进程
                close(serviceSock);
            }
        }
        ~HttpServer()
        {
            if(_listenSock >= 0) close(_listenSock);
        }
    private:
        int _listenSock;
        uint16_t _port;
        Func _func;
    };
}