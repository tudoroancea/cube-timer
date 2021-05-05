//
// Created by Tudor Oancea on 13/04/2021.
//

#ifndef CUBE_TIMER_MAINWINDOW_HPP
#define CUBE_TIMER_MAINWINDOW_HPP

#include "TimesList.hpp"

#include <QMainWindow>
#include <unordered_map>

QT_BEGIN_NAMESPACE
	class QLabel;
	class QTimer;
	class QHBoxLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
	// Timers
	/**
	 * @brief Main timer used to time the cube and update the displayed time live
	 */
	QTimer* timer;
	/**
	 * @brief Single-shot timer with launcingInterval s to make sure that one did not start the timer by mistake
	 */
	QTimer* launchingTimer;
	/**
	 * @brief Reference time point (beginning of timing) for timer results.
	 */
	std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;

	// Widgets
	QLabel* timeLabel;
	QLabel* scrambleLabel;
	QHBoxLayout* mainLayout;
	TimesList* timesList;

	//	Status info ============
	/**
	 * @brief contains argv[0]
	 */
	char* exePath;
	/**
	 * @brief Used to ensure that when the timer is running and we press space, it doesn't start again when we release the space bar
	 */
	bool stoppedChronoJustBefore = false;

	//	Settings ========================
	int launchingInterval = 300;
	bool autoSave = true;

	// Actions =========
	std::unordered_map<std::string, QAction*> actions;

	// Menus =============
	QMenu* fileMenu;

	//	Utility functions ===============
	void createTimers();
	void createLabels();
	void createActions();
	void createMenus();

private slots:
	/**
	 * @brief Change the color of timeLabel to red.
	 */
	void makeTimeRed() const;
	/**
	 * @brief Change the color of timeLabel to green.
	 */
	void makeTimeGreen() const;
	/**
	 * @brief Resets the color of timeLabel.
	 */
	void resetTimeColor() const;
	/**
	 * @brief Update timeLabel to match the time that has passed since startPoint.
	 */
	void changeDisplayedTime();

//	Actions ==========
	/**
	 * @brief Saves data to any CSV (default or custom location).
	 */
	void saveAs();
	void loadDefaultCSV();
	void loadCustomCSV();

public:
	explicit MainWindow(char* const& argv0);
	~MainWindow() override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
};

#endif //CUBE_TIMER_MAINWINDOW_HPP
