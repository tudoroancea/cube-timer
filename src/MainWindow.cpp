//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"
#include "TimesList.hpp"
#include "rapidcsv.h"

#include <QApplication>
#include <QtWidgets>
#include <QEvent>
#include <QTimer>
#include <QLabel>
#include <QBoxLayout>
#include <iostream>
#include <filesystem>

MainWindow::MainWindow(char* const& argv0)
	: timer(new QTimer),
	  launchingTimer(new QTimer),
	  timeLabel(new QLabel("0.000")),
	  scrambleLabel(new QLabel(Scramble().toQString(), this)),
	  mainLayout(new QHBoxLayout),
	  timesList(new TimesList(argv0, this)),
	  exePath(argv0)
{

	this->setWindowTitle("Cube Timer");
	this->setMinimumSize(700,500);
	QList screens(QGuiApplication::screens());
	this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);
	this->statusBar()->showMessage("Press Space bar to start timer");

	createTimers();
	createLabels();

	auto* rhs(new QVBoxLayout);
	rhs->addWidget(scrambleLabel, 2, Qt::AlignCenter);
	rhs->addWidget(timeLabel, 8, Qt::AlignCenter);

	mainLayout->addWidget(timesList, 0);
	mainLayout->addLayout(rhs, 1);

	this->setCentralWidget(new QWidget);
	this->centralWidget()->setLayout(mainLayout);

	this->show();
	this->setFocus();
}

MainWindow::~MainWindow() {
	delete timeLabel;
	delete scrambleLabel;
	delete timer;
	delete launchingTimer;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	if (timer->isActive()) {
		std::chrono::time_point<std::chrono::high_resolution_clock> now(std::chrono::high_resolution_clock::now());
		timer->stop();
		stoppedChronoJustBefore = true;
		scrambleLabel->setText(Scramble().toQString());
		std::chrono::milliseconds currentTime(std::chrono::duration_cast<std::chrono::milliseconds>(now-startPoint));
		Duration<long long int> duration(currentTime.count());
		timesList->addTime(duration);
	} else {
		switch (event->key()) {
			case Qt::Key_N:
		        scrambleLabel->setText(Scramble().toQString());
			    break;
			case Qt::Key_Space: {
				if (!event->isAutoRepeat()) {
					this->makeTimeRed();
					launchingTimer->start(launchingInterval);
				}
			    break;
			}
			default:
				break;
		}
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
	QWidget::keyReleaseEvent(event);
	switch(event->key()) {
		case Qt::Key_Space: {
			if (!event->isAutoRepeat()) {
				if (!timer->isActive() && !stoppedChronoJustBefore) {
					if (launchingTimer->isActive()) {
						this->statusBar()->showMessage("One must wait 1s before starting a timer.", 1000);
						launchingTimer->stop();
					} else {
						timer->start();
						startPoint = std::chrono::high_resolution_clock::now();
					}
					this->resetTimeColor();
				}
				stoppedChronoJustBefore = false;
			}
			break;
		}
		default:
			break;
	}
}
void MainWindow::changeDisplayedTime() {
	std::chrono::time_point<std::chrono::high_resolution_clock> now(std::chrono::high_resolution_clock::now());
	std::chrono::milliseconds currentTime(std::chrono::duration_cast<std::chrono::milliseconds>(now-startPoint));
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
	scrambleLabel->move(0,0);
	font = scrambleLabel->font();
	font.setPointSize(15);
	scrambleLabel->setFont(font);
}
