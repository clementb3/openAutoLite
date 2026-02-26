#include "canService.h"
#include "actionCan.h"
#include <thread>
#include <iostream>

int main() {
    service::CanService can;

    if (!can.init("can0")) {
        std::cerr << "Erreur init CAN" << std::endl;
        return 1;
    }
    else {
		can.sendMessage(0x416, { 0xFC, 0x16, 0x3F, 0xFF, 0xFF , 0xFF, 0xFF, 0xFF });
        can.setOnMessageReceived(openauto::actionCan::onMessage);
    }

    std::thread canThread(&service::CanService::run, &can);

    std::cout << "CAN Service started" << std::endl;

    canThread.join();

    return 0;
}