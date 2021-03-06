#include "Rtc.h"
#include "config/Pins.h"
#include "Hardware.h"
#include "Logging.h"

#include <Wire.h>


using namespace utils;

namespace rtc {
	#define CURRENT_LOGGER "rtc"
	RTC_A_CLASS RTC_A;

	void setup() {
		#define CURRENT_LOGGER_FUNCTION "setup"
		VERBOSE;

		hardware::i2c::powerOn();
		RTC_A.control(DS3231_12H, DS3231_OFF); //24 hours clock
		RTC_A.control(DS3231_A1_INT_ENABLE, DS3231_OFF); //Alarm 1 OFF
		RTC_A.control(DS3231_INT_ENABLE, DS3231_ON); //INTCN ON
		hardware::i2c::powerOff();
	}

	int16_t getTemperature() {
		hardware::i2c::powerOn();
		float temperature = RTC_A.readTempRegister();
		hardware::i2c::powerOff();

		return static_cast<int16_t>(temperature * 100);
	}

	bool isAccurate() {
		hardware::i2c::powerOn();
		bool accurate = RTC_A.status(DS3231_HALTED_FLAG) == DS3231_OFF;
		hardware::i2c::powerOff();

		return accurate;
	}

	timestamp_t getTime() {
		tmElements_t time;
		getTime(time);
		return time::makeTimestamp(time);
	}

	void getTime(tmElements_t &time) {
		#define CURRENT_LOGGER_FUNCTION "getTime"

		hardware::i2c::powerOn();
		RTC_A.readTime(time);
		hardware::i2c::powerOff();

		VERBOSE_FORMAT("%d/%d/%d %d:%d:%d", time.year, time.month, time.day, time.hour, time.minute, time.second);
	}

	void setTime(const tmElements_t &time) {
		#define CURRENT_LOGGER_FUNCTION "setTime"
		VERBOSE_FORMAT("%d/%d/%d %d:%d:%d", time.year, time.month, time.day, time.hour, time.minute, time.second);

		hardware::i2c::powerOn();
		RTC_A.writeTime(time);
		RTC_A.control(DS3231_HALTED_FLAG, DS3231_OFF);
		hardware::i2c::powerOff();
	}

	void setAlarm(uint16_t seconds) {
		tmElements_t currentTime;
		tmElements_t alarmTime;

		hardware::i2c::powerOn();
		getTime(currentTime);
		time::breakTime(time::makeTimestamp(currentTime) + seconds, alarmTime);

		setAlarm(alarmTime);
		hardware::i2c::powerOff();
	}

	void setAlarm(const tmElements_t &time) {
		#define CURRENT_LOGGER_FUNCTION "setAlarm"

		hardware::i2c::powerOn();
		WRITE_ALARM_1(time);

		RTC_A.control(DS3231_A1_FLAG, DS3231_OFF); //reset Alarm 1 flag
		RTC_A.control(DS3231_A1_INT_ENABLE, DS3231_ON); //Alarm 1 ON
		RTC_A.control(DS3231_INT_ENABLE, DS3231_ON); //INTCN ON

		tmElements_t alarmTime;
		READ_ALARM_1(alarmTime);
		NOTICE_FORMAT("Next alarm : %d:%d:%d", alarmTime.hour, alarmTime.minute, alarmTime.second);

		hardware::i2c::powerOff();
	}

}