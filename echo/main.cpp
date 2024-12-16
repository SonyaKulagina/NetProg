#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 7777
#define BUFFER_SIZE 1024
using namespace std;
int main() {
    int server, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Сокет не создался");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Привязка не удалась");
        close(server);
        exit(EXIT_FAILURE);
    }
    if (listen(server, 3) < 0) {
        perror("Прослушивание не удалось");
        close(server);
        exit(EXIT_FAILURE);
    }
    cout << "Сервер слушает порт " << PORT << endl;
    if ((new_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Соединение не удалось");
        close(server);
        exit(EXIT_FAILURE);
    }
    while (true) {
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            cout << "Клиент отключился." << endl;
            break;
        }
        send(new_socket, buffer, valread, 0);
        cout << "Ответ отправлен: " << buffer << endl;
        memset(buffer, 0, BUFFER_SIZE);
    }
    close(new_socket);
    close(server);
    return 0;
}
