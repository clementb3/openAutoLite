#include "bluetoothService.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

namespace service {
    BluetoothService::BluetoothService() : _running(true) {
    }

    BluetoothService::~BluetoothService() {
        stop();
    }

    void BluetoothService::run() const {
        std::string lastMac = readLastDevice();

        if (!lastMac.empty()) {
            std::cout << "[BT] connect to " << lastMac << std::endl;
            std::system(("bluetoothctl connect " + lastMac).c_str());
            if (int result = std::system(("bluetoothctl connect " + lastMac).c_str()); result == 0) {
                std::cout << "[BT] connected" << std::endl;
            }
        }
        while (_running) {
            if (!isBluetoothConnected()) {
                std::cout << "[BT] search device" << std::endl;

                std::system("bluetoothctl power on");
                std::system("bluetoothctl agent NoInputNoOutput");
                std::system("bluetoothctl discoverable on");
                std::system("bluetoothctl pairable on");

                while (!isBluetoothConnected()) {
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }

                std::cout << "[BT] mac adress" << getDevice() << std::endl;

                saveDevice(getDevice());
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    std::string BluetoothService::readLastDevice() {
        std::ifstream file("last_device.txt");
        if (std::string mac; file >> mac) return mac;
        return "";
    }

    void BluetoothService::saveDevice(const std::string &mac) {
        std::ofstream file("last_device.txt");
        file << mac;
    }

    bool BluetoothService::isBluetoothConnected() {
        std::array<char, 128> buffer{};
        std::string result;

        const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("bluetoothctl devices Connected", "r"), pclose);
        if (!pipe) {
            return false;
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return !result.empty();
    }

    std::string BluetoothService::getDevice() {
        std::array<char, 128> buffer{};
        std::string result;

        const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("bluetoothctl devices Connected", "r"), pclose);
        if (!pipe) {
            return "null";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result.substr(7, 17);
    }

    void BluetoothService::stop() {
        _running = false;
    }
}
