//
// Created by Tudor Oancea on 02/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "TimesList.hpp"
#include "libs/rapidcsv.h"
#include "Duration.hpp"

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
	pbs.fill(Duration<long long>());
	this->clearContents();
	size_t N(resource.GetRowCount());
	this->setRowCount(N);
	bool anyMetadataMissing = false;
	Duration<long long> readTime, readMO3, readAO5, readAO12;
	for (int i(0); i < N; ++i) {
		bool lineLacksMetadata = false;
		try {
			lineLacksMetadata = (resource.GetCell<std::string>(resource.GetColumnIdx("scramble"), i).empty() || resource.GetCell<std::string>(resource.GetColumnIdx("timeStamp"), i).empty());
		} catch (...) {
			std::cerr << "there was a problem" << std::endl;
		}
		if (lineLacksMetadata) {
			anyMetadataMissing = true;
		}
		this->setVerticalHeaderItem(i,new QTableWidgetItem(QString(std::to_string(N-i).c_str())));

		auto createItem = [lineLacksMetadata](Duration<long long> time)->QTableWidgetItem* {
			QTableWidgetItem* newItem;
			if (time > 0) {
				newItem = new QTableWidgetItem(time.toQString());
			} else {
				newItem = new QTableWidgetItem(QString());
			}
			if (lineLacksMetadata) {
				newItem->setBackground(QBrush(QColor(240,77,113,100)));
			}
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			return newItem;
		};

		readTime = resource.GetCell<long long>("time",i);
		this->setItem(N-i-1,0,createItem(readTime));
		if (readTime < pbs[0]) {
			pbs[0] = readTime;
		}
		readMO3 = resource.GetCell<long long>("mo3",i);
		this->setItem(N-i-1,1,createItem(readMO3));
		if (readMO3 < pbs[1]) {
			pbs[1] = readMO3;
		}
		readAO5 = resource.GetCell<long long>("ao5",i);
		this->setItem(N-i-1,2,createItem(readAO5));
		if (readAO5 < pbs[2]) {
			pbs[2] = readAO5;
		}
		readAO12 = resource.GetCell<long long>("ao12",i);
		this->setItem(N-i-1,3,createItem(readAO12));
		if (readAO12 < pbs[3]) {
			pbs[3] = readAO12;
		}
	}
	this->resizeColumnsToContents();
	this->resizeRowsToContents();
	emit sendPBs(pbs[0], pbs[1], pbs[2], pbs[3]);
	if (anyMetadataMissing) {
		throw Error(missingMetadata);
	}
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

	try {
		this->loadDefaultCSV();
	} catch (TimesList::Error const& err) {
		switch (err.type()) {
		    case TimesList::wrongPath: {
			    QMessageBox::critical(this, "", "The default CSV has not been found. The app cannot start.");
			    QCoreApplication::exit(1);
			    std::exit(1);
			    break;
		    }
	        case TimesList::wrongFormat: {
		        QMessageBox::critical(this, "",
		                              "The default CSV has been corrupted and has no longer the right format. The app cannot start.");
		        QCoreApplication::exit(1);
		        std::exit(1);
		        break;
	        }
		    default:
		        break;
		}
	}
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
		if (toAdd < pbs[0]) {
			pbs[0] = toAdd;
		}
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
			if (mo3 < pbs[1]) {
				pbs[1] = mo3;
			}
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
			if (ao5 < pbs[2]) {
				pbs[2] = ao5;
			}
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
			if (ao12 < pbs[3]) {
				pbs[3] = ao12;
			}
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

