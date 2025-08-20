#include "../include/mainwindow.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCore/qjsonobject.h>
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QMessageBox>
#include <QNetworkAccessManager>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), apiManager(new InternshipApiManager(this)){
    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // Setup buttons
    addButton = new QPushButton("Add");
    editButton = new QPushButton("Edit");
    deleteButton = new QPushButton("Delete");
    addButton->setFixedWidth(100);
    editButton->setFixedWidth(100);
    deleteButton->setFixedWidth(100);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->setAlignment(Qt::AlignLeft);
    layout->addLayout(buttonLayout);

    // Setup table
    table = new InternshipTable(this);
    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);
    resize(1280, 720);

    connect(apiManager, &InternshipApiManager::internshipsFetched, this, &MainWindow::onInternshipsFetched);

    apiManager->fetchInternships();
}

void MainWindow::onInternshipsFetched(const QJsonArray &tableArr){
    table->updateTable(tableArr);
}