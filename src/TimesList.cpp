//
// Created by Tudor Oancea on 02/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "TimesList.hpp"
#include "libs/rapidcsv.h"

#include <QTableWidget>
#include <QHeaderView>
#include <filesystem>

namespace csv = rapidcsv;
namespace fs = std::filesystem;

void TimesList::readCurrentCSV() {
	this->clearContents();
	size_t N(resource.GetRowCount());
	this->setRowCount(N);
	long long readValue(0);
	for (int i(0); i < resource.GetRowCount(); ++i) {
		this->setVerticalHeaderItem(i,new QTableWidgetItem(QString(std::to_string(N-i).c_str())));
		for (int j(0); j < resource.GetColumnCount(); ++j) {
			readValue = resource.GetCell<long long>(j, i);
			QTableWidgetItem* newItem;
			if (readValue > 0) {
				newItem = new QTableWidgetItem(Duration<long long>(readValue).toQString());
			} else {
				newItem = new QTableWidgetItem(QString());
			}
			newItem->setFlags(Qt::ItemIsEnabled);
			this->setItem(N-i-1,j,newItem);
		}
	}
	this->resizeColumnsToContents();
	this->resizeRowsToContents();
}

TimesList::TimesList(char* const& argv0, QWidget* parent) : QTableWidget(0, 4, parent) {
	this->setHorizontalHeaderLabels(QStringList({"time", "mo3", "ao5", "ao12"}));
	this->setWordWrap(false);
	this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	this->setFocusPolicy(Qt::NoFocus);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	defaultPath = fs::path(argv0);
	defaultPath /= "../../Resources/default-historic.csv";
	defaultPath = fs::canonical(defaultPath);

	this->loadDefaultCSV();
}

TimesList::~TimesList() {}

void TimesList::addTime(Duration<long long> const& toAdd) {
	size_t oldRowCountTable(this->rowCount()), oldRowCountCSV(resource.GetRowCount());
	if (oldRowCountTable == oldRowCountCSV) {
		// On ajoute le temps
		resource.SetCell<long long>(0, oldRowCountCSV, toAdd.toT());
		this->insertRow(0);
		auto newItem(new QTableWidgetItem(toAdd.toQString()));
		this->setItem(0,0,newItem);
		newItem->setFlags(Qt::ItemIsEnabled);
		// On calcule les mo3, ao5 et ao12
		if (oldRowCountCSV >= 2) {
			long long mo3(0);
			for (size_t i(oldRowCountCSV-2); i <= oldRowCountCSV; ++i) {
				mo3 += resource.GetCell<long long>("time", i);
			}
			mo3 /= 3;
			resource.SetCell<long long>(1, oldRowCountCSV, mo3);
			newItem = new QTableWidgetItem(Duration<long long>(mo3).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		newItem->setFlags(Qt::ItemIsEnabled);
		this->setItem(0, 1, newItem);

		if (oldRowCountCSV >= 4) {
			long long ao5(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t i(oldRowCountCSV-4); i <= oldRowCountCSV; ++i) {
				readValue = resource.GetCell<long long>("time", i);
				if (readValue < min) {
					min = readValue;
				}
				if (readValue > max) {
					max = readValue;
				}
				ao5 += readValue;
			}
			ao5 -= min;
			ao5 -= max;
			ao5 /= 3;
			resource.SetCell<long long>(2,oldRowCountCSV, ao5);
			newItem = new QTableWidgetItem(Duration<long long>(ao5).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		newItem->setFlags(Qt::ItemIsEnabled);
		this->setItem(0, 2, newItem);

		if (oldRowCountCSV >= 11) {
			long long ao12(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t i(oldRowCountCSV-11); i <= oldRowCountCSV; ++i) {
				readValue = resource.GetCell<long long>("time", i);
				if (readValue < min) {
					min = readValue;
				}
				if (readValue > max) {
					max = readValue;
				}
				ao12 += readValue;
			}
			ao12 -= min;
			ao12 -= max;
			ao12 /= 10;
			resource.SetCell<long long>(3, oldRowCountCSV, ao12);
			newItem = new QTableWidgetItem(Duration<long long>(ao12).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		newItem->setFlags(Qt::ItemIsEnabled);
		this->setItem(0, 3, newItem);

		//auto newVertLabel(new QTableWidgetItem(QString(std::to_string(oldRowCountTable+1).c_str())));
		//this->setVerticalHeaderItem(0,newVertLabel);
		this->setVerticalHeaderItem(0,new QTableWidgetItem(QString(std::to_string(oldRowCountTable+1).c_str())));
		this->resizeRowsToContents();
		this->resizeColumnsToContents();
	} else {
		std::cerr << "CSV file and table don't have the same number of rows" << std::endl;
	}
}

bool TimesList::isCurrentCSVDefault() const {
	return currentCSVIsDefault;
}

bool areEqual(std::vector<std::string> const& lhs, std::vector<std::string> const& rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	} else {
		for (size_t i(0); i < lhs.size(); ++i) {
			if (lhs[i] != rhs[i]) {
				return false;
			}
		}
	}
	return true;
}

bool TimesList::hasRightFormat(const std::string& pathToCSV) {
	csv::Document toVerify(pathToCSV);
	if (toVerify.GetColumnCount() != 4 || !areEqual(toVerify.GetColumnNames(),{"time","mo3","ao5","ao12"})) {
		return false;
	}
	return true;
}

void TimesList::loadDefaultCSV() {
	currentCSVIsDefault = true;
	csv::Document newResource;
	try {
		newResource.Load(defaultPath.string(),
					  csv::LabelParams(0,-1),
					  csv::SeparatorParams(),
					  csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		#ifdef DEBUG_MODE
		std::cerr << "Default CSV not found." << std::endl; // Error : wrongPath
		#endif
		throw wrongPath;
	}
	if (hasRightFormat(defaultPath.string())) {
		resource = newResource;
		this->readCurrentCSV();
	} else {
		#ifdef DEBUG_MODE
		std::cerr << "Default CVS has been corrupted. The new data has not been loaded." << std::endl; // Error : corruptedDefaultCSV OR wrongFormat
		#endif
		throw wrongFormat;
	}
}

void TimesList::loadCustomCSV(const std::string& pathToCSV) {
	currentCSVIsDefault = false;
	csv::Document newResource;
	try {
		newResource.Load(pathToCSV,
					  csv::LabelParams(0,-1),
					  csv::SeparatorParams(),
					  csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		#ifdef DEBUG_MODE
		std::cerr << "Default CSV not found." << std::endl; // Error : wrongPath
		#endif
		throw wrongPath;
	}
	if (hasRightFormat(pathToCSV)) {
		resource = newResource;
		this->readCurrentCSV();
	} else {
		#ifdef DEBUG_MODE
		std::cerr << "Custom CVS does not have the right format (not 4 columns or not the right headers). The new data has not been loaded." << std::endl;
		#endif
		throw wrongFormat;
	}
}

void TimesList::saveToCurrentCSV() {
	resource.Save();
}

void TimesList::saveToDefaultCSV() {
	resource.Save(defaultPath.string());
}

void TimesList::saveToCustomCSV(const std::string& pathToCSV) {
	resource.Save(pathToCSV);
}

#pragma clang diagnostic pop