// EscapeEventFilter.h
#pragma once
#include <QObject>
#include <QEvent>
#include <QKeyEvent>

class EscapeEventFilter : public QObject {
    Q_OBJECT

public:
    explicit EscapeEventFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
