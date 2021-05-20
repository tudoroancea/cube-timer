//
// Created by Tudor Oancea on 18/05/2021.
// Copyright (c) 2021 Tudor Oancea. All rights reserved.
// Licensed under the MIT license (see at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_PBWIDGET_HPP
#define CUBE_TIMER_PBWIDGET_HPP

#include "Duration.hpp"

#include <QGroupBox>
#include <array>

QT_BEGIN_NAMESPACE
	//class QVBoxLayout;
	class QLabel;
QT_END_NAMESPACE

class PBWidget : public QGroupBox {
Q_OBJECT
private:
	//QVBoxLayout* layout;
	std::array<QLabel*, 4> pbs;

public:
	PBWidget();

public slots:
	//void changePBS(Duration<long long> const& pb, Duration<long long> const& pbMO3, Duration<long long> const& pbAO5, Duration<long long> const& pbAO12);
};


#endif //CUBE_TIMER_PBWIDGET_HPP
