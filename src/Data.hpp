//
// Created by Tudor Oancea on 23/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_DATA_HPP
#define CUBE_TIMER_DATA_HPP

#include "MainWindow.hpp"
#include "libs/rapidcsv.h"
#include <string>

namespace csv = rapidcsv;

class Data : public csv::Document {
public:
	Data() = default;
	explicit Data(std::string const& pathToCSV);
	void Load(std::string const& pathToCSV);

	template <typename T>
	T getTime(size_t row, size_t session = MainWindow::currentSession);
	template <typename T>
	T getMO3(size_t row, size_t session = MainWindow::currentSession);
	template <typename T>
	T getAO5(size_t row, size_t session = MainWindow::currentSession);
	template <typename T>
	T getAO12(size_t row, size_t session = MainWindow::currentSession);
	template <typename T>
	T getScramble(size_t row, size_t session = MainWindow::currentSession);
	template <typename T>
	T getTimeStamp(size_t row, size_t session = MainWindow::currentSession);
	template <typename T>
	T getComment(size_t row, size_t session = MainWindow::currentSession);

	template <typename T>
	void setTime(size_t row, T const& val, size_t session = MainWindow::currentSession);
	template <typename T>
	void setMO3(size_t row, T const& val, size_t session = MainWindow::currentSession);
	template <typename T>
	void setAO5(size_t row, T const& val, size_t session = MainWindow::currentSession);
	template <typename T>
	void setAO12(size_t row, T const& val, size_t session = MainWindow::currentSession);
	template <typename T>
	void setScramble(size_t row, T const& val, size_t session = MainWindow::currentSession);
	template <typename T>
	void setTimeStamp(size_t row, T const& val, size_t session = MainWindow::currentSession);
	template <typename T>
	void setComment(size_t row, T const& val, size_t session = MainWindow::currentSession);

	bool hasRightFormat();
	size_t sessionRowCount(size_t session = MainWindow::currentSession);
	void recomputeStatistics(size_t const& session = MainWindow::currentSession);
};

template<typename T>
T Data::getTime(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.time, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
T Data::getMO3(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.mo3, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
T Data::getAO5(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.ao5, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
T Data::getAO12(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.ao12, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
T Data::getScramble(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.scramble, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
T Data::getTimeStamp(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.timeStamp, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
T Data::getComment(size_t row, size_t session) {
	if (row < this->GetRowCount()) {
		return this->GetCell<T>(std::to_string(session)+MainWindow::metadataHeaders.comment, row);
	} else if (typeid(T) == typeid(std::string)) {
		return T();
	} else {
		return 0;
	}
}

template<typename T>
void Data::setTime(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.time), row, val);
}

template<typename T>
void Data::setMO3(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.mo3), row, val);
}

template<typename T>
void Data::setAO5(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.ao5), row, val);
}

template<typename T>
void Data::setAO12(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.ao12), row, val);
}

template<typename T>
void Data::setScramble(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.scramble), row, val);
}

template<typename T>
void Data::setTimeStamp(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.timeStamp), row, val);
}

template<typename T>
void Data::setComment(size_t row, T const& val, size_t session) {
	this->SetCell<T>(this->GetColumnIdx(std::to_string(session)+MainWindow::metadataHeaders.comment), row, val);
}



#endif //CUBE_TIMER_DATA_HPP