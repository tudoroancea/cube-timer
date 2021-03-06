//
// Created by Tudor Oancea on 15/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_SCRAMBLE_HPP
#define CUBE_TIMER_SCRAMBLE_HPP

#include <QString>
#include <array>
#include <random>


enum Moves { R,
             Rp,
             U,
             Up,
             F,
             Fp,
             L,
             Lp,
             D,
             Dp,
             B,
             Bp,
             R2,
             U2,
             F2,
             L2,
             D2,
             B2 };
std::string toString(Moves const&);

struct Scramble {
    enum ErrorType { emptyInputString };
    struct Error : public std::exception {
        ErrorType type_;
        explicit Error(ErrorType const& t) : type_(t) {}
        [[nodiscard]] ErrorType const& type() const { return type_; }
        [[nodiscard]] const char* what() const noexcept override {
            switch (type_) {
                case emptyInputString:
                    return "Empty input string";
                default:
                    return "Scramble::Error";
            }
        }
    };

    std::array<Moves, 20> moves{};
    Scramble();
    explicit Scramble(std::string_view str);
    void regenerate();
    Moves& operator[](size_t const&);
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] QString toQString() const;

    static std::string stringScramble();
    static QString qstringScramble();

    bool operator==(const Scramble& other);
};

#endif  //CUBE_TIMER_SCRAMBLE_HPP
