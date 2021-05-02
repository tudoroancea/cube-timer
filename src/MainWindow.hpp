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
	class QGridLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
	QLabel* timeLabel;
	QLabel* scrambleLabel;
	QTimer* timer;
	QTimer* launchingTimer;
	std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
	//	Status info ============
	bool stoppedChronoJustBefore = false;

	//	Settings ========================
	int launchingInterval = 300;

private slots:
	void makeTimeRed() const;
	void makeTimeGreen() const;
	void resetTimeColor() const;
public slots:
	void changeDisplayedTime();

public:
	MainWindow();
	~MainWindow();
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
};

#endif //CUBE_TIMER_MAINWINDOW_HPP
