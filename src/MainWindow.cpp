//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include "Duration.hpp"

#include <QApplication>
#include <QtWidgets>
#include <QEvent>
#include <QTimer>
#include <QLabel>
#include <iostream>


MainWindow::MainWindow() : centralLabel(new QLabel), timer(new QTimer) {
	timer->setTimerType(Qt::PreciseTimer);
	connect(timer, &QTimer::timeout, this, &MainWindow::changeDisplayedTime);

	centralLabel->setText("0");
	centralLabel->setAlignment(Qt::AlignCenter);
	QFont font(centralLabel->font());
	font.setFamily("Fira Code iScript");
	font.setPointSize(45);
	centralLabel->setFont(font);

	this->setWindowTitle("Cube Timer");
	this->resize(200,200);
	this->move(QGuiApplication::screens()[1]->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);
	this->setCentralWidget(centralLabel);

	this->show();
	this->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	switch (event->key()) {
		case Qt::Key_Space: {
			if (!event->isAutoRepeat()) {
				if (timer->timerId() == -1) {
					timer->start();
					startPoint = std::chrono::high_resolution_clock::now();
//					centralLabel->setText("00000");
				} else {
					timer->stop();
				}
			}
		    break;
		}
		default:
			break;
	}

}
void MainWindow::keyReleaseEvent(QKeyEvent* event) {
	QWidget::keyReleaseEvent(event);
	switch(event->key()) {
		case Qt::Key_Space: {
//			centralLabel->setText("0");
			if (!event->isAutoRepeat()) {
				if (timer->timerId() == -1) {
//					timer->start();
//					startPoint = std::chrono::high_resolution_clock::now();
				}
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
	centralLabel->setText(Duration<long long int>(currentTime.count()).toQString());
}
