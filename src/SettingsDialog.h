//
// Created by Tudor Oancea on 12/05/2021.
//

#ifndef CUBE_TIMER_SETTINGSDIALOG_HPP
#define CUBE_TIMER_SETTINGSDIALOG_HPP

#include "Settings.hpp"

#include <QWidget>
#include <QDialog>

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

#endif // CUBE_TIMER_SETTINGSDIALOG_HPP
