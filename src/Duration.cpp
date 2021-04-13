//
// Created by Tudor Oancea on 13/04/2021.
//

#include "Duration.hpp"

//[[maybe_unused]] Duration::Duration(long h, long min, long s, long mil) : hours(std::max<long>(h,0)), minutes(std::max<long>(min,0)), seconds(std::max<long>(s,0)), milliseconds(std::max<long>(mil,0)) {}
//
//Duration::Duration(long mil) {
//	if (mil <= 0) {
//		hours = 0;
//		minutes = 0;
//		seconds = 0;
//		milliseconds = 0;
//	} else {
//		hours = mil / 3600000;
//		mil %= 3600000;
//		minutes = mil / 60000;
//		mil %= 60000;
//		seconds = mil / 1000;
//		mil %= 1000;
//		milliseconds = mil;
//	}
//}
//
//QString Duration::toString() const {
//	QString result;
//	if (hours > 0) {
//		result += std::to_string(hours).data();
//		result += ":";
//	}
//	if (minutes > 0) {
//		result += std::to_string(minutes).data();
//		result += ":";
//	}
//	if (seconds > 0) {
//		result += std::to_string(seconds).data();
//		result += ".";
//	}
//	if (milliseconds > 0) {
//		result += std::to_string(milliseconds).data();
//	}
//	return result;
//}