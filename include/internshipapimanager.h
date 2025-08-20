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

signals:
    void internshipsFetched(const QJsonArray &tableArr);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *network;
    QString apiUrl;

    QString getApiUrl();
};


#endif //INTERNSHIPTRACKER_INTERNSHIPAPIMANAGER_H
