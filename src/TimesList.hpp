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
#include <QDateTime>
#include <filesystem>

namespace csv = rapidcsv;

class TimesList : public QTableWidget {
Q_OBJECT
private:
	/**
	 * @brief Absolute canonical path to the default.csv file in the bundle Resources.
	 */
	std::filesystem::path defaultPath;
	/**
	 * @brief rapidcsv Document with the current handled csv data.
	 */
	csv::Document resource;
	/**
	 * @brief pretty self-explanatory
	 */
	[[maybe_unused]] bool currentCSVIsDefault = true;

//	Utility methods
	/**
	 * @brief Deletes all the content of the QTableWidget and populates it with the current CSV file.
	 */
	void readCurrentCSV();


private slots:
	//	Actions =========
	void modifyComment(int row);
	void moreInfo(int row);
	void deleteTime(int row);
	//void redoScramble(int row);
	void copyScramble(int row);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;

public:
	TimesList(char* const& argv0, QWidget* parent);
	~TimesList() override;
	/**
	 * @brief Add time to the currently handled CSV data. It won't be saved to a file before the program is closed ot an appropriate method is called.
	 * @param toAdd
	 */
	void addTime(Duration<long long int> const& toAdd, Scramble const& scramble, QDateTime const& timeStamp = QDateTime::currentDateTime(), QString const& comment = QString());
	/**
	 * @return currentCSVIsDefault value
	 */
	[[nodiscard]] bool isCurrentCSVDefault() const;

	enum Error{wrongPath, wrongFormat};
	static bool hasRightFormat(std::string const& pathToCSV);
	//static void recomputeStatistics(std::string const& pathToCSV);


public slots:
	/**
	 * @brief Load data from default CSV. WARNING: does not saveToCurrentCSV previously loaded data
	 */
	void loadDefaultCSV();
	/**
	 * @brief Load data from custom CSV. WARNING: does not saveToCurrentCSV previously loaded data
	 * @param pathToCSV
	 */
	void loadCustomCSV(const std::string& pathToCSV);
	/**
	 * @brief Saves data to the currently loaded CSV (default or custom location)
	 */
	void saveToCurrentCSV();
	/**
	 * @brief Saves data to the default CSV (overrides it).
	 */
	void saveToDefaultCSV();
	/**
	 * @brief Saves data to a custom location. If the given CSV file already exists overrides it, otherwise creates a new file.
	 * @param pathToCSV
	 */
	void saveToCustomCSV(const std::string& pathToCSV);
	void print(int row, int col);
	void showContextMenu(QPoint const& pos);

};


#endif //CUBE_TIMER_TIMESLIST_HPP
