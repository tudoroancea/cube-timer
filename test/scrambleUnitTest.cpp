//
// Created by Tudor Oancea on 20/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include <gtest/gtest.h>
#include "Scramble.hpp"


size_t number(std::string const& str) {
	size_t result(0);
	for (char const& i : str) {
		if (i == 'R' || i == 'U' || i == 'F' || i == 'L' || i == 'D' || i == 'B') ++result;
	}
	return result;
}

TEST(ScrambleTest, Size) {
	for (size_t k(0); k < 10000; ++k) {
		ASSERT_NO_THROW(Scramble s);
		Scramble s;
		size_t n(number(s.toString()));
		ASSERT_EQ(n, 20) << n << " au lieu de 20 (itération " << k << ")";
	}
}

TEST(ScrambleTest, MovesConstruction) {
	for (int k(0); k < 100; ++k) {
		ASSERT_NO_THROW(Moves m((Moves(k)))) << "k";
	}
}