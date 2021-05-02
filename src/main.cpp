#include "MainWindow.hpp"
#include "MainWindow2.hpp"
#include <QApplication>


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
