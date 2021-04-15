//
// Created by Tudor Oancea on 15/04/2021.
//

#include "Scramble.hpp"
#include <random>

std::string toString(const Moves& move) {
	switch (move) {
		case R: return "R";
		case Rp: return "R'";
		case U: return "U";
		case Up: return "U'";
		case F: return "F";
		case Fp: return "F'";
		case L: return "L";
		case Lp: return "L'";
		case D: return "D";
		case Dp: return "D'";
		case B: return "B";
		case Bp: return "B'";
		case R2: return "R2";
		case U2: return "U2";
		case F2: return "F2";
		case L2: return "L2";
		case D2: return "D2";
		case B2: return "B2";
		default: return "";
	}
}

Scramble::Scramble() {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<long> distrib(0,19);
	for (size_t i(0); i < 20; ++i) {
		moves[i] = Moves(distrib(gen));
	}
}

Moves& Scramble::operator[](const size_t& i) {
	return moves[i];
}

std::string Scramble::toString() const {
	std::string result;
	for (const auto & move : moves) {
	    result += ::toString(move)+" ";
	}
//	result.erase(result.size());
	return result;
}

