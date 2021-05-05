//
// Created by Tudor Oancea on 02/05/2021.
//

#ifndef CUBE_TIMER_TIMESLIST_HPP
#define CUBE_TIMER_TIMESLIST_HPP

#include "libs/rapidcsv.h"
#include "Duration.hpp"

#include <QTableWidget>
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
public:
	TimesList(char* const& argv0, QWidget* parent);
	~TimesList() override;
	/**
	 * @brief Add time to the currently handled CSV data. It won't be saved to a file before the program is closed ot an appropriate method is called.
	 * @param toAdd
	 */
	void addTime(Duration<long long int> const& toAdd);
	/**
	 * @return currentCSVIsDefault value
	 */
	[[nodiscard]] bool isCurrentCSVDefault() const;

	enum Error{wrongPath, wrongFormat};
	static bool hasRightFormat(std::string const& pathToCSV);

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
};


#endif //CUBE_TIMER_TIMESLIST_HPP
