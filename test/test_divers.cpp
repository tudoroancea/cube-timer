//
// Created by Tudor Oancea on 13/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

#include "Duration.hpp"
#include "MainWindow.hpp"
#include "Scramble.hpp"
#include "Settings.hpp"
#include "TimesList.hpp"
#include "libs/rapidcsv.h"

#include <QTimer>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

namespace fs = std::filesystem;
namespace csv = rapidcsv;
using namespace std;


int main(int argc, char** argv) {
	csv::Document doc("/Users/tudoroancea/CLionProjects/cube-timer/cmake-build-debug/src/cube-timer.app/Contents/Resources/default-historic.csv",
					  csv::LabelParams(0, -1),
					  csv::SeparatorParams(),
					  csv::ConverterParams(true, 0.0, 0));
	std::cout << doc.GetColumnIdx(std::to_string(0) + MainWindow::metadataHeaders.time) << std::endl;
	std::cout << std::to_string(0) + MainWindow::metadataHeaders.time << std::endl;
	auto hdr(doc.GetColumnNames());
	std::copy(hdr.begin(), hdr.end(), std::ostream_iterator<std::string>(std::cout, " ; "));
	doc.SetCell<std::string>(doc.GetColumnIdx(std::to_string(0) + MainWindow::metadataHeaders.time), 0, "");
	doc.Save();
	return 0;
}
#pragma clang diagnostic pop