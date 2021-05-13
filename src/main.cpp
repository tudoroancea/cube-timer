//
// Created by Tudor Oancea on 13/04/2021.
//

#include "MainWindow.hpp"
#include <QApplication>


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	MainWindow w(argv[0]);
	w.show();
	return QApplication::exec();
}
