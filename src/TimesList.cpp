//
// Created by Tudor Oancea on 02/05/2021.
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
	defaultPath /= "../../Resources/default.csv";
	defaultPath = fs::canonical(defaultPath);

	this->loadDefaultCSV();
}

TimesList::~TimesList() {
	resource.Save();
}

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

void TimesList::loadDefaultCSV() {
	currentCSVIsDefault = true;
	try {
		resource.Load(defaultPath.string(),
		              csv::LabelParams(0,-1),
		              csv::SeparatorParams(),
		              csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		std::cerr << "Document not opened" << std::endl;
	}
	this->readCurrentCSV();
}

void TimesList::loadCustomCSV(const std::string& pathToCSV) {
	currentCSVIsDefault = false;
	try {
		resource.Load(pathToCSV,
		              csv::LabelParams(0,-1),
		              csv::SeparatorParams(),
		              csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		std::cerr << "Document not opened" << std::endl;
	}
	this->readCurrentCSV();
}

void TimesList::loadCustomCSV(const std::filesystem::path& pathToCSV) {
	this->loadCustomCSV(fs::canonical(pathToCSV).string());
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

void TimesList::saveToCustomCSV(const std::filesystem::path& pathToCSV) {
	resource.Save(fs::canonical(pathToCSV).string());
}

#pragma clang diagnostic pop