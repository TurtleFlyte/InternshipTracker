#ifndef INTERNSHIPTRACKER_INTERNSHIPAPIMANAGER_H
#define INTERNSHIPTRACKER_INTERNSHIPAPIMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class InternshipApiManager : public QObject{
    Q_OBJECT
public:
    explicit InternshipApiManager(QObject *parent = nullptr);
    void fetchInternships();
    void addInternship(const QJsonObject &data);
    void deleteInternship(const QJsonObject &data);

signals:
    void internshipsFetched(const QJsonArray &tableArr);
    void internshipAdded();
    void internshipDeleted();
    void errorOccurred(const QString &err);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *network;
    QString apiUrl;

    enum class RequestType {Get, Post, Put, Delete};
    QHash<QNetworkReply*, RequestType> pendingRequests;

    QString getApiUrl();
};


#endif //INTERNSHIPTRACKER_INTERNSHIPAPIMANAGER_H
