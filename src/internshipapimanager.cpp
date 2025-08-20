#include "../include/internshipapimanager.h"
#include <QSettings>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>

InternshipApiManager::InternshipApiManager(QObject *parent) : QObject(parent), network(new QNetworkAccessManager){
    connect(network, &QNetworkAccessManager::finished, this, &InternshipApiManager::onReplyFinished);

    apiUrl = getApiUrl();
}

void InternshipApiManager::fetchInternships(){
    QNetworkRequest request((QUrl(apiUrl)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    network->get(request);
}

void InternshipApiManager::onReplyFinished(QNetworkReply *reply){
    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);

    if(doc.isObject()){
        QJsonArray arr = doc.object()["table"].toArray();
        emit internshipsFetched(arr);
    }
}

QString InternshipApiManager::getApiUrl(){
    QString settingsFile = "api_config.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
    QString url = settings.value("API/api_url", "").toString();
    if (url.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "API URL not found in api_config.ini!");
    }
    return url;
}