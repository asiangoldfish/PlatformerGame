#include "QtApp.h"
#include <QTreeWidget>
#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>

QtApp::QtApp(int &argc, char **argv)
    : app(argc, argv) {
    mainWindow.setWindowTitle("Qt Editor");
    mainWindow.resize(1280, 720);

    // Start the VSCodium server
    vscodium = new VSCodiumManager("vscodium-server", {"--port", "8080", "--auth", "none"});
    vscodium->start();

    setupUI();
}

void QtApp::setupUI() {
    QWidget *centralWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    // Left: Scene Tree
    QTreeWidget *sceneTree = new QTreeWidget();
    sceneTree->setHeaderLabel("Scene");
    sceneTree->addTopLevelItem(new QTreeWidgetItem(QStringList("Root")));

    // Center: Viewport
    QLabel *viewport = new QLabel("Viewport");
    viewport->setAlignment(Qt::AlignCenter);
    viewport->setStyleSheet("background-color: #444; color: white;");

    // Right: VSCodium Web View
    QWebEngineView *vscodiumView = new QWebEngineView();
    vscodiumView->load(QUrl("http://localhost:8080"));

    // Create splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(sceneTree);
    splitter->addWidget(viewport);
    splitter->addWidget(vscodiumView);
    splitter->setSizes({300, 500, 480}); // Adjust as needed

    layout->addWidget(splitter);
    mainWindow.setCentralWidget(centralWidget);
}

int QtApp::run() {
    mainWindow.show();
    return app.exec();
}

QtApp::~QtApp() {
    delete vscodium; // Stops the server
}
