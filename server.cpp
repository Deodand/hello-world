#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *threadFunc(void *val1) {
    int locVal1 = * (int *) val1;
    for(int i=0; i<10; ++i) {
        std::cout << locVal1 << std::endl;
        //++locVal1;
    }
}

int main(int argc, char* argv[]) {
    int client1, client2, server, clientNumber = 0;
    int portNum;
    int bufsize = 1024;
    char buffer[bufsize];
    int optval = 1;

    /*pthread_t threadId1, threadId2;
    int value1 = 1;
    int value2 = 2;

    pthread_create(&threadId1, NULL, threadFunc, &value1);
    pthread_create(&threadId2, NULL, threadFunc, &value2);

    pthread_join(threadId1, NULL);
    pthread_join(threadId2, NULL);*/

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

    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    std::cout << "Binding server...\n";

    if ((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        std::cerr << "Error binding connection...\n";
        return -1;
    }

    sizeServerAddress = sizeof(server_addr);


    while(1) {
        listen(server, 10);
        std::cout << "Waiting clients...\n";

        client1 = accept(server,(struct sockaddr *)&server_addr, &sizeServerAddress);
        if (client1 < 0) {
            std::cerr << "Error accepting client " << clientNumber << "...\n";
            return -1;
        }
        else {
            ++clientNumber;
            std::cout << "Client " << clientNumber << " has been connected.\n";
        }

        read(client1, buffer, bufsize);
        std::cout << "Client message: " << buffer << std::endl;
        write(client1, "Your message accepted!", sizeof("Your message accepted!"));

        client2 = accept(server,(struct sockaddr *)&server_addr, &sizeServerAddress);
        if (client2 < 0) {
            std::cerr << "Error accepting client " << clientNumber << "...\n";
            return -1;
        }
        else {
            ++clientNumber;
            std::cout << "Client " << clientNumber << " has been connected.\n";
        }

        //strcpy(buffer, "Your message accepted");
        write(client2, buffer, bufsize);

        std::cout << "Server message has been sended.\n\n";
        
        close(client1);
        close(client2);
    }

    std::cout << "Closing server...\n";
    close(server);

    return 0;
}
