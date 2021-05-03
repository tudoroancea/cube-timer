//
// Created by Tudor Oancea on 02/05/2021.
//

#include "TimesList.hpp"
#include "rapidcsv.h"

#include <QTableWidget>
#include <QHeaderView>
#include <filesystem>

namespace csv = rapidcsv;
namespace fs = std::filesystem;

//static const QStringList headers({"time", "ao5", "ao12", "ao25", "ao50", "ao100", "ao200", "ao500"});

TimesList::TimesList(char* const& argv0, QWidget* parent) : QTableWidget(0, 3, parent) {
	std::cerr << argv0 << std::endl;
	fs::path path(fs::current_path());
	std::cerr << path.string() << std::endl;
	try {
		path /= "../Resources/test.csv";
		resource = csv::Document(path.string(),
		//resource = csv::Document(std::string(argv0).append("/../../Resources/test.csv"),
						         csv::LabelParams(0,-1),
						         csv::SeparatorParams(),
						         csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		std::cerr << "Document not opened" << std::endl;
	}
	//resource.Load("../../Resources/test.csv");
	this->setRowCount(resource.GetRowCount());

	long long int val(0);
	for (int i(0); i < resource.GetRowCount(); ++i) {
		for (int j(0); j < resource.GetColumnCount(); ++j) {
			val = resource.GetCell<long long int>(j,i);
			QTableWidgetItem* newItem;
			if (val > 0) {
				newItem = new QTableWidgetItem(Duration<long long int>(val).toQString());
			} else {
				newItem = new QTableWidgetItem(QString());
			}
			this->setItem(i,j,newItem);
			this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
			this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		}
	}
	this->setHorizontalHeaderLabels(QStringList({"time", "ao5", "ao12"}));
	this->setWordWrap(false);
	this->resizeColumnsToContents();
	this->resizeRowsToContents();
	this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

TimesList::~TimesList() {

}

void TimesList::addTime(Duration<long long int> const& toAdd) {
	size_t oldRowCount(resource.GetRowCount());
	try {
		resource.SetCell(0, oldRowCount, (long long int)toAdd);
	} catch (rapidcsv::no_converter const&) {
		std::cerr << "PAS BIEN" << std::endl;
	} catch (...) {
		std::cerr << "pas bien" << std::endl;
	}
	this->setRowCount(oldRowCount+1);
	auto newItem(new QTableWidgetItem(toAdd.toQString()));
	std::cout << std::boolalpha << (this->item(oldRowCount, 0)==nullptr) << std::endl;
	try {
		this->setItem(this->rowCount()-1, 0, newItem);
	}
	catch (rapidcsv::no_converter const&) {
		std::cerr << "HEYO" << std::endl;
	}


	if (oldRowCount >= 4) {
		long long int ao5(0);
		for (size_t i(oldRowCount-4); i <= oldRowCount; i++) {
			ao5 += resource.GetCell<long long int>(0,i);
		}
		ao5 /= 5; // moyenne en milliseconds
		resource.SetCell(1, oldRowCount, ao5);
		//this->item(this->rowCount(),0)->setText(Duration<long long int>(ao5).toQString());
		this->setItem(oldRowCount, 1, new QTableWidgetItem(Duration<long long int>(ao5).toQString()));
	} else {
		resource.SetCell(1, oldRowCount, "");
		//this->item(this->rowCount(),1)->setText("");
		this->setItem(oldRowCount, 1, new QTableWidgetItem(QString()));
	}
	if (oldRowCount >= 11) {
		long long int ao12(0);
		for (size_t i(oldRowCount-11); i <= oldRowCount; i++) {
			ao12 += resource.GetCell<long long int>(0,i);
		}
		ao12 /= 12;
		resource.SetCell(2, oldRowCount, ao12);
		//this->item(this->rowCount(),0)->setText(Duration<long long int>(ao12).toQString());
		this->setItem(oldRowCount, 2, new QTableWidgetItem(Duration<long long int>(ao12).toQString()));
	} else {
		resource.SetCell(2, oldRowCount, "");
		//this->item(this->rowCount(),2)->setText("");
		this->setItem(oldRowCount, 2, new QTableWidgetItem(QString()));
	}
}
