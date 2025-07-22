#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "mainwindow.h"

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>


class UpdateManager : public QObject {
    Q_OBJECT

public:
    explicit UpdateManager(MainWindow *mwr, QObject *parent = nullptr);
    void CheckForUpdateAndPromptUser();
    void PostAnonymousUsageLog();

private:
    MainWindow *mw;
    QNetworkAccessManager* manager;
    const QString githubVersionRef = "https://raw.githubusercontent.com/Bo0sted/CrossplatformStopwatch/refs/heads/master/version/version";
    const QString cloudflarePingEndpoint = "https://workers-playground-delicate-heart-a0b1.donttrackmepls.workers.dev/ping";
    const QString currentVersion;

};

#endif // UPDATEMANAGER_H
