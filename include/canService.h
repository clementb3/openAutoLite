#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <linux/can.h>

namespace service {
    struct CanMessage {
        uint32_t id;
        std::vector<uint8_t> data;
    };

    using MessageCallback = std::function<void(const CanMessage &)>;

    class CanService {
    public:
        explicit CanService();

        virtual ~CanService();

        CanService(const CanService &) = delete;

        CanService &operator=(const CanService &) = delete;

        bool init(const std::string &interfaceName);

        void stop();

        void run();

        void sendMessage(uint32_t id, const std::vector<uint8_t> &data);

        void setOnMessageReceived(MessageCallback callback);

    private:
        std::map<uint32_t, std::vector<uint8_t> > _lastMessages;
        int _socket{-1};
        std::atomic<bool> _running{false};
        MessageCallback _onMessageReceived;
    };
}