[[maybe_unused]] void TimesList::recomputeStatistics(csv::Document& csvToRecompute) {
	for (size_t i(0); i < csvToRecompute.GetRowCount(); ++ i) {
		if (i >= 2) {
			long long mo3(0);
			for (size_t j(i - 2); j <= i; ++j) {
				mo3 += csvToRecompute.GetCell<long long>("time", j);
			}
			mo3 /= 3;
			csvToRecompute.SetCell<long long>(csvToRecompute.GetColumnIdx("mo3"), i, mo3);
		} else {
			csvToRecompute.SetCell<std::string>(csvToRecompute.GetColumnIdx("mo3"), i, "");
		}
		if (i >= 4) {
			long long ao5(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t j(i-4); j <= i; ++j) {
				readValue = csvToRecompute.GetCell<long long>("time", j);
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
			csvToRecompute.SetCell<long long>(csvToRecompute.GetColumnIdx("ao5"),i, ao5);
		} else {
			csvToRecompute.SetCell<std::string>(csvToRecompute.GetColumnIdx("ao5"), i, "");
		}
		if (i >= 11) {
			long long ao12(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t j(i-11); j <= i; ++j) {
				readValue = csvToRecompute.GetCell<long long>("time", j);
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
			csvToRecompute.SetCell<long long>(csvToRecompute.GetColumnIdx("ao12"), i, ao12);
		} else {
			csvToRecompute.SetCell<std::string>(csvToRecompute.GetColumnIdx("ao12"), i, "");
		}
	}
	csvToRecompute.Save();
}

[[maybe_unused]] void TimesList::completeColumns(const std::string& pathToCSV) {
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
		throw Error(wrongPath);
	}
	if (hasRightFormat(defaultPath.string())) {
		resource = newResource;
		try { // necessary because we want to show this warning only when we load a CSV file (for the first time), not every time we read one's content (for example when we delete times).
			this->readCurrentCSV();
		} catch (Error const& err) {
			if (err.type() == missingMetadata) {
				QMessageBox::warning(this, "", "Some times did not have scrambles or time stamps. The corresponding lines are highlighted in red.");
			} else {
				#ifdef DEBUG_MODE
				std::cerr << "Unexpected error raised by TimesList::readCurrentCSV() called in TimesList::loadDefaultCSV()" << std::endl;
				#endif
			}
		}

	} else {
		#ifdef DEBUG_MODE
		std::cerr << "Default CVS has been corrupted. The new data has not been loaded." << std::endl; // Error : corruptedDefaultCSV OR wrongFormat
		#endif
		throw Error(wrongFormat);
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
		throw Error(wrongPath);
	}
	if (hasRightFormat(pathToCSV)) {
		resource = newResource;
		try { // necessary because we want to show this warning only when we load a CSV file (for the first time), not every time we read one's content (for example when we delete times).
			this->readCurrentCSV();
		} catch (Error const& err) {
			if (err.type() == missingMetadata) {
				QMessageBox::warning(this, "", "Some times did not have scrambles or time stamps. The corresponding lines are highlighted in red.");
			} else {
				#ifdef DEBUG_MODE
				std::cerr << "Unexpected error raised by TimesList::readCurrentCSV() called in TimesList::loadCustomCSV()" << std::endl;
				#endif
			}
		}
	} else {
		#ifdef DEBUG_MODE
		std::cerr << "Custom CVS does not have the right format (not 4 columns or not the right headers). The new data has not been loaded." << std::endl;
		#endif
		throw Error(wrongFormat);
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
		menu.addAction("Copy Scramble", [this, row](){
			QGuiApplication::clipboard()->setText(QString::fromStdString(resource.GetCell<std::string>("scramble", resource.GetRowCount()-1-row)));
		});
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
		for (auto const & row : rows) {
			this->setRangeSelected(QTableWidgetSelectionRange(row,0,row,3), true);
		}
		menu.addAction("Copy Scrambles", [this, rows](){
			QString scrambles;
			for (auto const & row : rows) {
				scrambles += QString::fromStdString(resource.GetCell<std::string>("scramble", resource.GetRowCount()-1-row))+"; ";
			}
			scrambles.remove(scrambles.size()-3,3);
			QGuiApplication::clipboard()->setText(scrambles);
		});
		menu.addAction("Delete time", [this, rows](){
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
				for (auto const & row : rows) {
					this->deleteTime(row);
				}
			}
		});
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
	TimesList::recomputeStatistics(resource);
	try {
		this->readCurrentCSV();
	} catch (...) {}
}

void TimesList::tryScrambleAgain(int row) {
	Scramble s;
	try {
		s = Scramble(resource.GetCell<std::string>("scramble",resource.GetRowCount()-1-row));
	} catch (Scramble::Error const&) {
		s.regenerate();
		QMessageBox::warning(this, "", "This time did not have a scramble saved. Another one was generated.");
	}
	emit sendScramble(s);
}
void TimesList::copyScramble(int row) {
	QGuiApplication::clipboard()->setText(QString::fromStdString(resource.GetCell<std::string>("scramble", resource.GetRowCount()-1-row)));
}

#pragma clang diagnostic pop