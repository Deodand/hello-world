#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    int client, server, clientNumber = 0;
    int portNum;
    int bufsize = 1024;
    char buffer[bufsize];

    if(argc > 1) 
        portNum = atoi(argv[1]);
    else
        portNum = 15000;

    struct sockaddr_in server_addr;
    socklen_t sizeServerAddress;

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0) 
    {
        std::cerr << "Error establishing socket..!11!..\n";
        exit(1);
    }

    std::cout << "Socket server using port " << portNum << " has been created.\n";


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    std::cout << "Binding server...\n";

    if ((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        std::cerr << "Error binding connection...\n";
        return -1;
    }

    sizeServerAddress = sizeof(server_addr);


    while(1) {
        std::cout << "Waiting clients...\n";
        ++clientNumber;
        listen(server, 5);

        client = accept(server,(struct sockaddr *)&server_addr, &sizeServerAddress);

        if (client < 0) {
            std::cerr << "Error accepting client " << clientNumber << "...\n";
            return -1;
        }
        else
            std::cout << "Client " << clientNumber << " has been connected.\n";


        read(client, buffer, bufsize);
        std::cout << "Client message: " << buffer << std::endl;
        strcpy(buffer, "Your message accepted");
        write(client, buffer, bufsize);

        std::cout << "Server message has been sended.\n\n";
        
        close(client);
    }

    std::cout << "Closing server...\n";
    close(server);

    return 0;
}