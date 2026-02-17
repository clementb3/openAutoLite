#pragma once
#include "canService.h"


namespace openauto {
	class actionCan {
	public:
		static void onMessage(const service::CanMessage& msg);

	private:
		static void volumePlus();
		static void volumeMinus();
		static void nextTrack();
		static void previousTrack();
		static void playPause();
	};

}