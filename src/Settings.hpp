//
// Created by Tudor Oancea on 05/05/2021.
// Copyright (c) 2021 Tudor Oancea.. All rights reserved.
// Licensed under the MIT licence (see details at https://github.com/tudoroancea/cube-timer/blob/develop/LICENSE)
//

#ifndef CUBE_TIMER_SETTINGS_HPP
#define CUBE_TIMER_SETTINGS_HPP

#include "libs/rapidcsv.h"

#include <QObject>
#include <QVariant>
#include <exception>
#include <filesystem>

QT_BEGIN_NAMESPACE
class QString;
class QVariant;
class QWidget;
QT_END_NAMESPACE

namespace fs = std::filesystem;
namespace csv = rapidcsv;

class Settings : public QObject {
    Q_OBJECT
private:
    fs::path settingsPath;
    fs::path defaultSettingsPath;
    csv::Document data;

    bool settingExists(QString const& key);
    static void settingDoesntExistWarning(QString const& key, QWidget* warningParent = nullptr);

public:
    enum ErrorType { wrongPath,
                     wrongFormat,
                     settingDoesntExist,
                     badConversion };
    struct Error : public std::exception {
        ErrorType type_;
        explicit Error(ErrorType const& t) : type_(t) {}
        [[nodiscard]] ErrorType const& type() const { return type_; }
        [[nodiscard]] const char* what() const noexcept override {
            switch (type_) {
                case wrongPath:
                    return "Wrong Path";
                case wrongFormat:
                    return "Wrong Format";
                case settingDoesntExist:
                    return "Settings does not exist";
                case badConversion:
                    return "Bad Conversion";
                default:
                    return "Settings::Error";
            }
        }
	};

	explicit Settings(char* const& argv0);
	~Settings() override;

	/**
	 * @brief Exception safe method. If the asked setting does not exist, a warning dialog will be created and a default value will be returned.
	 * @param key name of the desired setting.
	 * @param defaultValue value to return if the setting does not exist.
	 * @param warningParent parent of the warning dialog.
	 */
	QVariant getSetting(QString const& key, QVariant const& defaultValue = QVariant(), QWidget* warningParent = nullptr) noexcept;
	/**
	 * @brief Resets the settings to their default values specified by the default-settings.csv file.s
	 */
	void reset();

public slots:
	/**
	 * @brief Exception safe method. If the asked setting does not exist, a warning dialog will be created.
	 * @param key name of the desired setting.
	 * @param value new value to give the setting.
	 * @param warningParent parent of the warning dialog.
	 */
	void setSetting(QString const& key, QVariant const& value, QWidget* warningParent = nullptr) noexcept;
};

#endif  //CUBE_TIMER_SETTINGS_HPP
