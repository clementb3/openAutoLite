#include "actionCan.h"
#include <iostream>

namespace openauto {
	void actionCan::onMessage(const service::CanMessage &msg) {
		switch (msg.id) {
			case 0x1CA:
				std::cout << "[Can receive message] id [" << msg.data.size() << "] " << msg.id << " : " << msg.data[0]
						<< ":" << msg.data[1] <<
						std::endl;
				if (msg.data.size() == 4 && msg.data[0] == 0x03) {
					switch (msg.data[1]) {
						case 0x10:
							volumePlus();
							break;
						case 0x20:
							volumeMinus();
							break;
						case 0x01:
							nextTrack();
							break;
						case 0x02:
							previousTrack();
							break;
						default:
							break;
					}
				}
				break;
			case 0x1A8:
				std::cout << "[Can receive message] id [" << msg.data.size() << "] " << msg.id << " : " << msg.data[0]
						<< ":" << msg.data[1] <<
						std::endl;
				if (msg.data.size() == 2) {
					switch (msg.data[0]) {
						case 0x40:
							playPause();
							break;
						case 0x80:
							break;
						default: break;
					}
				}
				break;

			default:
				break;
		}
	}

	void actionCan::volumePlus() {
		std::cout << "[Action] Volume Up (+5%)" << std::endl;
		std::system("amixer -D bluealsa sset 'Z Fold4 de clement A2DP' 5%+");
	}

	void actionCan::volumeMinus() {
		std::cout << "[Action] Volume Down (-5%)" << std::endl;
		std::system("amixer -D bluealsa sset 'Z Fold4 de clement A2DP' 5%-");
	}

	void actionCan::nextTrack() {
		std::cout << "[Action] Skip Next" << std::endl;
		std::system("playerctl next");
	}

	void actionCan::previousTrack() {
		std::cout << "[Action] Skip Previous" << std::endl;
		std::system("playerctl previous");
	}

	void actionCan::playPause() {
		std::cout << "[Action] Play/Pause Toggle" << std::endl;
		std::system("playerctl play-pause");
	}
}
