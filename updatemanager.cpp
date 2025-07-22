#include "mainwindow.h"
#include "updatemanager.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QDebug>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>


UpdateManager::UpdateManager(MainWindow *mwr, QObject *parent)
    : manager(new QNetworkAccessManager(this))
    , currentVersion{QCoreApplication::applicationVersion()}
    , mw(mwr)
{
}

void UpdateManager::CheckForUpdateAndPromptUser() {
    QNetworkRequest request(githubVersionRef);
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString latestVersion = QString(reply->readAll()).trimmed();
            qDebug() << "Current version:" << currentVersion;
            qDebug() << "Latest version:" << latestVersion;

        if (latestVersion > currentVersion) {
            QMessageBox::StandardButton result = QMessageBox::question(
                nullptr,
                "Update Available",
                QString("A new version (%1) is available. Would you like to visit the download page?").arg(latestVersion),
                QMessageBox::Open | QMessageBox::Ignore
                );

            if (result == QMessageBox::Yes) {
                QDesktopServices::openUrl(QUrl("https://github.com/Bo0sted/CrossplatformStopwatch/releases"));
            }
        }
        else {
            qDebug() << "You're up to date.";
        }
        mw->SetLastUpdateCheckUnix(QDateTime::currentSecsSinceEpoch());
    } else {
        qWarning() << "Version check failed:" << reply->errorString();
    }

    reply->deleteLater();
    });
}

void UpdateManager::PostAnonymousUsageLog()
{
    QUrl endpoint(cloudflarePingEndpoint);
    QNetworkRequest request(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["os"] = QSysInfo::prettyProductName();
    json["version"] = QCoreApplication::applicationVersion();
    json["uuid"] = mw->qsm.FetchUUID();
    QLocale locale;
    json["country"] = QLocale::countryToString(locale.country());
    //Using server timestamp instead
    //json["timestamp"] = QDateTime::currentSecsSinceEpoch();

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply* reply = manager->post(request, data);

    // Optional: connect to handle the reply asynchronously
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        mw->SetLastPingUnix(QDateTime::currentSecsSinceEpoch());
    });
}


