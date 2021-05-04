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
	std::filesystem::path defaultPath;
	csv::Document resource;
	[[maybe_unused]] bool currentCSVIsDefault = true;

//	Utility methods
	void readCurrentCSV();
public:
	TimesList(char* const& argv0, QWidget* parent);
	~TimesList() override;
	void addTime(Duration<long long int> const& toAdd);

public slots:
	void loadDefaultCSV();
	void loadOtherCSV(const std::string& pathToCSV);
	void loadOtherCSV(const std::filesystem::path& pathToCSV);

	void saveCurrentCSV();
	void overrideDefaultCSVByCurrent();
	void saveCurrentCSVTo(const std::string& pathToCSV);
	void saveCurrentCSVTo(const std::filesystem::path& pathToCSV);
};


#endif //CUBE_TIMER_TIMESLIST_HPP
