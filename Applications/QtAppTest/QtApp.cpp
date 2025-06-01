#include "QtApp.h"
#include "EscapeEventFilter.h"
#include "ConfigManager.hpp"
#include "VSCodiumManager.h"

#include <QSettings>
#include <QSplitter>
#include <QLabel>
#include <QTreeWidget>
#include <QWebEngineView>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDebug>
#include <QMenuBar>

QtApp::QtApp(int& argc, char** argv)
  : app(argc, argv) {

    // Load settings
    QSettings settings(ConfigManager::getConfigFilepath(),
                       QSettings::IniFormat);
    int width = settings.value("window/width", 1280).toInt();
    int height = settings.value("window/height", 720).toInt();

    // Create main window
    mainWindow.setWindowTitle("Qt Editor");
    mainWindow.resize(width, height);
    mainWindow.setDockOptions(QMainWindow::AllowNestedDocks |
                              QMainWindow::AllowTabbedDocks |
                              QMainWindow::AnimatedDocks);

    // Setup event filter for ESC to quit
    // EscapeEventFilter* escFilter = new EscapeEventFilter(&app);
    // app.installEventFilter(escFilter);

    // Start VSCodium server
    vscodium = new VSCodiumManager(
      settings.value("codeEditor/executablePath", "code-server").toString(),
      { "--port",
        settings.value("codeEditor/port", "8080").toString(),
        "--auth",
        "none" });
    vscodium->start();

    setupUI(settings);
}

void QtApp::setupUI(QSettings& settings) {
    // === Scene Tree ===
    QTreeWidget* treeWidget = new QTreeWidget();
    treeWidget->setHeaderLabel("Scene");
    treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList("Root")));

    QDockWidget* sceneDock = new QDockWidget("Scene Tree", &mainWindow);
    sceneDock->setObjectName("SceneTreeDock");
    sceneDock->setWidget(treeWidget);
    sceneDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    sceneDock->setFeatures(QDockWidget::DockWidgetMovable |
                           QDockWidget::DockWidgetFloatable);
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, sceneDock);

    // === Viewport ===
    QLabel* viewport = new QLabel("Viewport");
    viewport->setAlignment(Qt::AlignCenter);
    viewport->setStyleSheet("background-color: #444; color: white;");

    QDockWidget* viewportDock = new QDockWidget("Viewport", &mainWindow);
    viewportDock->setObjectName("ViewportDock");
    viewportDock->setWidget(viewport);
    viewportDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    viewportDock->setFeatures(QDockWidget::DockWidgetMovable |
                              QDockWidget::DockWidgetFloatable);
    mainWindow.addDockWidget(Qt::RightDockWidgetArea, viewportDock);
    mainWindow.splitDockWidget(sceneDock, viewportDock, Qt::Horizontal);

    // === VSCodium (Web) ===
    QWebEngineView* webView = new QWebEngineView();
    webView->load(QUrl("http://localhost:8080"));

    QDockWidget* vscodiumDock = new QDockWidget("Code Editor", &mainWindow);
    vscodiumDock->setObjectName("CodeEditorDock");
    vscodiumDock->setWidget(webView);
    vscodiumDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    vscodiumDock->setFeatures(QDockWidget::DockWidgetMovable |
                              QDockWidget::DockWidgetFloatable);
    mainWindow.addDockWidget(Qt::RightDockWidgetArea, vscodiumDock);
    mainWindow.splitDockWidget(viewportDock, vscodiumDock, Qt::Horizontal);

    // =========== Menu bar =============
    QMenuBar* menuBar = mainWindow.menuBar();

    // File Menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    QObject::connect(
      exitAction, &QAction::triggered, &app, &QApplication::quit);

    // Edit Menu
    QMenu* editMenu = menuBar->addMenu("&Edit");
    QAction* undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    QAction* redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    // connect these to your actual undo/redo slots if you have them

    // You can add more menus similarly:
    // QMenu* viewMenu = menuBar->addMenu("&View");
    // QMenu* helpMenu = menuBar->addMenu("&Help");

    // === Restore layout if available ===
    if (settings.contains("window/state")) {
        mainWindow.restoreGeometry(
          settings.value("window/geometry").toByteArray());
        qDebug() << "Restoring layout:"
                 << mainWindow.restoreState(
                      settings.value("window/state").toByteArray());
    }
}

int QtApp::run() {
    mainWindow.showMaximized();
    return app.exec();
}

QtApp::~QtApp() {
    // Save layout
    QSettings settings(ConfigManager::getConfigFilepath(),
                       QSettings::IniFormat);
    settings.setValue("window/geometry", mainWindow.saveGeometry());
    settings.setValue("window/state", mainWindow.saveState());

    delete vscodium; // This stops the server
}
