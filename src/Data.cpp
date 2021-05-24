//
// Created by Tudor Oancea on 23/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "Data.hpp"
#include "libs/rapidcsv.h"

#include <QMessageBox>
#include <QApplication>
namespace csv = rapidcsv;


Data::Data(std::string const& pathToCSV) : csv::Document(pathToCSV,csv::LabelParams(0,-1),csv::SeparatorParams(),csv::ConverterParams(true, 0.0, 0)) {}

void Data::Load(std::string const& pathToCSV) {
	try {
		this->csv::Document::Load(pathToCSV,
		                          csv::LabelParams(0,-1),
		                          csv::SeparatorParams(),
		                          csv::ConverterParams(true, 0.0, 0));
	} catch (...) {
#ifdef DEBUG_MODE
		std::cerr << "Default CSV not found." << std::endl; // Error : wrongPath
#endif
		QMessageBox::critical(nullptr, "", "The default CSV has not been found. The app crashed.");
		QCoreApplication::exit(1);
		std::exit(1);
	}
	if (!this->hasRightFormat()) {
#ifdef DEBUG_MODE
		std::cerr << "Default CVS has been corrupted. The new data has not been loaded." << std::endl; // Error : corruptedDefaultCSV OR wrongFormat
#endif
		QMessageBox::critical(nullptr, "","The default CSV has been corrupted and has no longer the right format. The app crashed.");
		QCoreApplication::exit(1);
		std::exit(1);
	}
}

bool Data::hasRightFormat() {
		size_t columnCount = this->GetColumnCount();
		if (columnCount % HeadersNumber != 0) {
			return false;
		}
		for (size_t i(0); i < columnCount; ++i) {
			if (this->GetColumnName(i) != std::to_string(i/HeadersNumber)+MainWindow::metadataHeaders[i%HeadersNumber]) { // NOLINT(cppcoreguidelines-narrowing-conversions)
				return false;
			}
		}
		return true;
}

size_t Data::sessionRowCount(size_t session) {
	size_t res(0);
	while (res < this->GetRowCount() && !this->getTime<std::string>(res, session).empty()) {
		++res;
	}
	return res;
}

void Data::recomputeStatistics(size_t const& session) {
	for (size_t i(0); i < this->sessionRowCount(session); ++ i) {
		if (i >= 2) {
			long long mo3(0);
			for (size_t j(i - 2); j <= i; ++j) {
				mo3 += this->getTime<long long>(j, session);
			}
			mo3 /= 3;
			this->setMO3<long long>(i, mo3, session);
		} else {
			this->setMO3<std::string>(i, "", session);
		}
		if (i >= 4) {
			long long ao5(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t j(i - 4); j <= i; ++j) {
				readValue = this->getTime<long long>(j, session);
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
			this->setAO5<long long>(i, ao5, session);
		} else {
			this->setAO5<std::string>(i, "", session);
		}
		if (i >= 11) {
			long long ao12(0);
			long long min(LLONG_MAX), max(0), readValue(0);
			for (size_t j(i - 11); j <= i; ++j) {
				readValue = this->getTime<long long>(j, session);
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
			this->setAO12<long long>(i, ao12, session);
		} else {
			this->setAO12<std::string>(i, "", session);
		}
	}
}