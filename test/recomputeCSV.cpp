//
// Created by Tudor Oancea on 18/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "Data.hpp"
//#include "libs/rapidcsv.h"

int main(int argc, char** argv) {
	if (argc >= 3) {
		Data doc;
		bool works = true;
		try {
			doc.Load(argv[1]);
		} catch (...) {
			works = false;
			std::cerr << "The document does not exist" << std::endl;
		}
		if (works) {
			doc.recomputeStatistics(std::stoul(argv[2]));
		}
	} else {
		std::cerr << "Not enough arguments" << std::endl;
	}
    return 0;
}