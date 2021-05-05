//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include "Duration.hpp"
#include "Scramble.hpp"
#include "TimesList.hpp"

#include <QApplication>
#include <QtWidgets>
#include <QEvent>
#include <QTimer>
#include <QLabel>
#include <QBoxLayout>
#include <QAction>
#include <QFileDialog>
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
	#ifdef RELEASE_MODE
		this->statusBar()->addWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
	#else
		this->statusBar()->showMessage("Press Space bar to start timer");
	#endif

	createTimers();
	createLabels();
	createActions();
	createMenus();

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
	if (autoSave) {
		timesList->saveToCurrentCSV();
	}
	delete timeLabel;
	delete scrambleLabel;
	delete timer;
	delete launchingTimer;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	if (timer->isActive()) {
		timer->stop();
		std::chrono::time_point<std::chrono::high_resolution_clock> now(std::chrono::high_resolution_clock::now());
		std::chrono::milliseconds currentTime(std::chrono::duration_cast<std::chrono::milliseconds>(now-startPoint));
		timeLabel->setText(Duration<long long int>(currentTime.count()).toQString());
		stoppedChronoJustBefore = true;
		scrambleLabel->setText(Scramble().toQString());
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

void MainWindow::createActions() {
	actions["save"] = new QAction("Save", this);
	actions["save"]->setShortcut(QKeySequence::Save);
	connect(actions["save"], SIGNAL(triggered()), timesList, SLOT(saveToCurrentCSV()));

	actions["saveAs"] = new QAction("Save As", this);
	actions["saveAs"]->setShortcut(QKeySequence::SaveAs);
	connect(actions["saveAs"], SIGNAL(triggered()), this, SLOT(saveAs()));

	actions["loadDefaultCSV"] = new QAction("Load default CSV", this);
	actions["loadDefaultCSV"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_D));
	connect(actions["loadDefaultCSV"], SIGNAL(triggered()), this, SLOT(loadDefaultCSV()));

	actions["loadCustomCSV"] = new QAction("Load custom CSV", this);
	actions["loadCustomCSV"]->setShortcut(QKeySequence::Open);
	connect(actions["loadCustomCSV"], SIGNAL(triggered()), this, SLOT(loadCustomCSV()));
}
void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction(actions["save"]);
	fileMenu->addAction(actions["saveAs"]);
	fileMenu->addAction(actions["loadDefaultCSV"]);
	fileMenu->addAction(actions["loadCustomCSV"]);
}

void MainWindow::saveAs() {
	QString str(QFileDialog::getSaveFileName(this, "Save current data", "/Users/untitled.csv", "CSV files (*.csv)"));
	if (!str.isEmpty()) {
		timesList->saveToCustomCSV(str.toStdString());
	}
}

void MainWindow::loadDefaultCSV() {
	if (!timesList->isCurrentCSVDefault()) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::information(this, QString(), "Do you want to save current data ?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
		switch (reply) {
			case QMessageBox::Ok: {
				timesList->saveToCurrentCSV();
				timesList->loadDefaultCSV();
				break;
			}
			case QMessageBox::No:
				timesList->loadDefaultCSV();
			    break;
			default:
				break;
		}
	} else {
		this->statusBar()->showMessage("The default CSV is already open", 1000);
	}
}

void MainWindow::loadCustomCSV() {
	QMessageBox::StandardButton reply(QMessageBox::question(this, QString(), "Do you want to save current data?",  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No));
	bool letsDoIt(false);
	switch (reply) {
		case QMessageBox::Yes: {
			timesList->saveToCurrentCSV();
			letsDoIt = true;
			break;
		}
		case QMessageBox::No: {
			letsDoIt = true;
			break;
		}
		default:
			break;
	}
	if (letsDoIt) {
		QString path(QFileDialog::getOpenFileName(this, "Save current data", "/Users/untitled.csv", "CSV files (*.csv)"));
		if (!path.isEmpty()) {
			try {
				timesList->loadCustomCSV(path.toStdString());
			} catch (TimesList::Error const& err) {
				QString message;
				switch (err) {
					case TimesList::wrongPath: {
						message = QString("There was a problem with the given path. The document has not been open.");
						break;
					}
					case TimesList::wrongFormat:
						message = QString("The given file did not have the right format (not 4 columns or not the right headers). The new data has not been loaded.");
						break;
					default:
						break;
				}
				//QMessageBox::StandardButton newReply(QMessageBox::warning(this, "", message));
				QMessageBox::warning(this, "", message);
			}
		}
	}
}
