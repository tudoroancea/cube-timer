//
// Created by Tudor Oancea on 18/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_PBWIDGET_HPP
#define CUBE_TIMER_PBWIDGET_HPP

#include "Duration.hpp"
#include "TimesList.hpp"

#include <QGroupBox>
#include <QLabel>
#include <array>


class TimeLabel : public Time, public QLabel {
protected:
	void mouseDoubleClickEvent(QMouseEvent* event) override;

public:
	TimeLabel(QString const& str, QWidget* parent, size_t const& row, Type type);
	TimeLabel(long long const& val, QWidget* parent, size_t const& row, Type type);
	TimeLabel(TimeItem* item, QWidget* parent);
};

class PBWidget : public QGroupBox {
Q_OBJECT
private:
	std::array<TimeLabel*, 4> pbs;

public:
	PBWidget(QWidget* parent = nullptr);

public slots:
	void changePBS(std::array<std::pair<Duration<long long>,size_t>,4> const& vals);
};


#endif //CUBE_TIMER_PBWIDGET_HPP
