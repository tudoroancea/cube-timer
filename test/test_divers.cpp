//
// Created by Tudor Oancea on 13/04/2021.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

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
	std::cout << Duration<int>(0).toString() << std::endl;
	std::cout << std::boolalpha << (typeid(long long int)==typeid(long long)) << std::endl;
    return 0;
}
#pragma clang diagnostic pop