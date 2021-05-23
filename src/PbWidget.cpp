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

PBWidget::PBWidget() {
	auto pbLine(new QHBoxLayout);
	auto mainLayout(new QVBoxLayout);
}

void PBWidget::changePBS(Duration<long long int> const& pb, Duration<long long int> const& pbMO3, Duration<long long int> const& pbAO5, Duration<long long int> const& pbAO12) {

}

