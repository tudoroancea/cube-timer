//
// Created by Tudor Oancea on 02/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_TIMESLIST_HPP
#define CUBE_TIMER_TIMESLIST_HPP

#include "libs/rapidcsv.h"
#include "Duration.hpp"
#include "Scramble.hpp"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <filesystem>
#include <QLabel>

namespace csv = rapidcsv;

class Time {
public:
	enum Type{time, mo3, ao5, ao12};
protected:
	size_t rowCSV;
	Type type;
public:
	Time(size_t const& row, Type type);
	void setRowCSV(size_t const& row);
	[[nodiscard]] size_t const& getRowCSV() const;
	[[nodiscard]] Type getType() const;
	void showDoubleCLick();
};

class TimeItem : public Time, public QTableWidgetItem {
public:
	TimeItem(QString const& str, size_t const& row, Time::Type type);
	TimeItem(long long const& val, size_t const& row, Time::Type type);
};


class TimesList : public QTableWidget {
Q_OBJECT
private:
	// Utility methods =============
	/**
	 * @brief Deletes all the content of the QTableWidget and populates it with the current CSV file.
	 */
	void readCSV();
	/**
	 * @brief PBs for the best time, mo3, ao5 and ao12.
	 */
	 std::array<std::pair<Duration<long long>,size_t>,4> pbs;

private slots:
	//	Actions =========
	void modifyComment(int row);
	void moreInfo(int row);
	void deleteTime(int row);
	void tryScrambleAgain(int row);
	void copyScramble(int row);

	void treatDoubleClick(QTableWidgetItem* item);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

public:
	explicit TimesList(QWidget* parent = nullptr);
	~TimesList() override;
	/**
	 * @brief Add time to the currently handled CSV data. It won't be saved to a file before the program is closed ot an appropriate method is called.
	 * @param toAdd
	 */
	void addTime(Duration<long long int> const& toAdd, Scramble const& scramble, QDateTime const& timeStamp = QDateTime::currentDateTime(), QString const& comment = QString());

	enum ErrorType{missingMetadata};
	struct Error : public std::exception {
		ErrorType type_;
		explicit Error(ErrorType const& t) : type_(t) {}
		[[nodiscard]] ErrorType const& type() const {return type_;}
		[[nodiscard]] const char* what() const noexcept override {
			switch (type_) {
				case missingMetadata: return "Missing Metadata";
				default: return "TimesList::Error";
			}
		}
	};
	void emitSendPBs();

public slots:
	void print(int row, int col);

public: signals:
	void sendScramble(Scramble const& toTry);
	void sendPBs(std::array<std::pair<Duration<long long>,size_t>,4> const& pbs);
};


#endif //CUBE_TIMER_TIMESLIST_HPP
