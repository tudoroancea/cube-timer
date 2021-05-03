//
// Created by Tudor Oancea on 13/04/2021.
//

#ifndef CUBE_TIMER_MAINWINDOW_HPP
#define CUBE_TIMER_MAINWINDOW_HPP

#include "TimesList.hpp"

#include <QMainWindow>
#include <array>

QT_BEGIN_NAMESPACE
	class QLabel;
	class QTimer;
	class QHBoxLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
	// Timers
	QTimer* timer;
	QTimer* launchingTimer;
	std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;

	// Widgets
	QLabel* timeLabel;
	QLabel* scrambleLabel;
	QHBoxLayout* mainLayout;
	TimesList* timesList;

	//	Status info ============
	char* exePath;
	bool stoppedChronoJustBefore = false;

	//	Settings ========================
	int launchingInterval = 300;

	//	Utility functions ===============
	void createTimers();
	void createLabels();

private slots:
	void makeTimeRed() const;
	void makeTimeGreen() const;
	void resetTimeColor() const;
public slots:
	void changeDisplayedTime();

public:
	MainWindow(char* const& argv0);
	~MainWindow();
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
};

#endif //CUBE_TIMER_MAINWINDOW_HPP
