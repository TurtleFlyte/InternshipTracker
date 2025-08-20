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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), network(new QNetworkAccessManager(this)){
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
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"ID", "Name", "Link", "Status", "App Cycle"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setColumnHidden(0, true); // Hide id column
    table->setCornerButtonEnabled(false);
    table->verticalHeader()->setVisible(false); // Hide row numbers
    table->horizontalHeader()->setMaximumSectionSize(400);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);
    resize(1280, 720);

    apiURL = getApiUrl();
    connect(network, &QNetworkAccessManager::finished, this, &MainWindow::onReplyFinished);

    fetchInternships();
}

void MainWindow::fetchInternships() {
    QNetworkRequest request((QUrl(apiURL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    network->get(request);
}

void MainWindow::onReplyFinished(QNetworkReply *reply) {
    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);

    if(doc.isObject()){
        QJsonArray arr = doc.object()["table"].toArray();
        updateTable(arr);
    }
}

void MainWindow::updateTable(const QJsonArray &rows) {
    table->setRowCount(rows.size());

    for (int i = 0; i < rows.size(); ++i) {
        auto obj = rows[i].toObject();
        table->setItem(i,0, new QTableWidgetItem(obj["id"].toInt()));
        table->setItem(i,1, new QTableWidgetItem(obj["name"].toString()));
        table->setItem(i,2, new QTableWidgetItem(obj["link"].toString()));
        table->setItem(i,3, new QTableWidgetItem(obj["status"].toString()));
        table->setItem(i,4, new QTableWidgetItem(obj["appCycle"].toString()));
    }
    table->resizeColumnsToContents();
}

QString MainWindow::getApiUrl() {
    QString settingsFile = "api_config.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
    QString url = settings.value("API/api_url", "").toString();
    if (url.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "API URL not found in api_config.ini!");
    }
    return url;
}
