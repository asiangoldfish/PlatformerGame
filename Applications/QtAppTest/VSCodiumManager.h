#pragma once

#include <QProcess>
#include <QString>

class VSCodiumManager {
public:
    VSCodiumManager(const QString &execPath, const QStringList &args = {});
    ~VSCodiumManager();

    void start();
    void stop();

private:
    QProcess process;
    QString executable;
    QStringList arguments;
};
