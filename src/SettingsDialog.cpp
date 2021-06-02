//
// Created by Tudor Oancea on 12/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "SettingsDialog.hpp"

#include <QBoxLayout>
#include <QCheckBox>
#include <QGuiApplication>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QToolBox>
#include <QVariant>


SettingsDialog::SettingsDialog(Settings* settings, QWidget* parent)
    : settings(settings), QDialog(parent) {
    QVBoxLayout* verticalLayout;
    verticalLayout = new QVBoxLayout(this);
    auto* toolbox = new QToolBox;
    verticalLayout->addWidget(toolbox);

    // Page 1 ====================================================================================
    auto page1 = new QWidget;
    auto layout1 = new QGridLayout;
    page1->setLayout(layout1);
    // Launching interval ========================
    auto launchingIntervalLabel = new QLabel("Launching Interval (in milliseconds)");
    //launchingIntervalLabel->setFrameStyle(QFrame::Raised|QFrame::Panel);
    layout1->addWidget(launchingIntervalLabel, 0, 0);
    auto input(new QLineEdit);
    input->setText(this->settings->getSetting("launchingInterval", QVariant(""), this).toString());
    input->setValidator(new QIntValidator(0, 5000));
    QObject::connect(input, &QLineEdit::textChanged, [this](QString const& value) { this->settings->setSetting("launchingInterval", QVariant(value.toInt()), this); });
    layout1->addWidget(input, 0, 1);


    // Page 2 ====================================================================================
    auto page2 = new QWidget;
    auto layout2 = new QVBoxLayout;
    page2->setLayout(layout2);
    // Auto Save ========================
    auto checkBox = new QCheckBox("Auto Save");
    checkBox->setToolTip("If unselected, the app will ask you whether you want to save your new data or not upon application quit.");
    checkBox->setChecked(this->settings->getSetting("autoSave", QVariant(false), this).toBool());
    QObject::connect(checkBox, &QCheckBox::toggled, [this](bool value) { this->settings->setSetting("autoSave", value, this); });
    layout2->addWidget(checkBox);

    // Adding the pages ====================================================================================
    toolbox->addItem(page2, tr("General"));
    toolbox->addItem(page1, tr("Timer"));

    this->resize(350, 200);
    setWindowTitle(QGuiApplication::applicationDisplayName());
}