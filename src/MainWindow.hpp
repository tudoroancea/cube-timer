//
// Created by Tudor Oancea on 13/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_MAINWINDOW_HPP
#define CUBE_TIMER_MAINWINDOW_HPP

#include "TimesList.hpp"
#include "libs/rapidcsv.h"
#include "Settings.hpp"
#include "Scramble.hpp"
//#include "Data.hpp"

#include <QMainWindow>
#include <filesystem>
#include <unordered_map>

QT_BEGIN_NAMESPACE
	class QLabel;
	class QTimer;
	class QHBoxLayout;
QT_END_NAMESPACE
class Data;

#define HeadersNumber 7
struct Headers {
	std::string time;
	std::string mo3;
	std::string ao5;
	std::string ao12;
	std::string scramble;
	std::string timeStamp;
	std::string comment;
	Headers(std::string time, std::string mo3, std::string ao5, std::string ao12, std::string scramble, std::string timeStamp, std::string comment);
	bool matches(std::vector<std::string> const& vec) const;
	std::string operator[](size_t const& index) const;
};

class MainWindow : public QMainWindow {
Q_OBJECT
private:
	//	Global info / settings ============
	/**
	 * @brief contains argv[0]
	 */
	[[maybe_unused]] char* exePath;
	/**
	 * @brief Brief description of the app to display in the About action
	 */
	QString aboutMessage;
	/**
	 * @brief Pretty self-explanatory
	 */
	Settings* settings;

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
	Scramble scramble;
	QLabel* scrambleLabel;
	QHBoxLayout* mainLayout;
	TimesList* timesList;

	// Behavior helps =====================
	/**
 * @brief Indicates that we are indeed starting the timer (the space bar wasn't pressed to stop the timer)
 */
	bool startingTimer = false;
	/**
	 * @brief Indicates if the last action was saving (to current location)..
	 */
	bool savedJustBefore = false;

	// Actions =========
	std::unordered_map<std::string, QAction*> actions;

	// Menus =============
	QMenu* fileMenu;

	//	Utility functions ===============
	void createTimers();
	void createLabels();
	void createActions();
	void createMenus();
	void createAboutMessage();

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
	 * @brief Saves data to current CSV (default or custom location)..
	 */
	void save();
	/**
	 * @brief Saves data to any CSV (default or custom location).
	 */
	void saveAs();

	//void loadCustomCSV();
	void openPreferences();
	void about();

public:
	// CSV Data
	static Data data;
	static size_t currentSession;
	std::filesystem::path dataPath;
	static const Headers metadataHeaders;

	explicit MainWindow(char* const& argv0);
	~MainWindow() override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

public slots:
	void tryScrambleAgain(Scramble const& toTry);
};

#endif //CUBE_TIMER_MAINWINDOW_HPP
