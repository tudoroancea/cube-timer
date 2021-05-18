//
// Created by Tudor Oancea on 18/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "TimesList.hpp"
#include "libs/rapidcsv.h"

int main(int argc, char** argv) {
	if (argc >= 2) {
		csv::Document doc;
		bool works = true;
		try {
			doc.Load(argv[1],
			         csv::LabelParams(0,-1),
			         csv::SeparatorParams(),
			         csv::ConverterParams(true, 0.0, 0));
		} catch (...) {
			works = false;
			std::cerr << "The document does not exist" << std::endl;
		}
		if (works) {
			TimesList::recomputeStatistics(doc);
		}
	} else {
		std::cerr << "no argument" << std::endl;
	}
    return 0;
}