#include "canService.h"
#include <iostream>
#include <cstring>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

namespace service {

    CanService::CanService() : _socket(-1), _running(false) {}

    CanService::~CanService() {
        stop();
    }

    bool CanService::init(const std::string& interfaceName) {
        _socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (_socket < 0) {
            perror("Error socket");
            return false;
        }

        struct ifreq ifr;
        std::strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ - 1);
        if (ioctl(_socket, SIOCGIFINDEX, &ifr) < 0) {
            perror("Error ioctl");
            return false;
        }

        struct sockaddr_can addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (bind(_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Error bind");
            return false;
        }

        _running = true;
        return true;
    }

    void CanService::run() {
        struct can_frame frame;

        while (_running) {
            int nbytes = read(_socket, &frame, sizeof(struct can_frame));

            if (nbytes < 0) {
                if (errno != EAGAIN && _running) {
                    perror("Error read CAN");
                }
                continue;
            }

            if (nbytes < (int)sizeof(struct can_frame)) {
                continue;
            }

            CanMessage msg;
            msg.id = frame.can_id;
            msg.data.assign(frame.data, frame.data + frame.can_dlc);

            if (_onMessageReceived) {
                _onMessageReceived(msg);
            }
        }
    }

    void CanService::sendMessage(uint32_t id, const std::vector<uint8_t>& data) {
        if (_socket < 0 || !_running) return;

        struct can_frame frame;
        std::memset(&frame, 0, sizeof(frame));

        frame.can_id = id;
        frame.can_dlc = static_cast<__u8>(std::min(data.size(), (size_t)8));

        std::memcpy(frame.data, data.data(), frame.can_dlc);

        if (write(_socket, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
            perror("Error send CAN");
        }
    }

    void CanService::setOnMessageReceived(MessageCallback callback) {
        _onMessageReceived = callback;
    }

    void CanService::stop() {
        _running = false;
        if (_socket >= 0) {
            close(_socket);
            _socket = -1;
        }
    }

}