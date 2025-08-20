#pragma once

#ifndef INTERNSHIPTRACKER_MAINWINDOW_H
#define INTERNSHIPTRACKER_MAINWINDOW_H

#include "internshiptable.h"
#include "internshipapimanager.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QPushButton>
#include <QString>

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    InternshipTable *table;
    InternshipApiManager *apiManager;

    QPushButton *addButton, *editButton, *deleteButton;

private slots:
    void onInternshipsFetched(const QJsonArray &tableArr);
};


#endif //INTERNSHIPTRACKER_MAINWINDOW_H
