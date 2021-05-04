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
	this->setHorizontalHeaderLabels(QStringList({"time", "ao5", "ao12"}));
	this->setWordWrap(false);
	this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	this->setFocusPolicy(Qt::NoFocus);

	resourcePath = fs::path(argv0);
	resourcePath /= "../../Resources/test.csv";
	resourcePath = fs::canonical(resourcePath);
	try {
		resource.Load(resourcePath.string(),
				      csv::LabelParams(0,-1),
				      csv::SeparatorParams(),
				      csv::ConverterParams(true, 0.0, 0));
	}
	catch (...) {
		std::cerr << "Document not opened" << std::endl;
	}
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
		// On calcule les ao5 et ao12
		if (oldRowCountCSV >= 4) {
			long long ao5(0);
			for (size_t i(oldRowCountCSV); i > oldRowCountCSV - 5; --i) {
				ao5 += resource.GetCell<long long>("time", i);
			}
			ao5 /= 5;
			resource.SetCell<long long>(1,oldRowCountCSV, ao5);
			newItem = new QTableWidgetItem(Duration<long long>(ao5).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		this->setItem(oldRowCountTable, 1, newItem);
		if (oldRowCountCSV >= 11) {
			long long ao12(0);
			for (size_t i(oldRowCountCSV); i > oldRowCountCSV - 12; --i) {
				ao12 += resource.GetCell<long long>("time", i);
			}
			ao12 /= 12;
			resource.SetCell<long long>(2, oldRowCountCSV, ao12);
			newItem = new QTableWidgetItem(Duration<long long>(ao12).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		this->setItem(oldRowCountTable, 2, newItem);
	} else {
		std::cerr << "CSV file and table don't have the same number of rows" << std::endl;
	}

	//
	//size_t oldRowCount(this->rowCount());
	////try {
	////	resource.SetCell(0, oldRowCount, (long long)toAdd);
	////	std::cout << resource.GetCell<long long>(0,oldRowCount) << std::endl;
	////} catch (rapidcsv::no_converter const&) {
	////	std::cerr << "PAS BIEN" << std::endl;
	////} catch (...) {
	////	std::cerr << "pas bien" << std::endl;
	////}
	//this->insertRow(this->rowCount());
	//auto newItem(new QTableWidgetItem(  toAdd.toQString()));
	//newItem->setFlags(Qt::ItemIsEnabled);
	////std::cout << std::boolalpha << (this->item(oldRowCount, 0)==nullptr) << std::endl;
	////try {
	//this->setItem(this->rowCount()-1, 0, newItem);
	////}
	////catch (rapidcsv::no_converter const&) {
	////	std::cerr << "HEYO" << std::endl;
	////}
	//
	//this->resizeColumnsToContents();
	//this->resizeRowsToContents();
	//
	//if (oldRowCount >= 4) {
	//	long long int ao5(0);
	//	for (size_t i(oldRowCount-4); i <= oldRowCount; i++) {
	//		//ao5 += resource.GetCell<long long int>(0,i);
	//		std::string str(this->item(i,1)->text().toStdString());
	//		if (!str.empty()) ao5 += std::stoll(str);
	//	}
	//	ao5 /= 5; // moyenne en milliseconds
	//	//resource.SetCell(1, oldRowCount, ao5);
	//	//this->item(this->rowCount(),0)->setText(Duration<long long int>(ao5).toQString());
	//	newItem = new QTableWidgetItem(Duration<long long int>(ao5).toQString());
	//	newItem->setFlags(Qt::ItemIsEnabled);
	//	this->setItem(oldRowCount, 1, newItem);
	//} else {
	//	//resource.SetCell(1, oldRowCount, "");
	//	//this->item(this->rowCount(),1)->setText("");
	//	this->setItem(oldRowCount, 1, new QTableWidgetItem(QString()));
	//}
	//if (oldRowCount >= 11) {
	//	long long int ao12(0);
	//	for (size_t i(oldRowCount-11); i <= oldRowCount; i++) {
	//		//ao12 += resource.GetCell<long long int>(0,i);
	//		std::string str(this->item(i,2)->text().toStdString());
	//		if (!str.empty()) ao12 += std::stoll(str);
	//	}
	//	ao12 /= 12;
	//	//resource.SetCell(2, oldRowCount, ao12);
	//	newItem = new QTableWidgetItem(Duration<long long int>(ao12).toQString());
	//	newItem->setFlags(Qt::ItemIsEnabled);
	//	//this->item(this->rowCount(),0)->setText(Duration<long long int>(ao12).toQString());
	//	this->setItem(oldRowCount, 2, newItem);
	//} else {
	//	//resource.SetCell(2, oldRowCount, "");
	//	//this->item(this->rowCount(),2)->setText("");
	//	this->setItem(oldRowCount, 2, new QTableWidgetItem(QString()));
	//}
	//this->resizeColumnsToContents();
	//this->resizeRowsToContents();
}
