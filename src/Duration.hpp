//
// Created by Tudor Oancea on 13/04/2021.
//

#ifndef CUBE_TIMER_DURATION_HPP
#define CUBE_TIMER_DURATION_HPP

#include <QString>

template<typename T>
class Duration {
private:
	T hours;
	T minutes;
	T seconds;
	T milliseconds;
public:
	[[maybe_unused]] Duration(T h, T min, T s, T mil) : hours(h), minutes(min), seconds(s), milliseconds(mil) {}
	Duration(long mil) {
		if (mil <= 0) {
			hours = 0;
			minutes = 0;
			seconds = 0;
			milliseconds = 0;
		} else {
			hours = mil / 3600000;
			mil %= 3600000;
			minutes = mil / 60000;
			mil %= 60000;
			seconds = mil / 1000;
			mil %= 1000;
			milliseconds = mil;
		}
	}
	[[nodiscard]] QString toQString() const {
		return QString(toString().c_str());
	}

	[[nodiscard]] std::string toString() const {
		std::string result;
		bool h(false),mins(false),secs(false);
		if (hours > 0) {
			h = true;
			result += std::to_string(hours);
			result += ":";
		}
		if (minutes > 0) {
			mins = true;
			if (h && minutes <= 9) {
				result += "0";
			}
			result += std::to_string(minutes);
			result += ":";
		} else if (h) {
			result += "00:";
		}
		if (seconds > 0) {
			secs = true;
			if (mins && seconds <= 9) {
				result += "0";
			}
			result += std::to_string(seconds);
			result += ".";
		} else if (mins) {
			result += "00.";
		}
		if (milliseconds > 0) {
			if (result.empty()) {
				result += "0.";
			}
			if (milliseconds <= 99) {
				result += "0";
			}
			if (milliseconds <= 9) {
				result += "0";
			}
			result += std::to_string(milliseconds);
		} else {
			if (!secs) {
				result += "0.";
			}
			result += "000";
		}
		return result;
	}

//	Getters ==============
	T getHours() const {return hours;}
	T getMinutes() const {return minutes;}
	T getSeconds() const {return seconds;}
	T getMilliseconds() const {return milliseconds;}
};


#endif //CUBE_TIMER_DURATION_HPP
