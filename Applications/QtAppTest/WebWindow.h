#pragma once

#include <QMainWindow>
#include <QWebEngineView>

class WebWindow : public QMainWindow {
    Q_OBJECT

public:
    WebWindow(const QUrl &url, QWidget *parent = nullptr);

private:
    QWebEngineView *webView;
};
