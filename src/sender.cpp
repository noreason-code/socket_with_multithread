#include <iostream>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

int create_socket(const std::string& addr, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    struct sockaddr_in rcv_addr;
    memset(&rcv_addr, 0, sizeof(rcv_addr));
    rcv_addr.sin_family = AF_INET;
    rcv_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    rcv_addr.sin_port = htons(port);

    if(connect(sockfd, (struct sockaddr*)&rcv_addr, sizeof(rcv_addr)) < 0) {
        std::cerr << "Error connecting to receover" << std::endl;
        close(sockfd);
        return -1;
    }

    return sockfd;    
}

int main(int argc, char** argv) {
    std::string rcv_addr = argv[1];     // Receiver IP address
    int rcv_port = std::stoi(argv[2]);  // Receiver port
    std::string msg = argv[3];          // Message to send

    int sockfd = create_socket(rcv_addr, rcv_port);
    if(sockfd < 0) {
        return -1;
    }

    while(true) {
        if(send(sockfd, msg.c_str(), msg.size(), 0) < 0) {
            std::cerr << "Error sending message" << std::endl;
            close(sockfd);
            return -1;
        }
        sleep(1);
    }
    close(sockfd);
    
    return 0;
}