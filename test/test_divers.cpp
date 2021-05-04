//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"

#include <iostream>
#include <QTimer>
#include <iomanip>
#include <chrono>
#include <vector>
#include <filesystem>
#include <limits>
namespace fs = std::filesystem;

int main(int argc, char** argv) {
//	// Get the last position of '/'
//	std::string aux(argv[0]);
//
//	// get '/' or '\\' depending on unix/mac or windows.
//#if defined(_WIN32) || defined(WIN32)
//	int pos = aux.rfind('\\');
//#else
//	int pos = aux.rfind('/');
//#endif
//
//	// Get the path and the name
//	std::string path = aux.substr(0,pos+1);
//	std::string name = aux.substr(pos+1);
//	// show results
//	std::cout << "Path: " << path << std::endl;
//	std::cout << "Name: " << name << std::endl;
//	fs::current_path("/")
	std::cout << Duration<int>(0).toString() << std::endl;
	std::cout << std::boolalpha << (typeid(long long int)==typeid(long long)) << std::endl;
	auto x = std::stoll("");
    return 0;
}