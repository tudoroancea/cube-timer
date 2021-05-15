//
// Created by Tudor Oancea on 15/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_SCRAMBLE_HPP
#define CUBE_TIMER_SCRAMBLE_HPP

#include <array>
#include <random>
#include <QString>


enum Moves{R,Rp,U,Up,F,Fp,L,Lp,D,Dp,B,Bp,R2,U2,F2,L2,D2,B2};
std::string toString(Moves const&);

struct Scramble{
	std::array<Moves,20> moves{};
	Scramble();
	Moves& operator[](size_t const&);
	[[nodiscard]] std::string toString() const;
	[[nodiscard]] QString toQString() const;
};

#endif //CUBE_TIMER_SCRAMBLE_HPP
