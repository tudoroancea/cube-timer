//
// Created by Tudor Oancea on 13/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "MainWindow.hpp"
#include "Data.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"
#include "SettingsDialog.hpp"
#include "TimesList.hpp"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QEvent>
#include <QFileDialog>
#include <QLabel>
#include <QTimer>
#include <QtWidgets>
#include <iostream>
#include <utility>

namespace csv = rapidcsv;
namespace fs = std::filesystem;

Headers::Headers(std::string time, std::string const& mo3, std::string const& ao5, std::string const& ao12, std::string const& scramble, std::string const& timeStamp, std::string const& comment) : time(std::move(time)), mo3(mo3), ao5(ao5), ao12(ao12), scramble(scramble), timeStamp(timeStamp), comment(comment) {}
std::string Headers::operator[](size_t const& index) const {
    switch (index) {
        case 0:
            return time;
        case 1:
            return mo3;
        case 2:
            return ao5;
        case 3:
            return ao12;
        case 4:
            return scramble;
        case 5:
            return timeStamp;
        case 6:
            return comment;
        default:
            return "";
    }
}

size_t MainWindow::currentSession = 0;
const Headers MainWindow::metadataHeaders("time", "mo3", "ao5", "ao12", "scramble", "timeStamp", "comment");
//data and settings won't contain any pointer here. They will only if we use make_unique explicitely (in the main)
std::unique_ptr<Data> MainWindow::data = std::unique_ptr<Data>();
std::unique_ptr<Settings> MainWindow::settings = std::unique_ptr<Settings>();

MainWindow::MainWindow()
    : timer(new QTimer),
      launchingTimer(new QTimer),
      timeLabel(new QLabel("0.000", this)),
      scramble(),
      scrambleLabel(new QLabel(scramble.toQString(), this)),
      mainLayout(new QHBoxLayout),
      fileMenu(nullptr) {
    timesList = new TimesList();
    connect(timesList, SIGNAL(sendScramble(Scramble const&)), this, SLOT(tryScrambleAgain(Scramble const&)));
    pbWidget = new PBWidget();
    connect(timesList, SIGNAL(sendPBs(std::array<std::pair<Duration<long long>, size_t>, 4> const&)), pbWidget, SLOT(changePBS(std::array<std::pair<Duration<long long>, size_t>, 4> const&)));
    timesList->emitSendPBs();

    createTimers();
    createLabels();
    createActions();
    createMenus();
    createAboutMessage();

    auto* lhs(new QVBoxLayout);
    lhs->addWidget(pbWidget, 0);
    lhs->addWidget(timesList, 1);
    mainLayout->addLayout(lhs, 0);


    auto* rhs(new QVBoxLayout);
    rhs->addWidget(scrambleLabel, 2, Qt::AlignCenter);
    rhs->addWidget(timeLabel, 8, Qt::AlignCenter);


    //mainLayout->addWidget(timesList, 0);
    mainLayout->addLayout(rhs, 1);

    this->setCentralWidget(new QWidget);
    this->centralWidget()->setLayout(mainLayout);

    this->setWindowTitle("Cube Timer");
    this->setMinimumSize(700, 500);
    this->resize(700, 500);
    QList screens(QGuiApplication::screens());
    this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
    this->setUnifiedTitleAndToolBarOnMac(true);
#ifdef RELEASE_MODE
    this->statusBar()->addWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#else
    this->statusBar()->showMessage("Press Space bar to start timer");
#endif
    this->show();
    this->setFocus();
}

