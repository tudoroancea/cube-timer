//
// Created by Tudor Oancea on 13/04/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "MainWindow.hpp"
#include "Data.hpp"

#include <QApplication>
#include <memory>
#include <filesystem>


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	std::filesystem::path dataPath(argv[0]);
	dataPath /= "../../Resources/default-historic.csv";
	dataPath = std::filesystem::canonical(dataPath);
	MainWindow::data = std::make_unique<Data>(dataPath.string());

	MainWindow::settings = std::make_unique<Settings>(argv[0]);

	MainWindow w;
	w.show();
	return QApplication::exec();
}
