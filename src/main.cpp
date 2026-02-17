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
        can.setOnMessageReceived(openauto::actionCan::onMessage);
    }

    std::thread canThread(&service::CanService::run, &can);

    std::cout << "CAN Service started" << std::endl;

    canThread.join();

    return 0;
}