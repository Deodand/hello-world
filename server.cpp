#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int main(int argc, char* argv[]) {
    int server, client, clientNumber = 1;
    int portNum;
    int bufSize = 1024;
    char buffer[255];
    std::string nicknames[128];

    if(argc > 1) 
        portNum = atoi(argv[1]);
    else
        portNum = 15000;


    struct sockaddr_in server_addr;
    socklen_t sizeServerAddress;

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0) {
        std::cerr << "Error establishing socket...\n";
        exit(1);
    }

    std::cout << "Socket server using port " << portNum << " has been created.\n";


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    // fix trouble "port already in use"
    int optval = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    std::cout << "Binding server...\n";

    if ((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        std::cerr << "Error bind()";
        exit(2);
    }

    // listening
    if(listen(server, 10) < 0) {
        std::cerr << "Error listen()";
        exit(3);
    }
    std::cout << "Waiting clients...\n";

    // creating set of descriptors
    fd_set master;      // set of descriptors
    fd_set fdRead;
    int fdMax;          // max number of descriptors
    FD_ZERO(&master);
    FD_ZERO(&fdRead);

    // add server descriptor to master set of descriptors
    FD_SET(server, &master); 
    fdMax = server;

    while(1) {
        fdRead = master;
        if(select(fdMax+1, &fdRead, NULL, NULL, NULL) < 0) {
            std::cerr << "error select()";
            exit(4);
        }

        // check the descriptors for reading
        for(int i = 0; i <= fdMax; ++i) {
            if(FD_ISSET(i, &fdRead)) {  // descriptor i have some changes
                if(i == server) {
                    // we have new client
                    sizeServerAddress = sizeof(server_addr);
                    client = accept(server, 
                                    (struct sockaddr*)&server_addr,
                                    &sizeServerAddress);
                    if(client < 0) 
                        std::cerr << "Error accept()";
                    else {
                        read(client, buffer, 1024);
                        nicknames[client] = buffer;
                        
                        FD_SET(client, &master);
                        std::cout << "Client " << nicknames[client]
                                  << " has been connected\n";
                        clientNumber++;
                        if(client > fdMax) 
                            fdMax = client;
                            memset(buffer, 0, sizeof(buffer));
                    }
                }
                else {
                    // we have data from one of the clients
                    memset(buffer, 0, sizeof(buffer));
                    if((bufSize = read(i, buffer, 1024)) < 0) {
                        std::cerr << "Error read()";
                    }
                    else if(bufSize == 0) {
                        // client is offline now
                        std::cout << nicknames[i] << " is offline now\n";
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else {
                        // we have message from client
                        for(int j=0; j <= fdMax; ++j) {
                            // send this message and nickname of sender to all clients
                            if(FD_ISSET(j, &master)) {
                                if(j != server && j != i) {
                                    std::string temp(buffer, sizeof(buffer));
                                    if(write(j, (nicknames[i]+": "+temp).c_str(),
                                             bufSize+sizeof(nicknames[i])+2) < 0)
                                        std::cerr << "Error write()";
                                }
                            }
                        }
                    }
                }
            }
        }
    }





    std::cout << "Closing server...\n";
    close(server);

    return 0;
}
