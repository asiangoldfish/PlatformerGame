#include "EscapeEventFilter.h"

bool EscapeEventFilter::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            qApp->quit(); // cleanly exit the app
            return true;  // event handled
        }
    }
    return QObject::eventFilter(obj, event);
}