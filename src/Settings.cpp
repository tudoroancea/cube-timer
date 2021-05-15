//
// Created by Tudor Oancea on 05/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#include "Settings.hpp"
#include "libs/rapidcsv.h"

#include <QVariant>
#include <QString>
#include <QMessageBox>
#include <QDialog>

namespace fs = std::filesystem;
namespace csv = rapidcsv;

Settings::Settings(char* const& argv0) {
	settingsPath = fs::path(argv0);
	settingsPath /= "../../Resources/settings.csv";
	settingsPath = fs::canonical(settingsPath);
	defaultSettingsPath = fs::path(argv0);
	defaultSettingsPath /= "../../Resources/default-settings.csv";
	defaultSettingsPath = fs::canonical(defaultSettingsPath);

	try {
		data.Load(settingsPath.string(),
			csv::LabelParams(0,0),
			csv::SeparatorParams(),
			csv::ConverterParams(true, 0.0, 0));
	} catch (...) {
		#ifdef DEBUG_MODE
		std::cerr << "Settings file not found." << std::endl;
		#endif
		throw Error(wrongPath);
	}
	size_t nbrCols(data.GetColumnCount());
	if (nbrCols != 1) {
		#ifdef DEBUG_MODE
		std::cerr << "Settings file does not have the right format (not 1 header column and 1 value column)." << std::endl;
		#endif
		try {
			data.Load(defaultSettingsPath.string(),
					  csv::LabelParams(0,0),
					  csv::SeparatorParams(),
					  csv::ConverterParams(true, 0.0, 0));
		} catch (...) {
			#ifdef DEBUG_MODE
			std::cerr << "Default settings file not found." << std::endl;
			#endif
			throw Error(wrongPath);
		}
		nbrCols = data.GetColumnCount();
		if (nbrCols != 1) {
			#ifdef DEBUG_MODE
			std::cerr << "Both settings and default settings do not have the right format (not 1 header column and 1 value column)." << std::endl;
			#endif
			throw Error(wrongFormat);
		}
		QMessageBox::warning(nullptr, "", "The default settings have been loaded as the current ones did not have the right format.");
	}
}

Settings::~Settings() {
	data.Save(settingsPath.string());
}

QVariant Settings::getSetting(QString const& key, QVariant const& defaultValue, QWidget* warningParent) noexcept {
	if (this->settingExists(key)) {
		return QVariant(data.GetCell<std::string>(0, key.toStdString()).c_str());
	} else {
		settingDoesntExistWarning(key, warningParent);
		return defaultValue;
	}
}

void Settings::setSetting(QString const& key, QVariant const& value, QWidget* warningParent) noexcept {
	if (this->settingExists(key)) {
		data.SetCell("value", key.toStdString(), value.toString().toStdString());
		data.Save(settingsPath.string());
	} else {
		settingDoesntExistWarning(key, warningParent);
	}
}

bool Settings::settingExists(QString const& key) {
	std::vector<std::string> list(data.GetRowNames());
	return (std::find(list.begin(), list.end(), key.toStdString()) != list.end());
}

void Settings::reset() {
	data.Load(defaultSettingsPath.string(),
			  csv::LabelParams(0,0),
			  csv::SeparatorParams(),
			  csv::ConverterParams(true, 0.0, 0));
	data.Save(settingsPath.string());
}

void Settings::settingDoesntExistWarning(QString const& key, QWidget* warningParent) {
	QString message("The setting \"");
	message += key;
	message += "\" does not exist.";
	QMessageBox::warning(warningParent, "Settings error", message);
	#ifdef DEBUG_MODE
	std::cerr << message.toStdString() << std::endl;
	#endif
}

