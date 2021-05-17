//
// Created by Tudor Oancea on 13/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"
#include "libs/rapidcsv.h"
#include "Settings.hpp"
#include "TimesList.hpp"

#include <iostream>
#include <QTimer>
#include <iomanip>
#include <chrono>
#include <vector>
#include <filesystem>
#include <limits>

namespace fs = std::filesystem;
namespace csv = rapidcsv;
using namespace std;


int main(int argc, char** argv) {
	//TimesList::recomputeStatistics("/Users/tudoroancea/CLionProjects/cube-timer/test/test.csv");
	//TimesList::completeColumns("/Users/tudoroancea/CLionProjects/cube-timer/test/test.csv");
	//TimesList::recomputeStatistics("/Users/tudoroancea/Desktop/save-2021-05-15-13-40.csv");
	//TimesList::completeColumns("/Users/tudoroancea/Desktop/save-2021-05-15-13-40.csv");
	return 0;
}
#pragma clang diagnostic pop