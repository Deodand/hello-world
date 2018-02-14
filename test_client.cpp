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
    int bufsize = 1024;
    char buffer[bufsize];

    // wtf strange bug
    std::string buf;
    //fd_set set;

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
    /*
    std::cout << "Your message: ";
    std::cin >> buffer;
    write(client, buffer, sizeof(buffer));
    std::cout << "Sended message: " << buffer << '\n';

    read(client, buffer, bufsize);
    std::cout << "Server message: " << buffer << std::endl;
*/
    close(client);
    return 0;
}