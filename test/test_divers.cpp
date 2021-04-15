//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>

int main() {
	std::vector<Duration<int>> tab{999,1000,601000,572091,0,12,98,122,60122};
	for (auto const& num : tab) {
		std::cout << num.toString().data() << std::endl;
	}
    return 0;
}