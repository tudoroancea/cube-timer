//
// Created by Tudor Oancea on 13/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "MainWindow.hpp"
#include <QApplication>


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	MainWindow w(argv[0]);
	w.show();
	return QApplication::exec();
}
