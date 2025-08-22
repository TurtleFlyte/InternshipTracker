#include "../include/internshipapimanager.h"
#include <QSettings>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>

InternshipApiManager::InternshipApiManager(QObject *parent) : QObject(parent), network(new QNetworkAccessManager){
    connect(network, &QNetworkAccessManager::finished, this, &InternshipApiManager::onReplyFinished); // Connect db response parsing to finished signal

    apiUrl = getApiUrl();
}

void InternshipApiManager::fetchInternships(){
    QNetworkRequest request((QUrl(apiUrl)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = network->get(request);
    pendingRequests[reply] = RequestType::Get;
}

void InternshipApiManager::addInternship(const QJsonObject &data) {
    QNetworkRequest request((QUrl(apiUrl)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray body = QJsonDocument(data).toJson();
    QNetworkReply *reply = network->post(request, body);
    pendingRequests[reply] = RequestType::Post;
}

void InternshipApiManager::deleteInternship(const QJsonObject &data) {
    QNetworkRequest request((QUrl(apiUrl)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray body = QJsonDocument(data).toJson();
    QNetworkReply *reply = network->sendCustomRequest(request, "DELETE", body);
    pendingRequests[reply] = RequestType::Delete;
}

void InternshipApiManager::onReplyFinished(QNetworkReply *reply){
    auto it = pendingRequests.find(reply); // Get iterator to request type
    if(it == pendingRequests.end()){ // Delete reply if request type does not exist
        reply->deleteLater();
        return;
    }

    RequestType requestType = *it;
    pendingRequests.erase(it);

    if(reply->error() != QNetworkReply::NoError){
        QString err = reply->errorString();
        emit errorOccurred(err);
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &jsonError);
    if(jsonError.error != QJsonParseError::NoError){
        emit errorOccurred("JSON parse error: " + jsonError.errorString());
        return;
    }

    QJsonObject obj = doc.object();

    switch (requestType) {
        case RequestType::Get:{
            QJsonArray arr = doc.object()["table"].toArray();
            emit internshipsFetched(arr);
            break;
        }
        case RequestType::Post:{
//            int newId = obj["id"].toInt(-1);
            emit internshipAdded();
            break;
        }
        case RequestType::Put:{

            break;
        }
        case RequestType::Delete:{
            emit internshipDeleted();
            break;
        }
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
