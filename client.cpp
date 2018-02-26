#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <thread>

void reading(int client) {
    int bufSize;
    char buffer[1024];
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        if((bufSize = read(client, buffer, 1024)) < 0) {
            std::cerr << "Error: read()\n";
        }
        else if(bufSize == 0) {
            std::cout << "Server is closed\n";
            break;
        }
        else {
            std::cout << buffer << '\n';
        }
    }
}
void writing(int client) {
    int bufSize;
    char buffer[1024];
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        if((bufSize = read(STDIN_FILENO, buffer, 1024)) <= 0)
        {
            std::cerr << "Error: read from STRIN\n";
            break;
        }
        if((write(client, buffer, bufSize-1)) < 0)
        {
            std::cerr << "Error: read()\n";
        }
    }
}


int main(int argc, char* argv[]) {
    int portNum;
    std::string buf;

    if(argc > 1) 
        portNum = atoi(argv[1]);
    else
        portNum = 15000;


    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) 
    {
        std::cerr << "Error establishing socket...\n" ;
        exit(1);
    }

    std::cout << "Socket client has been created.\n" ;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        std::cout << "Connection to the server port number: " 
                  << portNum << std::endl;
    else {
        std::cerr << "Connection to the server port number: " 
                  << portNum << " was failed.\n";
        exit(2);
    }

    // send to server your nickname
    std::cout << "Please, enter your nickname:\n";
    std::cin >> buf;
    if(write(client, buf.c_str(), sizeof(buf)) <= 0)
        std::cerr << "Error write()";
    buf.clear();

    // working with threads
    std::thread readThread(reading, client);
    std::thread writeThread(writing, client);

    readThread.join();
    writeThread.join();


    close(client);
    return 0;
}