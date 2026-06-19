#pragma once

#include <string>
#include <atomic>

namespace service {
    class BluetoothService {
    public:
        explicit BluetoothService();

        virtual ~BluetoothService();

        BluetoothService(const BluetoothService &) = delete;

        BluetoothService &operator=(const BluetoothService &) = delete;

        void stop();

        void run() const;
        static std::string getDevice();

      private:
        std::atomic<bool> _running{false};

        static std::string readLastDevice();

        static void saveDevice(const std::string &mac);

        static bool isBluetoothConnected();
    };
}
