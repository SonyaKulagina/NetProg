#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(13);
    if (inet_pton(AF_INET, "172.16.40.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Ошибка при преобразовании IP-адреса" << std::endl;
        return 1;
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(sockfd);
        return 1;
    }
    char buffer[1024];
    ssize_t bytes_sent = sendto(sockfd, buffer, 0, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0) {
        std::cerr << "Ошибка при отправке данных" << std::endl;
        close(sockfd);
        return 1;
    }
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);
    ssize_t bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&recv_addr, &addr_len);
    if (bytes_received < 0) {
        std::cerr << "Ошибка при получении данных" << std::endl;
        close(sockfd);
        return 1;
    }
    buffer[bytes_received] = '\0';
    std::cout << "Ответ сервера: " << buffer << std::endl;
    close(sockfd);
    return 0;
}
