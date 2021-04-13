//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"

#include <QApplication>
#include <QtWidgets>
#include <QEvent>
#include <QTimer>
#include <QLabel>
#include <iostream>


MainWindow::MainWindow() : centralLabel(new QLabel), timer(new QTimer) {
	timer->setTimerType(Qt::PreciseTimer);
	connect(timer, &QTimer::timeout, this, &MainWindow::changeDisplayedTime);

	centralLabel->setNum(0);
	centralLabel->setAlignment(Qt::AlignCenter);

	this->setWindowTitle("Cube Timer");
	this->resize(100,100);
	this->move(QGuiApplication::screens()[0]->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);
	this->setCentralWidget(centralLabel);
	this->show();
	this->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	switch (event->key()) {
		case Qt::Key_Space: {
			if (timer->timerId() == -1) {
				timer->start();
				startPoint = std::chrono::high_resolution_clock::now();
			} else {
				timer->stop();
				centralLabel->setNum(0);
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
	centralLabel->setNum((int)currentTime.count());
	centralLabel->update();
}
