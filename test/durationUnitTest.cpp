//
// Created by Tudor Oancea on 13/04/2021.
//

#include <gtest/gtest.h>
#include "MainWindow.hpp"
#include "Duration.hpp"
#include <random>


TEST(DurationTest, Construction) {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<long> distrib(0);
	for (size_t i(0); i < 10000; ++i) {
		long x(distrib(gen));
		Duration<long> duration(x);
		ASSERT_EQ(x,duration.getMilliseconds()+duration.getSeconds()*1000+duration.getMinutes()*60000+duration.getHours()*3600000);
	}
}

TEST(DurationTest, StringConversion) {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<long> distrib(0);
	for (size_t i(0); i < 10000; ++i) {
		Duration<long> duration(distrib(gen));
		ASSERT_NO_THROW((void)duration.toString());
	}
}