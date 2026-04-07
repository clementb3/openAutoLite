#include "canService.h"
#include "bluetoothService.h"
#include "actionCan.h"
#include <thread>
#include <iostream>

int main() {
    service::CanService canService;
    service::BluetoothService bluetoothService;
    
    std::thread bluetoothThread(&service::BluetoothService::run, &bluetoothService);
    std::cout << "bluetooth Service started" << std::endl;

    if (!canService.init("can0")) {
        std::cerr << "Erreur init CAN" << std::endl;
    }
    else {
        canService.setOnMessageReceived(openauto::actionCan::onMessage);
        std::thread canThread(&service::CanService::run, &canService);
        std::cout << "CAN Service started" << std::endl;
        if (canThread.joinable()) canThread.join();
    }

    if (bluetoothThread.joinable()) bluetoothThread.join();

    return 0;
}