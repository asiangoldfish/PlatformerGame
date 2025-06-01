/**
 * Example to write to configuration file:
 *   QSettings settings(ConfigManager::getConfigFilepath, QSettings::IniFormat);
 *   settings.setValue("window/width", 1200);
 * 
 * Read from configuration file:
 *   QSettings settings(ConfigManager::getConfigFilepath, QSettings::IniFormat);
 *   int width = settings.value("window/width", 480).toInt(); // default 480
 * 
 * Read more at: https://doc.qt.io/qt-6/qsettings.html
 */
#pragma once

#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include <vector>

namespace ConfigManager {
    /**
     * Get the configuration file path.
     *
     * The parent directory is set in the following order of priority, where
     * APP_NAME is a macro for the application name.
     * 1. $XDG_CONFIG_HOME/APPLICATION_NAME/
     * 2. $HOME/.config/APPLICATION_NAME/
     * 3. The application executable's parent directory.
     *
     * @param fileName The configuration filename.
     */
    inline QString getConfigFilepath(const QString& fileName = "settings.ini") {
        QString configDir;
        QString appName = APPLICATION_NAME;

        std::vector locations = { "XDG_CONFIG_HOME", "HOME" };

        for (auto& location : locations) {
            QByteArray c = qgetenv(location);
            if (!c.isEmpty()) {
                configDir = QString::fromLocal8Bit(c);
                break;
            }
        }

        // Check read/write permissions
        QDir dir(configDir + "/" + appName);
        if (!dir.exists() && !dir.mkpath(".")) {
            qCritical() << "Failed to create configuration directory:"
                        << dir.absolutePath();
            QCoreApplication::exit(1);
            return QString();
        }


        return dir.filePath(fileName);
    }
}