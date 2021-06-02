//
// Created by Tudor Oancea on 12/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_SETTINGSDIALOG_HPP
#define CUBE_TIMER_SETTINGSDIALOG_HPP

#include "Settings.hpp"

#include <QDialog>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class SettingsDialog : public QDialog {
	Q_OBJECT
private:
	Settings* settings;

public:
	explicit SettingsDialog(Settings* settings, QWidget* parent = nullptr);
};

#endif  // CUBE_TIMER_SETTINGSDIALOG_HPP
