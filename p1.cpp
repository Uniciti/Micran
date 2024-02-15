#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iomanip>
#include <signal.h>

const int BUFFER_SIZE = 1024;
int sockFd;
volatile int clientPack = 0;
volatile int serverPack = 0;

// Обработка сигнала для завершения процесса
void signalHandler(int signal, bool isClient){
    if (isClient){
        std::cout << "Total packs sent: " << clientPack << std::endl;
    }
    else {
        std::cout << "Total packs received: " << serverPack << std::endl;
    }
    close(sockFd);
    exit(0);
}

// Функция перевода пакета в HEX формат
std::string stringToHex(const std::string& input) {
    std::string result = "";
    for (char c : input) {
        int ascii = static_cast<int>(c);
        std::stringstream ss;
        ss << std::hex << std::uppercase << ascii;
        result += ss.str();
    }
    return result;
}

// Функция генерации случайного сообщения
std::string randomData(int size) {
    std::string data;
    for (int i = 0; i < size; i++) {
        data.push_back('A' + rand() % 26);
    }
    return data;
}

// Функция для отправки данных
void sendData(int sockFd, int threadId, int packetSize, int packetsPerSecond, bool verbose) {
    while (true) {
        std::string data = randomData(packetSize);
        std::string hexData = stringToHex(data);
        if (verbose) {
            std::cout << "Thread " << threadId << " sending data: " << hexData << std::endl;
        }
        send(sockFd, hexData.c_str(), hexData.size(), 0);
        clientPack++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / packetsPerSecond));
    }
}

// Функция для приема данных
void receiveData(int sockFd, int threadId, bool verbose) {
    while (true) {
        char buffer[BUFFER_SIZE];
        int bytesReceived = recv(sockFd, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            break;
        }
        buffer[bytesReceived] = '\0';
        std::string data(buffer);
        if (verbose) {
            std::cout << "Thread " << threadId << " received data: " << data << std::endl;
        }
        serverPack++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        std::cerr << "Use: " << argv[0] << " <mode: server/client> <max_clients int> <num_threads int> <packet_size int> <packets_per_second int> <verbose 1/0>" << std::endl;
        return 1;
    }
    std::string mode = argv[1];
    int maxClients = std::stoi(argv[2]);
    int numThreads = std::stoi(argv[3]);
    int packetSize = std::stoi(argv[4]);
    int packetsPerSecond = std::stoi(argv[5]);
    bool verbose = std::stoi(argv[6]);

    // Создание сокета
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        std::cerr << "Socket error" << std::endl;
        return 1;   
    }

    // Серверный режим
    if (mode == "server") {
        signal(SIGINT, [](int signal) { signalHandler(signal, false); });
        sockaddr_in serv_addr, cli_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(8080);

        if (bind(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Binding error" << std::endl;
            return 1;
        }

        listen(sockFd, maxClients);
        std::cout << "Server listening..." << std::endl;

        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; i++) {
            int newSockFd = accept(sockFd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr);
            if (newSockFd < 0) {
                std::cerr << "Connection acceptance error" << std::endl;
                continue;
            }
            threads.emplace_back(receiveData, newSockFd, i + 1, verbose);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
    // Клиентский режим
    else if (mode == "client") {
        signal(SIGINT, [](int signal) { signalHandler(signal, true); });
        std::string serverIp = "127.0.0.1";
        sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(8080);
        inet_pton(AF_INET, serverIp.c_str(), &serv_addr.sin_addr);

        if (connect(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Server connection error" << std::endl;
            return 1;
        }

        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; i++) {
            threads.emplace_back(sendData, sockFd, i + 1, packetSize, packetsPerSecond, verbose);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
    // Некорректный режим
    else {
        std::cerr << "Invalid mode. use: <server/client>" << std::endl;
        return 1;
    }
    std::cout << "Total packs received: " << serverPack << std::endl;
    close(sockFd);
    return 0;
}
