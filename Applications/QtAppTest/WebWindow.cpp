#include "WebWindow.h"

WebWindow::WebWindow(const QUrl &url, QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Web Browser");
    resize(1024, 768);

    webView = new QWebEngineView(this);
    setCentralWidget(webView);
    webView->load(url);
}
