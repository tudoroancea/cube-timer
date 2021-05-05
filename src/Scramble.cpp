//
// Created by Tudor Oancea on 15/04/2021.
//

#include "Scramble.hpp"

#include <iostream>
#include <random>
#include <QString>

std::string toString(const Moves& move) {
	switch (move) {
		case R: return "R";
		case Rp: return "R'";
		case R2: return "R2";
		case U: return "U";
		case Up: return "U'";
		case U2: return "U2";
		case F: return "F";
		case Fp: return "F'";
		case F2: return "F2";
		case L: return "L";
		case L2: return "L2";
		case Lp: return "L'";
		case D: return "D";
		case Dp: return "D'";
		case D2: return "D2";
		case B: return "B";
		case Bp: return "B'";
		case B2: return "B2";
		default: return "";
	}
}
bool repeated(Moves const& lhs, Moves const& rhs) {
	switch (lhs) {
		case R:
		case Rp:
		case R2: {
			return (rhs == R || rhs == Rp || rhs == R2);
		}
		case U:
		case Up:
		case U2: {
			return (rhs == U || rhs == Up || rhs == U2);
		}
		case F:
		case Fp:
		case F2: {
			return (rhs == F || rhs == Fp || rhs == F2);
		}
		case L:
		case Lp:
		case L2: {
			return (rhs == L || rhs == Lp || rhs == L2);
		}
		case D:
		case Dp:
		case D2: {
			return (rhs == D || rhs == Dp || rhs == D2);
		}
		case B:
		case Bp:
		case B2: {
			return (rhs == B || rhs == Bp || rhs == B2);
		}
	}
}
bool indepdent(Moves const& lhs, Moves const& rhs) {
	switch (lhs) {
		case R:
		case Rp:
		case R2: {
			return (rhs == L || rhs == Lp || rhs == L2);
		}
		case U:
		case Up:
		case U2: {
			return (rhs == D || rhs == Dp || rhs == D2);
		}
		case F:
		case Fp:
		case F2: {
			return (rhs == B || rhs == Bp || rhs == B2);
		}
		case L:
		case Lp:
		case L2: {
			return (rhs == R || rhs == Rp || rhs == R2);
		}
		case D:
		case Dp:
		case D2: {
			return (rhs == U || rhs == Up || rhs == U2);
		}
		case B:
		case Bp:
		case B2: {
			return (rhs == F || rhs == Fp || rhs == F2);
		}
	}
}
Scramble::Scramble() {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<long> distrib(0,17);
	moves[0] = Moves(distrib(gen));
	for (size_t i(1); i < 20; ++i) {
		do {
			moves[i] = Moves(distrib(gen));
		} while (repeated(moves[i], moves[i-1]) ||
				 (i >= 2 && indepdent(moves[i], moves[i - 1]) && repeated(moves[i],moves[i - 2])));
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
	return result;
}

QString Scramble::toQString() const {
	return QString(toString().c_str());
}
