#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QSettings>

#include "WebWindow.h"
#include "VSCodiumManager.h"

class QtApp {
public:
    explicit QtApp(int &argc, char **argv);
    virtual ~QtApp();
    int run();

private:
    void setupUI(QSettings& settings);

private:
    QApplication app;
    QMainWindow mainWindow;
    VSCodiumManager *vscodium;
};