MainWindow::~MainWindow() {
	if (settings->getSetting("autoSave", QVariant(false), this).toBool()) {
		this->save();
	} else {
		if (!savedJustBefore) {
			QMessageBox::StandardButton reply(QMessageBox::question(this, "", "Do you want to save the current data ? ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes));
			if (reply == QMessageBox::Yes) {
				this->save();
            }
        }
    }
    delete timeLabel;
    delete scrambleLabel;
    delete timer;
    delete launchingTimer;
    //delete settings;
    delete fileMenu;
    delete mainLayout;
    for (auto const& action : actions) {
        delete action.second;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	if (!timer->isActive()) {
		// On le lance si on nous le demande
		if (!event->isAutoRepeat()) {
			if (event->key() == Qt::Key_Space || event->key() == Qt::Key_T) {
				this->makeTimeRed();
				launchingTimer->start(settings->getSetting("launchingInterval", QVariant(300), this).toInt());
				startingTimer = true;
				savedJustBefore = false;
			}
		}
		// On fait les autres trucs nécessaires
		if (event->key() == Qt::Key_N) {
			scramble.regenerate();
			scrambleLabel->setText(scramble.toQString());
			savedJustBefore = false;
		}
	} else {
		// We stop the timer
		timer->stop();
		// We measure the time elapsed since the beginning.
		std::chrono::time_point<std::chrono::high_resolution_clock> stopPoint(std::chrono::high_resolution_clock::now());
		std::chrono::milliseconds currentTime(std::chrono::duration_cast<std::chrono::milliseconds>(stopPoint - startPoint));
		Duration<long long> duration(currentTime.count());
		// We use the time to update the timeLabel
		timeLabel->setText(duration.toQString());
		// We use the time to update the timesList
		timesList->addTime(duration, scramble);
		// We regenerate the Scramble
		scramble.regenerate();
		scrambleLabel->setText(scramble.toQString());
		savedJustBefore = false;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
	QWidget::keyReleaseEvent(event);
	if (startingTimer && (event->key() == Qt::Key_Space || event->key() == Qt::Key_T)) {
		if (launchingTimer->isActive()) {
            QString message("One must wait ");
            message += Duration<uint>(settings->getSetting("launchingInterval", QVariant(0), this).toUInt()).toQString();
            message += "s before starting a timer.";
            this->statusBar()->showMessage(message, 1000);

            launchingTimer->stop();
        } else {
            timer->start();
            startPoint = std::chrono::high_resolution_clock::now();
        }
        startingTimer = false;  // In the first case, we were starting the Timer but we must abort, in the second case we just finished successfully. In both cases we are not starting the timer anymore.
        this->resetTimeColor();  // In the first case the label is still red, in the second case it's green. In both cases it must be reset to black.
    }
}
void MainWindow::changeDisplayedTime() {
    std::chrono::time_point<std::chrono::high_resolution_clock> now(std::chrono::high_resolution_clock::now());
    std::chrono::milliseconds currentTime(std::chrono::duration_cast<std::chrono::milliseconds>(now - startPoint));
    timeLabel->setText(Duration<long long int>(currentTime.count()).toQString());
}

void MainWindow::makeTimeRed() const {
	timeLabel->setStyleSheet("QLabel { color : rgb(240,94,135); }");
}
void MainWindow::makeTimeGreen() const {
	timeLabel->setStyleSheet("QLabel { color : rgb(123,216,143); }");
}
void MainWindow::resetTimeColor() const {
	timeLabel->setStyleSheet("");
}

void MainWindow::createTimers() {
	timer->setTimerType(Qt::PreciseTimer);
	connect(timer, &QTimer::timeout, this, &MainWindow::changeDisplayedTime);

	launchingTimer->setTimerType(Qt::PreciseTimer);
	launchingTimer->setSingleShot(true);
	connect(launchingTimer, &QTimer::timeout, this, &MainWindow::makeTimeGreen);
}
void MainWindow::createLabels() {
    timeLabel->setAlignment(Qt::AlignCenter);
    QFont font(timeLabel->font());
    font.setFamily("Fira Code iScript");
    font.setPointSize(45);
    timeLabel->setFont(font);
    this->setCentralWidget(timeLabel);

    scrambleLabel->setAlignment(Qt::AlignCenter);
    scrambleLabel->move(0, 0);
    font = scrambleLabel->font();
    font.setPointSize(15);
    scrambleLabel->setFont(font);
}
void MainWindow::createActions() {
	actions["save"] = new QAction("Save", this);
	actions["save"]->setShortcut(QKeySequence::Save);
	connect(actions["save"], SIGNAL(triggered()), this, SLOT(save()));

	actions["saveAs"] = new QAction("Save As", this);
	actions["saveAs"]->setShortcut(QKeySequence::SaveAs);
	connect(actions["saveAs"], SIGNAL(triggered()), this, SLOT(saveAs()));

    actions["openPreferences"] = new QAction("Open Preferences", this);
    actions["openPreferences"]->setShortcut(QKeySequence::Preferences);
    connect(actions["openPreferences"], SIGNAL(triggered()), this, SLOT(openPreferences()));

    actions["aboutAct"] = new QAction(tr("&About"), this);
    actions["aboutAct"]->setStatusTip(tr("Show the application's About box"));
    connect(actions["aboutAct"], &QAction::triggered, this, &MainWindow::about);

    actions["aboutQtAct"] = new QAction(tr("About &Qt"), this);
    actions["aboutQtAct"]->setStatusTip(tr("Show the Qt library's About box"));
    connect(actions["aboutQtAct"], &QAction::triggered, qApp, &QApplication::aboutQt);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
}
void MainWindow::createMenus() {
	auto preferencesMenu = menuBar()->addMenu("Preferences");
	preferencesMenu->addAction(actions["openPreferences"]);
	actions["openPreferences"]->setMenuRole(QAction::PreferencesRole);

	fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction(actions["save"]);
	fileMenu->addAction(actions["saveAs"]);

	auto helpMenu = menuBar()->addMenu("Help");
	helpMenu->addAction(actions["aboutAct"]);
	helpMenu->addAction(actions["aboutQtAct"]);
}
void MainWindow::createAboutMessage() {
    aboutMessage += "Cube-timer Version ";
    aboutMessage += PROJECT_VERSION;
    aboutMessage += "\nCopyright © 2021 Tudor Oancea.\nLicensed under the MIT Licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)\n\n";
    aboutMessage += "This is a simple GUI project using Qt to create a cube timer suited to my personal needs.\n";
    aboutMessage += "Concretly this means:\n";
    aboutMessage += "- It is purely intended for macOS users (on other OS's the CMakeLists won't even create the main target cube-timer). The main reason for this is that it uses the .app bundle particularities such as the Resource directory to save the preferences and data files. If you find easy to  implement alternatives for other platforms I am open to suggestions and pull-requests.\n";
    aboutMessage += "- The available features will probably never be ground-breaking compared to other timers out there, and I won't develop features I don't find intend to use (even though they may be very popular in the community).\n";
    aboutMessage += "For example, I won't implement inspection times for 3x3 blind or scrambles for cubes larger that 3x3. I won't develop neither stackmat nor other physical timers integration, nor smart cubes integration.";
    aboutMessage += "\n\nThe source code can be found at https://github.com/tudoroancea/cube-timer.git";
}

void MainWindow::save() {
	MainWindow::data->Save();
	savedJustBefore = true;
}

void MainWindow::saveAs() {
	QString str(QFileDialog::getSaveFileName(this, "Save current data", "/Users/untitled.csv", "CSV files (*.csv)"));
	if (!str.isEmpty()) {
		MainWindow::data->Save(str.toStdString());
		savedJustBefore = true;
	}
}


void MainWindow::openPreferences() {
	SettingsDialog dialog(settings.get(), this);
	dialog.exec();
}

void MainWindow::about() {
	QMessageBox::about(this, tr("About Menu"), aboutMessage);
}

void MainWindow::tryScrambleAgain(Scramble const& toTry) {
	if (!timer->isActive() && !launchingTimer->isActive() && !startingTimer) {
		scramble = toTry;
		scrambleLabel->setText(scramble.toQString());
	}
}
