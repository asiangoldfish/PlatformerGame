#include "VSCodiumManager.h"

VSCodiumManager::VSCodiumManager(const QString &execPath, const QStringList &args)
    : executable(execPath), arguments(args) {}

void VSCodiumManager::start() {
    process.start(executable, arguments);
}

void VSCodiumManager::stop() {
    if (process.state() != QProcess::NotRunning) {
        process.terminate();
        if (!process.waitForFinished(3000)) {
            process.kill();
        }
    }
}

VSCodiumManager::~VSCodiumManager() {
    stop();
}
