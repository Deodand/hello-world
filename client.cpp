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


int main(int argc, char* argv[]) {
    int client;
    int portNum;
    int bufSize = 1024;
    char buffer[bufSize];
    std::string buf;

    if(argc > 1) 
        portNum = atoi(argv[1]);
    else
        portNum = 15000;

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        std::cerr << "Error establishing socket...\n" ;
        exit(1);
    }

    std::cout << "Socket client has been created.\n" ;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        std::cout << "Connection to the server port number: " 
                  << portNum << std::endl;
    else {
        std::cerr << "Connection to the server port number: " 
                  << portNum << " was failed.\n";
        return -1;
    }

    // send to server your nickname
    std::cout << "Please, enter your nickname:\n";
    std::cin >> buf;
    if(write(client, buf.c_str(), sizeof(buf)) <= 0)
        std::cerr << "Error write()";
    buf.clear();

    // creating set of descriptors
    fd_set fdClient;
    fd_set fdRead;
    int fdMax = client;
    FD_ZERO(&fdClient);
    FD_ZERO(&fdRead);
    FD_SET(client, &fdClient);
    FD_SET(STDIN_FILENO, &fdClient);

    while(1) {
        fdRead = fdClient;
        if(select(fdMax+1, &fdRead, NULL, NULL, NULL) < 0) {
            std::cerr << "error select()";
            exit(4);
        }

        if (FD_ISSET(STDIN_FILENO, &fdRead)) {
            // client have message, you need to send it to server
            memset(buffer, 0, sizeof(buffer));
            if((bufSize = read(STDIN_FILENO, buffer, 1024)) <= 0) {
                std::cerr << "Error read()";
                exit(5);
            }
            if(write(client, buffer, bufSize-1) < 0) {
                std::cerr << "Error write()";
                exit(6);
            }
        }

        if (FD_ISSET(client, &fdRead)) {
            if((bufSize = read(client, buffer, 1024)) < 0) {
                std::cerr << "Error read()";
            }
            else if(bufSize == 0) {
                // server is offline now
                std::cout << "Server is offline now\n";
                return 0;
            }
            else {
                // we have a message from server
                std::cout << buffer << '\n';
            }
        }
    }


    close(client);
    return 0;
}