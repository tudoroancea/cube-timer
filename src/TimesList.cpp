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
	this->setRowCount(resource.GetRowCount());
	long long readValue(0);
	for (int i(0); i < resource.GetRowCount(); ++i) {
		for (int j(0); j < resource.GetColumnCount(); ++j) {
			readValue = resource.GetCell<long long>(j, i);
			QTableWidgetItem* newItem;
			if (readValue > 0) {
				newItem = new QTableWidgetItem(Duration<long long>(readValue).toQString());
			} else {
				newItem = new QTableWidgetItem(QString());
			}
			newItem->setFlags(Qt::ItemIsEnabled);
			this->setItem(i,j,newItem);
			this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
			this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		}
	}
	this->resizeColumnsToContents();
	this->resizeRowsToContents();
}

TimesList::TimesList(char* const& argv0, QWidget* parent) : QTableWidget(0, 3, parent) {
	this->setHorizontalHeaderLabels(QStringList({"time", "ao5", "ao12"}));
	this->setWordWrap(false);
	this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	this->setFocusPolicy(Qt::NoFocus);

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
		this->setRowCount(oldRowCountTable+1);
		auto newItem(new QTableWidgetItem(toAdd.toQString()));
		this->setItem(oldRowCountTable,0,newItem);
		newItem->setFlags(Qt::ItemIsEnabled);
		// On calcule les ao5 et ao12
		if (oldRowCountCSV >= 4) {
			long long ao5(0);
			for (size_t i(oldRowCountCSV-4); i <= oldRowCountCSV; ++i) {
				ao5 += resource.GetCell<long long>("time", i);
			}
			ao5 /= 5;
			resource.SetCell<long long>(1,oldRowCountCSV, ao5);
			newItem = new QTableWidgetItem(Duration<long long>(ao5).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		this->setItem(oldRowCountTable, 1, newItem);
		newItem->setFlags(Qt::ItemIsEnabled);
		if (oldRowCountCSV >= 11) {
			long long ao12(0);
			for (size_t i(oldRowCountCSV-11); i <= oldRowCountCSV; ++i) {
				ao12 += resource.GetCell<long long>("time", i);
			}
			ao12 /= 12;
			resource.SetCell<long long>(2, oldRowCountCSV, ao12);
			newItem = new QTableWidgetItem(Duration<long long>(ao12).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		this->setItem(oldRowCountTable, 2, newItem);
		newItem->setFlags(Qt::ItemIsEnabled);
	} else {
		std::cerr << "CSV file and table don't have the same number of rows" << std::endl;
	}
	this->resizeRowsToContents();
	this->resizeColumnsToContents();
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

void TimesList::loadOtherCSV(const std::string& pathToCSV) {
	currentCSVIsDefault = false;
	fs::path newPath;
	//  open the file witth a QFileDialog
	try {
		resource.Load(newPath.string(),
		              csv::LabelParams(0,-1),
		              csv::SeparatorParams(),
		              csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		std::cerr << "Document not opened" << std::endl;
	}
	this->readCurrentCSV();
	this->readCurrentCSV();
}

void TimesList::loadOtherCSV(const std::filesystem::path& pathToCSV) {
	this->loadOtherCSV(fs::canonical(pathToCSV).string());
}

void TimesList::saveCurrentCSV() {
	resource.Save();
}

void TimesList::overrideDefaultCSVByCurrent() {
	resource.Save(defaultPath.string());
}

void TimesList::saveCurrentCSVTo(const std::string& pathToCSV) {
	resource.Save(pathToCSV);
}

void TimesList::saveCurrentCSVTo(const std::filesystem::path& pathToCSV) {
	resource.Save(fs::canonical(pathToCSV).string());
}

#pragma clang diagnostic pop