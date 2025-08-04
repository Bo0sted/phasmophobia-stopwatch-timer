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
    void CheckForUpdateAndPromptUser(std::function<void()> onFinished = nullptr);
    void PostAnonymousUsageLog();
    void OpenGithubInBrowser();
    void OpenGithubIssuesInBrowser();
    bool WasUpdateCheckSuccessful();
    void SetUpdateCheckSuccessful(bool successful);

private:
    MainWindow *mw;
    QNetworkAccessManager* manager;
    const QString githubVersionRef = "https://raw.githubusercontent.com/Bo0sted/phasmophobia-stopwatch-timer/refs/heads/master/version/version";
    const QString githubBaseUrl = "https://github.com/Bo0sted/phasmophobia-stopwatch-timer";
    const QString cloudflarePingEndpoint = "https://workers-playground-delicate-heart-a0b1.donttrackmepls.workers.dev/ping";
    // If an update is performed then its return false is stored here
    bool wasUpdateCheckSuccessful = false;


};

#endif // UPDATEMANAGER_H
