#pragma once

#include <Arduino.h>
#include "RtcAbstraction.h"

namespace utils {
	namespace time {

		void breakTime(const timestamp_t time, tmElements_t &tm);
		timestamp_t makeTimestamp(const tmElements_t &tm);
	}
}
