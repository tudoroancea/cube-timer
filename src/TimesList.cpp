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
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QGuiApplication>
#include <QClipboard>

namespace csv = rapidcsv;
namespace fs = std::filesystem;

const std::vector<std::string> TimesList::metadataHeaders = {"time", "mo3", "ao5", "ao12", "scramble", "timeStamp", "comment"};

void TimesList::readCurrentCSV() {
	this->clearContents();
	size_t N(resource.GetRowCount());
	this->setRowCount(N);
	long long readValue(0);
	for (int i(0); i < N; ++i) {
		this->setVerticalHeaderItem(i,new QTableWidgetItem(QString(std::to_string(N-i).c_str())));
		for (int j(0); j < 4; ++j) {
			readValue = resource.GetCell<long long>(j, i);
			QTableWidgetItem* newItem;
			if (readValue > 0) {
				newItem = new QTableWidgetItem(Duration<long long>(readValue).toQString());
			} else {
				newItem = new QTableWidgetItem(QString());
			}
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
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

	this->setContextMenuPolicy(Qt::DefaultContextMenu);
}

TimesList::~TimesList() {}

void TimesList::addTime(Duration<long long int> const& toAdd, Scramble const& scramble, QDateTime const& timeStamp, QString const& comment) {
	size_t oldRowCountTable(this->rowCount()), oldRowCountCSV(resource.GetRowCount());
	if (oldRowCountTable == oldRowCountCSV) {
		// On ajoute le temps
		resource.SetCell<long long>(resource.GetColumnIdx("time"), oldRowCountCSV, toAdd.toT());
		resource.SetCell<std::string>(resource.GetColumnIdx("scramble"), oldRowCountCSV, scramble.toString());
		resource.SetCell<std::string>(resource.GetColumnIdx("timeStamp"), oldRowCountCSV, timeStamp.toString("yyyy-MM-dd-hh:mm:ss.zzz").toStdString());
		resource.SetCell<std::string>(resource.GetColumnIdx("comment"), oldRowCountCSV, comment.toStdString());

		this->insertRow(0);
		auto newItem(new QTableWidgetItem(toAdd.toQString()));
		this->setItem(0,0,newItem);
		newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		// On calcule les mo3, ao5 et ao12
		if (oldRowCountCSV >= 2) {
			long long mo3(0);
			for (size_t i(oldRowCountCSV-2); i <= oldRowCountCSV; ++i) {
				mo3 += resource.GetCell<long long>("time", i);
			}
			mo3 /= 3;
			resource.SetCell<long long>(resource.GetColumnIdx("mo3"), oldRowCountCSV, mo3);
			newItem = new QTableWidgetItem(Duration<long long>(mo3).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
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
			resource.SetCell<long long>(resource.GetColumnIdx("ao5"),oldRowCountCSV, ao5);
			newItem = new QTableWidgetItem(Duration<long long>(ao5).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
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
			resource.SetCell<long long>(resource.GetColumnIdx("ao12"), oldRowCountCSV, ao12);
			newItem = new QTableWidgetItem(Duration<long long>(ao12).toQString());
		} else {
			newItem = new QTableWidgetItem(QString());
		}
		newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		this->setItem(0, 3, newItem);

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
bool isStartOf(std::vector<std::string> const& lhs, std::vector<std::string> const& rhs) {
	if (lhs.size() > rhs.size()) {
		return false;
	} else {
		for (size_t i(0); i < lhs.size(); ++i) {
			if (lhs[i] != rhs[i]) {
				return false;
			}
		}
		return true;
	}
}
size_t firstDifference(std::vector<std::string> const& lhs, std::vector<std::string> rhs) {
	size_t res(0);
	while (lhs[res] == rhs[res]) {
		++res;
	}
	return res;
}
bool TimesList::hasRightFormat(const std::string& pathToCSV) {
	csv::Document toVerify(pathToCSV);
	if (!areEqual(toVerify.GetColumnNames(),metadataHeaders)) {
		return false;
	}
	return true;
}

void TimesList::recomputeStatistics(const std::string& pathToCSV) {
	csv::Document doc;
	bool works = true;
	try {
		doc.Load(pathToCSV,
                 csv::LabelParams(0,-1),
                 csv::SeparatorParams(),
                 csv::ConverterParams(true, 0.0, 0));
	} catch (...) {
		works = false;
	}
	if (works) {
		for (size_t i(0); i < doc.GetRowCount(); ++ i) {
			if (i >= 2) {
				long long mo3(0);
				for (size_t j(i - 2); j <= i; ++j) {
					mo3 += doc.GetCell<long long>("time", j);
				}
				mo3 /= 3;
				doc.SetCell<long long>(doc.GetColumnIdx("mo3"), i, mo3);
			} else {
				doc.SetCell<std::string>(doc.GetColumnIdx("mo3"), i, "");
			}
			if (i >= 4) {
				long long ao5(0);
				long long min(LLONG_MAX), max(0), readValue(0);
				for (size_t j(i-4); j <= i; ++j) {
					readValue = doc.GetCell<long long>("time", j);
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
				doc.SetCell<long long>(doc.GetColumnIdx("ao5"),i, ao5);
			} else {
				doc.SetCell<std::string>(doc.GetColumnIdx("ao5"), i, "");
			}
			if (i >= 11) {
				long long ao12(0);
				long long min(LLONG_MAX), max(0), readValue(0);
				for (size_t j(i-11); j <= i; ++j) {
					readValue = doc.GetCell<long long>("time", j);
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
				doc.SetCell<long long>(doc.GetColumnIdx("ao12"), i, ao12);
			} else {
				doc.SetCell<std::string>(doc.GetColumnIdx("ao12"), i, "");
			}
		}
		doc.Save();
	} else {
		std::cerr << "shyte the document cannot be open" << std::endl;
	}
}

void TimesList::completeColumns(const std::string& pathToCSV) {
	csv::Document toComplete(pathToCSV);
	if (isStartOf(toComplete.GetColumnNames(), metadataHeaders)) {
		for (size_t i(firstDifference(toComplete.GetColumnNames(),metadataHeaders)); i < metadataHeaders.size(); ++i) {
			toComplete.InsertColumn(toComplete.GetColumnCount(), std::vector<std::string>(toComplete.GetRowCount(),""));
			toComplete.SetColumnName(toComplete.GetColumnCount()-1, metadataHeaders[i]);
		}
		toComplete.Save();
	} else {
		std::cerr << "Ne commence pas pareil" << std::endl;
	}
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

void TimesList::print(int row, int col) {
	std::cout << row << "," << col << std::endl;
}

void TimesList::contextMenuEvent(QContextMenuEvent* event) {
	QAbstractScrollArea::contextMenuEvent(event);
	QMenu menu;
	QModelIndexList list(this->selectedIndexes());
	QSet<int> rows;
	for (auto const & index : list) {
	    rows.insert(index.row());
	}
	if (rows.size() == 1) {
		int row(list.front().row());
		this->setRangeSelected(QTableWidgetSelectionRange(row,0,row,3), true);
		menu.addAction("More Info", [this, row](){this->moreInfo(row);}, QKeySequence(Qt::Key_I));
		menu.addAction("Modify Comment", [this, row](){this->modifyComment(row);}, QKeySequence(Qt::Key_M));
		menu.addAction("Copy Scramble", [this, row](){ this->copyScramble(row);});
		menu.addAction("Try Scramble again", [this, row](){this->tryScrambleAgain(row);});
		menu.addAction("Delete time", [this, row](){
			auto result(QMessageBox::warning(this, "",
									"Are you sure you want to delete this time? This operation is irreversible.",
									QMessageBox::Cancel|QMessageBox::Yes,
#ifdef DEBUG_MODE
									QMessageBox::Yes
#else
									QMessageBox::Cancel
#endif
									));
			if (result == QMessageBox::Yes) {
				this->deleteTime(row);
			}
		});
	} else {
		std::cerr << "pas bien" << std::endl;
		menu.addAction("Error, several rows selected");
	}
	menu.exec(event->globalPos());
	this->setRangeSelected(QTableWidgetSelectionRange(0,0,this->rowCount(),this->columnCount()), false);

}

void TimesList::modifyComment(int row) {
	size_t N(resource.GetRowCount());
	QString input(QInputDialog::getText(this,
									 "Modify Comment",
									 "Enter comments",
									 QLineEdit::Normal,
									 QString::fromStdString(resource.GetCell<std::string>("comment",N-1-row))));
	if (!input.isNull()) {
		resource.SetCell(resource.GetColumnIdx("comment"), N-1-row, input.toStdString());
		resource.Save();
	}
}
void TimesList::moreInfo(int row) {
	size_t N(resource.GetRowCount());
	QString message("Time : ");
	message += QString::fromStdString(resource.GetCell<std::string>("time", N-1-row));
	message += "\n mo3 : ";
	message += QString::fromStdString(resource.GetCell<std::string>("mo3", N-1-row));
	message += "\n ao5 : ";
	message += QString::fromStdString(resource.GetCell<std::string>("ao5", N-1-row));
	message += "\n ao12 : ";
	message += QString::fromStdString(resource.GetCell<std::string>("ao12", N-1-row));
	message += "\n Scramble : ";
	message += QString::fromStdString(resource.GetCell<std::string>("scramble", N-1-row));
	message += "\n Time Stamp : ";
	message += QString::fromStdString(resource.GetCell<std::string>("timeStamp", N-1-row));
	message += "\n Comment : ";
	message += QString::fromStdString(resource.GetCell<std::string>("comment", N-1-row));
	QMessageBox::information(this, QString("Information on Time")+QString::fromStdString(std::to_string(N-row)), message);
}

void TimesList::deleteTime(int row) {
	size_t oldRowCount(resource.GetRowCount());
	size_t oldRank(oldRowCount-1-row);
	resource.RemoveRow(oldRank);
	this->removeRow(row);
	for (size_t i(0); i < resource.GetRowCount(); ++ i) {
		if (i >= 2) {
			long long mo3(0);
			for (size_t j(i - 2); j <= i; ++j) {
				mo3 += resource.GetCell<long long>("time", j);
			}
			mo3 /= 3;
			resource.SetCell<long long>(resource.GetColumnIdx("mo3"), i, mo3);
		} else {
			resource.SetCell<std::string>(resource.GetColumnIdx("mo3"), i, "");
		}
		if (i >= 4) {
			long long ao5(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t j(i-4); j <= i; ++j) {
				readValue = resource.GetCell<long long>("time", j);
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
			resource.SetCell<long long>(resource.GetColumnIdx("ao5"),i, ao5);
		} else {
			resource.SetCell<std::string>(resource.GetColumnIdx("ao5"), i, "");
		}
		if (i >= 11) {
			long long ao12(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t j(i-11); j <= i; ++j) {
				readValue = resource.GetCell<long long>("time", j);
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
			resource.SetCell<long long>(resource.GetColumnIdx("ao12"), i, ao12);
		}else {
			resource.SetCell<std::string>(resource.GetColumnIdx("ao12"), i, "");
		}
	}
	resource.Save();
	this->readCurrentCSV();
}
void TimesList::tryScrambleAgain(int row) {
	emit sendScramble(Scramble(resource.GetCell<std::string>("scramble",resource.GetRowCount()-1-row)));
}
void TimesList::copyScramble(int row) {
	QGuiApplication::clipboard()->setText(QString::fromStdString(resource.GetCell<std::string>("scramble", resource.GetRowCount()-1-row)));
}

#pragma clang diagnostic pop