#include "CanService.h"
#include <thread>
#include <iostream>

int main() {
    service::CanService can;

    if (!can.init("can0")) {
        std::cerr << "Erreur init CAN" << std::endl;
        return 1;
    }

    can.setOnMessageReceived([](const service::CanMessage& msg) {
        std::cout << "Receive ID: 0x" << std::hex << msg.id << " | Data size: " << std::dec << msg.data.size() << std::endl;
        });

    std::thread canThread(&service::CanService::run, &can);

    std::cout << "CAN Service started" << std::endl;

    canThread.join();

    return 0;
}