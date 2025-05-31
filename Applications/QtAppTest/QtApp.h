#pragma once

#include <QApplication>
#include <QMainWindow>
#include "WebWindow.h"
#include "VSCodiumManager.h"

class QtApp {
public:
    QtApp(int &argc, char **argv);
    virtual ~QtApp();
    int run();

private:
    void setupUI();

private:
    QApplication app;
    QMainWindow mainWindow;
    VSCodiumManager *vscodium;
};
