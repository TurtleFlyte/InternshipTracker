#pragma once

#ifndef INTERNSHIPTRACKER_MAINWINDOW_H
#define INTERNSHIPTRACKER_MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QPushButton>
#include <QString>

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private:
    QTableWidget *table;

    QNetworkAccessManager *network;
    QString apiURL;
    QPushButton *addButton, *editButton, *deleteButton;

private slots:
    void fetchInternships();
    void onReplyFinished(QNetworkReply *reply);
    void updateTable(const QJsonArray &rows);

    QString getApiUrl();
};


#endif //INTERNSHIPTRACKER_MAINWINDOW_H
