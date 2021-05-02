//
// Created by Tudor Oancea on 02/05/2021.
//

#ifndef CUBE_TIMER_MAINWINDOW2_HPP
#define CUBE_TIMER_MAINWINDOW2_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
	class QVBoxLayout;
	class QHBoxLayout;
	class QScrollArea;
	class QGridLayout;
	class QLabel;
	class QTableWidget;
QT_END_NAMESPACE

class MainWindow2 : public QMainWindow {
private:
	QTableWidget* lhs;
	QVBoxLayout* rhs;
	QLabel* label1;
	QLabel* label2;
	QLabel* label3;

public:
	MainWindow2();
	~MainWindow2();
};


#endif //CUBE_TIMER_MAINWINDOW2_HPP
