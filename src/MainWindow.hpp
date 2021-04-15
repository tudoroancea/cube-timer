//
// Created by Tudor Oancea on 13/04/2021.
//

#ifndef CUBE_TIMER_MAINWINDOW_HPP
#define CUBE_TIMER_MAINWINDOW_HPP

#include <QMainWindow>
#include <array>

QT_BEGIN_NAMESPACE
	class QLabel;
	class QTimer;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
private:
	QLabel* timeLabel;
	QLabel* scrambleLabel;
	QTimer* timer;
	std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
	//	Status info ============
	bool stoppedChronoJustBefore;
public slots:
	void changeDisplayedTime();

public:
	MainWindow();
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
};

#endif //CUBE_TIMER_MAINWINDOW_HPP
