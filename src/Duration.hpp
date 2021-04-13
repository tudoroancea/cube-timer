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
	explicit Duration(long mil) {
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
	[[nodiscard]] QString toString() const {
		QString result;
		if (hours > 0) {
			result += std::to_string(hours).data();
			result += ":";
		}
		if (minutes > 0) {
			result += std::to_string(minutes).data();
			result += ":";
		}
		if (seconds > 0) {
			result += std::to_string(seconds).data();
			result += ".";
		}
		if (milliseconds > 0) {
			result += std::to_string(milliseconds).data();
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
