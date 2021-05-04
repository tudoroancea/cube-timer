//
// Created by Tudor Oancea on 13/04/2021.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"
#include "libs/rapidcsv.h"

#include <iostream>
#include <QTimer>
#include <iomanip>
#include <chrono>
#include <vector>
#include <filesystem>
#include <limits>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	fs::current_path("/Users/tudoroancea/ClionProjects/cube-timer/test");
	rapidcsv::Document doc;
	doc.SetColumnName(0,"a");
	doc.SetColumnName(1,"b");
	doc.SetColumnName(2,"c");
	doc.SetCell<std::string>(0,0,"0a");
	doc.SetCell<std::string>(1,0,"0b");
	doc.SetCell<std::string>(2,0,"0c");
	doc.SetCell<std::string>(0,1,"1a");
	doc.SetCell<std::string>(1,1,"1b");
	doc.SetCell<std::string>(2,1,"1c");
	//doc.Save("test2.csv");
	doc.Load("test.csv");
	std::cout << doc.GetCell<std::string>(0,0) << std::endl;
	std::cout << doc.GetCell<std::string>(0,1) << std::endl;
	doc.InsertColumn(3, std::vector<std::string>(), "column");
	//doc.SetColumnName(3,"column");
	std::cout << doc.GetCell<std::string>(3,1) << std::endl;
	doc.Save();
    return 0;
}
#pragma clang diagnostic pop