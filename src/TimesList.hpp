//
// Created by Tudor Oancea on 02/05/2021.
//

#ifndef CUBE_TIMER_TIMESLIST_HPP
#define CUBE_TIMER_TIMESLIST_HPP

#include "rapidcsv.h"
#include "Duration.hpp"

#include <QTableWidget>
#include <filesystem>

namespace csv = rapidcsv;

class TimesList : public QTableWidget {
private:
	std::filesystem::path resourcePath;
	csv::Document resource;
public:
	TimesList(char* const& argv0, QWidget* parent);
	~TimesList() override;
	void addTime(Duration<long long int> const& toAdd);
};


#endif //CUBE_TIMER_TIMESLIST_HPP
