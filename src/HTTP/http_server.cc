#include"http_server.hpp"
#include"Log.hpp"
#include"Util.hpp"

#include<memory>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace bobo;

#define ROOT "./wwwroot"
void service(int sock)
{
    char buffer[1024];
    ssize_t s = recv(sock, buffer, sizeof buffer - 1, 0);
    if(s > 0) buffer[s] = 0;

    std::vector<std::string> vline;
    Util::cutString(buffer, "\n", &vline);

    std::vector<std::string> vblock;
    Util::cutString(vline[0], " ", &vblock);
    
    std::string file = vblock[1];
    std::string target = ROOT;
    if(file == "/") file = "/index.html";
    target += file;
    //std::cout << target << std::endl;
    FILE* pf = fopen("./message", "a");
    fputs(target.c_str(), pf);
    fputs("\n", pf);

    std::string content;
    std::ifstream in(target);
    if(in.is_open())
    {
        std::string line;
        while(std::getline(in, line))
        {
            content += line;
        }
        in.close(); // ******************
    }

    std::string httpResponse;
    if(content.empty()) httpResponse = "http/1.1 404 NotFound\r\n";
    else httpResponse = "http/1.1 200 OK\r\n";
    httpResponse += "\r\n";
    httpResponse += content;

    send(sock, httpResponse.c_str(), httpResponse.size(),0);
}

void MyDaemon()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    if(fork() > 0) exit(0);
    setsid();
    int fd = open("/dev/null", O_RDWR);
    if(fd > 0)
    {
        dup2(0, fd);
        dup2(1, fd);
        dup2(2, fd);
        close(fd);
    }
}
void usage(const char* progress)
{
    logMessage(ERROR, "Usage: %s ip\n", progress);
}
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }
    MyDaemon();
    std::unique_ptr<HttpServer> svr(new HttpServer(atoi(argv[1]), service));
    svr->start();

    return 0;
}