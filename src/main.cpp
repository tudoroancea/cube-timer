#include "MainWindow.hpp"
#include "MainWindow2.hpp"
#include <QApplication>


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	MainWindow w(argv[0]);
	w.show();
	return QApplication::exec();
}
