//
// Created by Tudor Oancea on 15/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "Scramble.hpp"

#include <QString>
#include <iostream>
#include <random>
#include <string_view>

std::string toString(const Moves& move) {
	switch (move) {
        case R:
            return "R";
        case Rp:
            return "R'";
        case R2:
            return "R2";
        case U:
            return "U";
        case Up:
            return "U'";
        case U2:
            return "U2";
        case F:
            return "F";
        case Fp:
            return "F'";
        case F2:
            return "F2";
        case L:
            return "L";
        case L2:
            return "L2";
        case Lp:
            return "L'";
        case D:
            return "D";
        case Dp:
            return "D'";
        case D2:
            return "D2";
        case B:
            return "B";
        case Bp:
            return "B'";
        case B2:
            return "B2";
        default:
            return "";
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
	this->regenerate();
}
Scramble::Scramble(std::string_view str) {
	if (str.empty()) {
		std::cerr << "The scramble is empty!" << std::endl;
		throw Scramble::Error(emptyInputString);
	} else {
		size_t pos(0), i(0);
		std::string move;
		while (pos != std::string::npos && i < 20) {
			pos = str.find(' ');
			move = str.substr(0, pos);
			if (move == "R") {
				moves[i] = R;
			} else if (move == "R2") {
				moves[i] = R2;
			} else if (move == "R'") {
				moves[i] = Rp;
			} else if (move == "L") {
				moves[i] = L;
			} else if (move == "L2") {
				moves[i] = L2;
			} else if (move == "L'") {
				moves[i] = Lp;
			} else if (move == "F") {
				moves[i] = F;
			} else if (move == "F2") {
				moves[i] = F2;
			} else if (move == "F'") {
				moves[i] = Fp;
			} else if (move == "D") {
				moves[i] = D;
			} else if (move == "D2") {
				moves[i] = D2;
			} else if (move == "D'") {
				moves[i] = Dp;
			} else if (move == "B") {
				moves[i] = B;
			} else if (move == "B2") {
				moves[i] = B2;
			} else if (move == "B'") {
				moves[i] = Bp;
			} else if (move == "U") {
				moves[i] = U;
            } else if (move == "U2") {
                moves[i] = U2;
            } else if (move == "U'") {
                moves[i] = Up;
            } else {
#ifdef DEBUG_MODE
                std::cerr << "pas bien" << std::endl;
#endif
            }
            ++i;
            str.remove_prefix(pos + 1);
        }
    }
}

void Scramble::regenerate() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib(0, 17);
    moves[0] = Moves(distrib(gen));
    for (size_t i(1); i < 20; ++i) {
        do {
            moves[i] = Moves(distrib(gen));
        } while (repeated(moves[i], moves[i - 1]) ||
                 (i >= 2 && indepdent(moves[i], moves[i - 1]) && repeated(moves[i], moves[i - 2])));
    }
}

Moves& Scramble::operator[](const size_t& i) {
	return moves[i];
}

std::string Scramble::toString() const {
    std::string result;
    for (const auto& move : moves) {
        result += ::toString(move) + " ";
    }
    return result;
}

QString Scramble::toQString() const {
	return QString::fromStdString(toString());
}

std::string Scramble::stringScramble() {
    std::string result;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib(0, 17);
    Moves move, oldMove, oldoldMove;
    oldMove = Moves(distrib(gen));
    result += ::toString(oldMove) + " ";
    for (size_t i(1); i < 20; ++i) {
        do {
            move = Moves(distrib(gen));
        } while (repeated(move, oldMove) ||
                 (i >= 2 && indepdent(move, oldMove) && repeated(move, oldoldMove)));
        result += ::toString(move) + " ";
        oldoldMove = oldMove;
        oldMove = move;
    }
    return result;
}

QString Scramble::qstringScramble() {
	return QString::fromStdString(Scramble::stringScramble());
}

bool Scramble::operator==(Scramble const& other) {
	for (size_t i(0); i < 20; ++i) {
		if (other.moves[i] != this->moves[i]) {
			return false;
		}
	}
	return true;
}
