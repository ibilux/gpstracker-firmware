#pragma once

#include <Arduino.h>

#define BACKUP_ENABLE_SDCARD 0
#define BACKUP_ENABLE_NETWORK 1

#if BACKUP_ENABLE_NETWORK
#include "NetworkPositionsConfig.h"
#endif

#define SIM808_BAUDRATE 4800

#define CONFIG_ADDR 0
#define CONFIG_RESERVED_SIZE 128
#define CONFIG_SEED 13
#define VERSION "1.10"

#define SLEEP_TIMING_TIME(hours, minutes) hours * 60 + minutes

#pragma region Default configuration values

#define MENU_TIMEOUT	10000
/**
 \def SLEEP_DEFAULT_TIME_SECONDS
 Hard coded value for default sleep time between position acquisitions.
 Exprimed in seconds
*/

#define CONFIG_DEFAULT_BATTERY_ALERT_LEVEL1			45
#define CONFIG_DEFAULT_BATTERY_ALERT_LEVEL2			38
#define CONFIG_DEFAULT_BATTERY_ALERT_CLEAR			60
#define CONFIG_DEFAULT_ACTIVE_ALERTS				0
#define CONFIG_DEFAULT_CONTACT_PHONE				"+642568452"

#define SLEEP_DEFAULT_TIME_SECONDS						1800
#define SLEEP_DEFAULT_STOPPED_THRESHOLD					5
#define SLEEP_DEFAULT_PAUSING_TIME_SECONDS				270

#define SLEEP_TIMING_MIN SLEEP_TIMING_TIME(0, 0)
#define SLEEP_TIMING_MAX SLEEP_TIMING_TIME(23, 59)
#define SLEEP_TIMING_MIN_MOVING_VELOCITY 5

#define GPS_DEFAULT_INTERMEDIATE_TIMEOUT_MS				10000L
#define GPS_DEFAULT_TOTAL_TIMEOUT_MS					80000L
#define GPS_DEFAULT_MISSED_POSITION_GAP_KM				2

#define NETWORK_DEFAULT_INTERMEDIATE_TIMEOUT_MS			10000L
#define NETWORK_DEFAULT_TOTAL_TIMEOUT_MS				80000L
#define NETWORK_DEFAULT_NO_NETWORK_QUALITY_THRESHOLD	8
#define NETWORK_DEFAULT_NO_NETWORK_TRIES				5

#define ALERTS_ON_SERIAL_IF_AVAILABLE					1
/**
 \def ALERTS_ON_SERIAL_IF_AVAILABLE
 Display alerts on serial when connected rather than sending an SMS.
 Useful for debugging purpose and avoid costs related to SMS sending.
*/
#define ALERT_SUSPICIOUS_RTC_TEMPERATURE				0

#pragma endregion

struct sleepTimings_t {
	uint8_t speed;
	uint16_t timeMin;
	uint16_t timeMax;
	uint16_t seconds;
};

struct config_t {
	uint8_t seed;					//sizeof = 1
	char version[5];				//sizeof = 5
	uint16_t firstEntry;			//sizeof = 2
	uint16_t lastEntry;				//sizeof = 2
#if BACKUP_ENABLE_NETWORK
	networkConfig_t network;		//sizeof = 73
#else
	char reserved[73];
#endif
	uint8_t alertBatteryLevel1;		//sizeof = 1
	uint8_t alertBatteryLevel2;		//sizeof = 1
	uint8_t alertBatteryLevelClear; //sizeof = 1
	uint8_t activeAlerts;			//sizeof = 1
	char contactPhone[15];			//sizeof = 15
};									//sizeof = 29 + 73 = 102

namespace config {

	static const sleepTimings_t defaultSleepTimings[] PROGMEM = {
		{ 0, SLEEP_TIMING_TIME(16, 00), SLEEP_TIMING_TIME(19, 59), 3600 },
		{ 0, SLEEP_TIMING_TIME(20, 00), SLEEP_TIMING_MAX, SLEEP_DEFAULT_TIME_SECONDS },
		{ 0, SLEEP_TIMING_MIN, SLEEP_TIMING_TIME(8, 29), SLEEP_DEFAULT_TIME_SECONDS },
		{ 0, SLEEP_TIMING_TIME(8, 30), SLEEP_TIMING_TIME(15, 59), 10800 },

		{ SLEEP_TIMING_MIN_MOVING_VELOCITY, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 540 },
		{ 10, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 270 },
		{ 20, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 225 },
		{ 30, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 240 },
		{ 45, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 280 },
		{ 65, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 276 },
		{ 85, SLEEP_TIMING_MIN, SLEEP_TIMING_MAX, 225 }
	};

	namespace main {
		extern config_t value;

		void setup();
		void save();

		void reset();
	}
}