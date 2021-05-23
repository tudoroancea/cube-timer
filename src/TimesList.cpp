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
#include "MainWindow.hpp"
#include "Data.hpp"

#include <QTableWidget>
#include <QTableWidgetItem>
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

Time::Time(const size_t& row, Type type) : rowCSV(row), type(type) {}
void Time::setRowCSV(const size_t& row) {rowCSV = row;}

size_t const& Time::getRowCSV() const {
	return rowCSV;
};

Time::Type Time::getType() const {
	return type;
}

TimeItem::TimeItem(QString const& str, size_t const& row, Time::Type type) : Time (row, type), QTableWidgetItem(str, QTableWidgetItem::UserType) {
	this->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
}
TimeItem::TimeItem(long long const& val, size_t const& row, Time::Type type) : Time(row, type), QTableWidgetItem((val > 0 ? Duration<long long>(val).toQString() : QString()), QTableWidgetItem::UserType) {
	this->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
}

void Time::showDoubleCLick() {
	QString message;
	switch (type) {
		case time:{
			message += "Time : ";
			message += Duration<long long>(MainWindow::data.getTime<long long>(rowCSV)).toQString();
		    break;
		}
		case mo3:{
			message +="mean of 3 : ";
			message += Duration<long long>(MainWindow::data.getMO3<long long>(rowCSV)).toQString();
			message += "\n\n Time list:\n";
			for (size_t i(0); i < 3; ++i) {
				message += Duration<long long>(MainWindow::data.getTime<long long>(rowCSV-i)).toQString();
				message += "\n";
			}
			break;
		}
		case ao5:{
			message += "average of 5 : ";
			message += Duration<long long>(MainWindow::data.getAO5<long long>(rowCSV)).toQString();
			message += "\n\n Time list:\n";
			for (size_t i(0); i < 5; ++i) {
				message += Duration<long long>(MainWindow::data.getTime<long long>(rowCSV-i)).toQString();
				message += "\n";
			}
			break;
		}
		case ao12:{
			message += "average of 12 : ";
			message += Duration<long long>(MainWindow::data.getAO12<long long>(rowCSV)).toQString();
			message += "\n\n Time list:\n";
			for (size_t i(0); i < 12; ++i) {
				message += Duration<long long>(MainWindow::data.getTime<long long>(rowCSV-i)).toQString();
				message += "\n";
			}
			break;
		}
	}
	QMessageBox::information(nullptr, "hey", message);
}

