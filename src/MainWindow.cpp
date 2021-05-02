//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"

#include <QApplication>
#include <QtWidgets>
#include <QEvent>
#include <QTimer>
#include <QLabel>
#include <iostream>
#include <filesystem>

MainWindow::MainWindow() : timeLabel(new QLabel("0.000")), scrambleLabel(new QLabel(Scramble().toQString(), this)), timer(new QTimer), launchingTimer(new QTimer) {

	this->setWindowTitle("Cube Timer");
	this->resize(500,500);
	QList screens(QGuiApplication::screens());
	this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);
	this->statusBar()->showMessage("Press Space bar to start timer");

	timer->setTimerType(Qt::PreciseTimer);
	connect(timer, &QTimer::timeout, this, &MainWindow::changeDisplayedTime);
	launchingTimer->setTimerType(Qt::PreciseTimer);
	launchingTimer->setSingleShot(true);
	connect(launchingTimer, &QTimer::timeout, this, &MainWindow::makeTimeGreen);

	timeLabel->setAlignment(Qt::AlignCenter);
	QFont font(timeLabel->font());
	font.setFamily("Fira Code iScript");
	font.setPointSize(45);
	timeLabel->setFont(font);
	this->setCentralWidget(timeLabel);

	scrambleLabel->setAlignment(Qt::AlignCenter);
	scrambleLabel->resize(500,100);
	scrambleLabel->move(0,0);
	font = scrambleLabel->font();
	font.setPointSize(15);
	scrambleLabel->setFont(font);

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
		timer->stop();
		stoppedChronoJustBefore = true;
		scrambleLabel->setText(Scramble().toQString());
	} else {
		switch (event->key()) {
			//case Qt::Key_T: {
			//    	auto filename = QFileDialog::getOpenFileName(this, "Open File", "~/CLionProjects/game_of_life/data");
			//	    this->statusBar()->showMessage(filename);
			//    break;
			//}
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
