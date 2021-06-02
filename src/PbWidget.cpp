//
// Created by Tudor Oancea on 18/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "PbWidget.hpp"
#include <QBoxLayout>

TimeLabel::TimeLabel(QString const& str, QWidget* parent, size_t const& row, Time::Type type) : Time(row, type), QLabel(str, parent) {}
TimeLabel::TimeLabel(const long long& val, QWidget* parent, const size_t& row, Type type) : Time(row, type), QLabel(QString::number(val), parent) {}
TimeLabel::TimeLabel(TimeItem* item, QWidget* parent) : Time(item->getRowCSV(), item->getType()), QLabel(item->text(), parent) {}

void TimeLabel::mouseDoubleClickEvent(QMouseEvent* event) {
    this->showDoubleCLick();
}

PBWidget::PBWidget(QWidget* parent) : QGroupBox(parent) {
    this->setTitle("PBs (Personnal Bests)");
    this->setAlignment(Qt::AlignHCenter);

    auto timeLine(new QHBoxLayout);
    timeLine->addWidget(new QLabel("Time"), 0, Qt::AlignHCenter);
	pbs[0] = new TimeLabel("", nullptr, 0, Time::time);
	timeLine->addWidget(pbs[0], 0, Qt::AlignHCenter);
	auto mo3Line(new QHBoxLayout);
	mo3Line->addWidget(new QLabel("mo3"), 0, Qt::AlignHCenter);
	pbs[1] = new TimeLabel("", nullptr, 0, Time::mo3);
	mo3Line->addWidget(pbs[1], 0, Qt::AlignHCenter);
	auto ao5Line(new QHBoxLayout);
	ao5Line->addWidget(new QLabel("ao5"), 0, Qt::AlignHCenter);
	pbs[2] = new TimeLabel("", nullptr, 0, Time::ao5);
	ao5Line->addWidget(pbs[2], 0, Qt::AlignHCenter);
	auto ao12Line(new QHBoxLayout);
	ao12Line->addWidget(new QLabel("ao12"), 0, Qt::AlignHCenter);
	pbs[3] = new TimeLabel("", nullptr, 0, Time::ao12);
	ao12Line->addWidget(pbs[3], 0, Qt::AlignHCenter);

	auto mainLayout(new QVBoxLayout);
	mainLayout->addLayout(timeLine);
	mainLayout->addLayout(mo3Line);
	mainLayout->addLayout(ao5Line);
	mainLayout->addLayout(ao12Line);
	this->setLayout(mainLayout);
}

void PBWidget::changePBS(std::array<std::pair<Duration<long long int>, size_t>, 4> const& vals) {
	for (size_t i(0); i < 4; ++i) {
		if (vals[i].first.toT() != LLONG_MAX) {
			pbs[i]->setText(vals[i].first.toQString());
			pbs[i]->setRowCSV(vals[i].second);
		}
	}
}