void TimesList::readCurrentCSV() {
	//pbs.fill(Duration<long long>());
	this->clearContents();
	size_t N(0);
	while (!MainWindow::data.getTime<std::string>(N).empty() && N < MainWindow::data.GetRowCount()) {
		++N;
	}
	this->setRowCount(N);
	bool anyMetadataMissing = false;
	long long readValue;
	for (int i(0); i < N; ++i) {
		bool lineLacksMetadata = (MainWindow::data.getScramble<std::string>(i).empty() || MainWindow::data.getTimeStamp<std::string>(i).empty());
		if (lineLacksMetadata) {
			anyMetadataMissing = true;
		}
		this->setVerticalHeaderItem(i,new QTableWidgetItem(QString(std::to_string(N-i).c_str())));

		QTableWidgetItem* newItem;
		readValue = MainWindow::data.getTime<long long>(i);
		if (readValue > 0) {
			newItem = new TimeItem(readValue, i, Time::time);
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		if (lineLacksMetadata) {
			newItem->setBackground(QBrush(QColor(240,77,113,100)));
		}
		this->setItem(N-i-1,0, newItem);

		readValue = MainWindow::data.getMO3<long long>(i);
		if (readValue > 0) {
			newItem = new TimeItem(readValue, i, Time::mo3);
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		if (lineLacksMetadata) {
			newItem->setBackground(QBrush(QColor(240,77,113,100)));
		}
		this->setItem(N-i-1,1, newItem);

		readValue = MainWindow::data.getAO5<long long>(i);
		if (readValue > 0) {
			newItem = new TimeItem(readValue, i, Time::ao5);
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		if (lineLacksMetadata) {
			newItem->setBackground(QBrush(QColor(240,77,113,100)));
		}
		this->setItem(N-i-1,2, newItem);

		readValue = MainWindow::data.getAO12<long long>(i);
		if (readValue > 0) {
			newItem = new TimeItem(readValue, i, Time::ao12);
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		if (lineLacksMetadata) {
			newItem->setBackground(QBrush(QColor(240,77,113,100)));
		}
		this->setItem(N-i-1,3, newItem);
	}
	this->resizeColumnsToContents();
	this->resizeRowsToContents();
	connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(treatDoubleClick(QTableWidgetItem *)));
	if (anyMetadataMissing) {
		throw Error(missingMetadata);
	}
}

TimesList::TimesList(QWidget* parent) : QTableWidget(0, 4, parent) {
	this->setHorizontalHeaderLabels(QStringList({"time", "mo3", "ao5", "ao12"}));
	this->setWordWrap(false);
	this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	this->setFocusPolicy(Qt::NoFocus);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->setContextMenuPolicy(Qt::DefaultContextMenu);

	try { // necessary because we want to show this warning only when we load a CSV file (for the first time), not every time we read one's content (for example when we delete times).
		this->readCurrentCSV();
	} catch (Error const& err) {
		if (err.type() == missingMetadata) {
			QMessageBox::warning(this, "", "Some times did not have scrambles or time stamps. The corresponding lines are highlighted in red.");
		} else {
			#ifdef DEBUG_MODE
			std::cerr << "Unexpected error raised by TimesList::readCurrentCSV()" << std::endl;
			#endif
		}
	}
}

TimesList::~TimesList() {}

void TimesList::addTime(Duration<long long> const& toAdd, Scramble const& scramble, QDateTime const& timeStamp, QString const& comment) {
	size_t oldRowCountCSV(MainWindow::data.sessionRowCount());
	size_t oldRowCountTable(this->rowCount());
	if (oldRowCountTable == oldRowCountCSV) {
		// On ajoute le temps
		MainWindow::data.setTime(oldRowCountCSV, toAdd.toT());
		MainWindow::data.setScramble(oldRowCountCSV, scramble.toString());
		MainWindow::data.setTimeStamp(oldRowCountCSV, timeStamp.toString("yyyy-MM-dd-hh:mm:ss.zzz").toStdString());
		MainWindow::data.setComment(oldRowCountCSV, comment.toStdString());
		//if (toAdd < pbs[0]) {
		//	pbs[0] = toAdd;
		//}
		this->insertRow(0);
		QTableWidgetItem* newItem(new TimeItem(toAdd.toQString(), oldRowCountCSV, Time::time));
		this->setItem(0,0, newItem);

		// On calcule les mo3, ao5 et ao12
		if (oldRowCountCSV >= 2) {
			long long mo3(0);
			for (size_t i(oldRowCountCSV-2); i <= oldRowCountCSV; ++i) {
				mo3 += MainWindow::data.getTime<long long>(i);
			}
			mo3 /= 3;
			MainWindow::data.setMO3<long long>(oldRowCountCSV, mo3);
			newItem = new TimeItem(Duration<long long>(mo3).toQString(), oldRowCountCSV, Time::mo3);
			//if (mo3 < pbs[1]) {
			//	pbs[1] = mo3;
			//}
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		this->setItem(0, 1, newItem);

		if (oldRowCountCSV >= 4) {
			long long ao5(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t i(oldRowCountCSV-4); i <= oldRowCountCSV; ++i) {
				readValue = MainWindow::data.getTime<long long>(i);
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
			MainWindow::data.setAO5<long long>(oldRowCountCSV, ao5);
			newItem = new TimeItem(Duration<long long>(ao5).toQString(), oldRowCountCSV, Time::ao5);
			//if (ao5 < pbs[2]) {
			//	pbs[2] = ao5;
			//}
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		this->setItem(0, 2, newItem);

		if (oldRowCountCSV >= 11) {
			long long ao12(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t i(oldRowCountCSV-11); i <= oldRowCountCSV; ++i) {
				readValue = MainWindow::data.getTime<long long>(i);
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
			MainWindow::data.setAO12<long long>(oldRowCountCSV, ao12);
			newItem = new TimeItem(Duration<long long>(ao12).toQString(), oldRowCountCSV, Time::ao12);
			//if (ao12 < pbs[3]) {
			//	pbs[3] = ao12;
			//}
		} else {
			newItem = new QTableWidgetItem(QString());
			newItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
		this->setItem(0, 3, newItem);

		this->setVerticalHeaderItem(0,new QTableWidgetItem(QString(std::to_string(oldRowCountTable+1).c_str())));
		this->resizeRowsToContents();
		this->resizeColumnsToContents();
	} else {
		std::cerr << "CSV file and table don't have the same number of rows" << std::endl;
	}
}

size_t firstDifference(std::vector<std::string> const& lhs, std::vector<std::string> rhs) {
	size_t res(0);
	while (lhs[res] == rhs[res]) {
		++res;
	}
	return res;
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
		menu.addAction("Copy Scramble", [row](){
			QGuiApplication::clipboard()->setText(QString::fromStdString(MainWindow::data.getScramble<std::string>(MainWindow::data.GetRowCount()-1-row)));
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
		menu.addAction("Copy Scrambles", [rows](){
			QString scrambles;
			for (auto const & row : rows) {
				scrambles += QString::fromStdString(MainWindow::data.getScramble<std::string>(MainWindow::data.GetRowCount()-1-row))+"; ";
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
	size_t N(MainWindow::data.GetRowCount());
	QString input(QInputDialog::getText(this,
									 "Modify Comment",
									 "Enter comments",
									 QLineEdit::Normal,
									 QString::fromStdString(MainWindow::data.getComment<std::string>(N-1-row))));
	if (!input.isNull()) {
		MainWindow::data.setComment(N-1-row, input.toStdString());
		//MainWindow::data.Save();
	}
}

void TimesList::moreInfo(int row) {
	size_t N(MainWindow::data.GetRowCount());
	QString message("Time : ");
	long long readValue(MainWindow::data.getTime<long long>(N-1-row));
	if (readValue > 0) {
		message += Duration<long long>(readValue).toQString();
	}
	message += "\n mo3 : ";
	readValue = MainWindow::data.getMO3<long long>(N-1-row);
	if (readValue > 0) {
		message += Duration<long long>(readValue).toQString();
	}
	message += "\n ao5 : ";
	readValue = MainWindow::data.getAO5<long long>(N-1-row);
	if (readValue > 0) {
		message += Duration<long long>(readValue).toQString();
	}
	message += "\n ao12 : ";
	readValue = MainWindow::data.getAO12<long long>(N-1-row);
	if (readValue > 0) {
		message += Duration<long long>(readValue).toQString();
	}
	message += "\n Scramble : ";
	message += QString::fromStdString(MainWindow::data.getScramble<std::string>(N-1-row));
	message += "\n Time Stamp : ";
	message += QString::fromStdString(MainWindow::data.getTimeStamp<std::string>(N-1-row));
	message += "\n Comment : ";
	message += QString::fromStdString(MainWindow::data.getComment<std::string>(N-1-row));
	QMessageBox::information(this, QString("Information on Time ")+QString::fromStdString(std::to_string(N-row)), message);
}

void TimesList::deleteTime(int row) {
	size_t oldRowCount(MainWindow::data.sessionRowCount());
	size_t oldRank(oldRowCount-1-row);
	for (size_t i(oldRank); i < oldRowCount-1; ++i) {
		MainWindow::data.setTime(i, MainWindow::data.getTime<std::string>(i+1));
		MainWindow::data.setMO3(i, MainWindow::data.getMO3<std::string>(i+1));
		MainWindow::data.setAO5(i, MainWindow::data.getAO5<std::string>(i+1));
		MainWindow::data.setScramble(i, MainWindow::data.getScramble<std::string>(i+1));
		MainWindow::data.setTimeStamp(i, MainWindow::data.getTimeStamp<std::string>(i+1));
		MainWindow::data.setComment(i, MainWindow::data.getComment<std::string>(i+1));
	}
	MainWindow::data.setTime<std::string>(oldRowCount-1, "");
	MainWindow::data.setMO3<std::string>(oldRowCount-1, "");
	MainWindow::data.setAO5<std::string>(oldRowCount-1, "");
	MainWindow::data.setScramble<std::string>(oldRowCount-1, "");
	MainWindow::data.setTimeStamp<std::string>(oldRowCount-1, "");
	MainWindow::data.setComment<std::string>(oldRowCount-1, "");
	for (int i(row-1); i > 0; --i) {
		dynamic_cast<TimeItem*>(this->item(i, 0))->setRowCSV(oldRowCount-i);
	}
	this->removeRow(row);
	MainWindow::data.recomputeStatistics();
	try {
		this->readCurrentCSV();
	} catch (...) {
	//	The only error that may be thrown is TimesList::missingData, which does not matter at this point of the execution.
	}
}

void TimesList::tryScrambleAgain(int row) {
	Scramble s;
	try {
		s = Scramble(MainWindow::data.getScramble<std::string>(MainWindow::data.GetRowCount()-1-row));
	} catch (Scramble::Error const&) {
		s.regenerate();
		QMessageBox::warning(this, "", "This time did not have a scramble saved. Another one was generated.");
	}
	emit sendScramble(s);
}
void TimesList::copyScramble(int row) {
	QGuiApplication::clipboard()->setText(QString::fromStdString(MainWindow::data.getScramble<std::string>(MainWindow::data.GetRowCount()-1-row)));
}

void TimesList::treatDoubleClick(QTableWidgetItem* item) {
	TimeItem* ptr;
	if ((ptr = dynamic_cast<TimeItem*>(item))) {
		ptr->showDoubleCLick();
	}
}

#pragma clang diagnostic pop