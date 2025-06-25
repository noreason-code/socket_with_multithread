#include <iostream>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>

std::atomic<int> active_connections(0);
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

    if(bind(sockfd, (struct sockaddr*)&rcv_addr, sizeof(rcv_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void sock_rcv(int sockfd) {
    char buffer[1024];
    while(true) {
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);//
        if(bytes_received < 0) {
            std::cerr << "Error receiving message" << std::endl;
            close(sockfd);
            return;
        }
        if(bytes_received == 0) {
            std::cout << "Connection closed by sender" << std::endl;
            close(sockfd);
            active_connections--;
            return;
        }
        std::cout << "From " << sockfd << " received message: " << std::string(buffer, bytes_received) << std::endl;
    }
}

int main(int argc, char** argv) {
    std::string rcv_addr = argv[1];     // Receiver IP address
    int rcv_port = std::stoi(argv[2]);  // Receiver port

    int sockfd = create_socket(rcv_addr, rcv_port);
    if(sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }
    if(listen(sockfd, 5) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        close(sockfd);
        return -1;
    }
    
    // Set timeout for select
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

    while(true) {
        std::cout << "Current active connections: " << active_connections.load() << ". Waiting for connection with 20s timeout..." << std::endl;
        int ret = select(sockfd + 1, &read_fds, nullptr, nullptr, &timeout);
        if(ret < 0) {
            std::cerr << "Error in select" << std::endl;
            close(sockfd);
            return -1;
        }else if(ret == 0) {
            if(active_connections.load() == 0) {
                std::cout << "Timeout: no incoming connection in 20 seconds." << std::endl;
                break;
            }else {
                std::cout << "No incoming connection but active connections." << std::endl;
                continue;
            } 
        }else {
            int snd_sock = accept(sockfd, nullptr, nullptr); //
            if(snd_sock < 0) {
                std::cerr << "Error accepting connection" << std::endl;
                close(sockfd);
                return -1;
            }else {
                active_connections++;
                std::thread rcv_thread(sock_rcv, snd_sock);
                rcv_thread.detach();
            }
        }
    }
    close(sockfd);

    return 0;
}