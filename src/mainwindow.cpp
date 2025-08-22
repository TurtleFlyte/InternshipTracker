#include "../include/mainwindow.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCore/qjsonobject.h>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), apiManager(new InternshipApiManager(this)){
    auto *central = new QWidget;
    auto *layout = new QVBoxLayout;
    auto *buttonLayout = new QHBoxLayout;

    // Setup buttons
    addButton = new QPushButton("Add");
    deleteButton = new QPushButton("Delete");
    addButton->setFixedWidth(100);
    deleteButton->setFixedWidth(100);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->setAlignment(Qt::AlignLeft);
    layout->addLayout(buttonLayout);

    // Setup table
    table = new InternshipTable(this);
    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);
    resize(1280, 720);

    connect(apiManager, &InternshipApiManager::internshipsFetched, this, &MainWindow::onInternshipsFetched); // Connect api manager signal to main window
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(table, &InternshipTable::internshipEdited, this, &MainWindow::onTableItemChanged);

    connect(apiManager, &InternshipApiManager::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(apiManager, &InternshipApiManager::internshipAdded, this, &MainWindow::fetchInternships);
    connect(apiManager, &InternshipApiManager::internshipDeleted, this, &MainWindow::fetchInternships);
    connect(apiManager, &InternshipApiManager::internshipEdited, this, &MainWindow::onCloudTableEdited);

    apiManager->fetchInternships();
}

void MainWindow::onInternshipsFetched(const QJsonArray &tableArr){
    table->updateTable(tableArr);

    table->enableTableEditing(true);
}

void MainWindow::fetchInternships() {
    apiManager->fetchInternships();
}

void MainWindow::onErrorOccurred(const QString &err) {
    QMessageBox::critical(this, "Network error: ", err);
}

void MainWindow::onAddClicked() {
    QDialog addDialog(this);
    QVBoxLayout addDialogLayout(&addDialog);
    QLineEdit name, link, appCycle;
    QComboBox status;
    status.addItems({"Seen", "Applied", "Ghosted", "Rejected", "Interview", "Offer"});
    addDialogLayout.addWidget(new QLabel("Add a New Internship"));
    addDialogLayout.addWidget(new QLabel("Name:")); addDialogLayout.addWidget(&name);
    addDialogLayout.addWidget(new QLabel("Link:")); addDialogLayout.addWidget(&link);
    addDialogLayout.addWidget(new QLabel("Application Status:")); addDialogLayout.addWidget(&status);
    addDialogLayout.addWidget(new QLabel("Application Cycle:")); addDialogLayout.addWidget(&appCycle);

    QPushButton okay("OK"), cancel("Cancel");
    QHBoxLayout buttonLayout;
    buttonLayout.addWidget(&cancel);
    buttonLayout.addWidget(&okay);
    addDialogLayout.addLayout(&buttonLayout);

    connect(&cancel, &QPushButton::clicked, &addDialog, &QDialog::reject);
    connect(&okay, &QPushButton::clicked, &addDialog, &QDialog::accept);

    if(addDialog.exec() == QDialog::Accepted){
        QJsonObject object;
        object["name"] = name.text();
        object["link"] = link.text();
        object["status"] = status.currentText();
        object["appCycle"] = appCycle.text();

        apiManager->addInternship(object);
    }

    table->enableTableEditing(false);
}

void MainWindow::onDeleteClicked() {
    if(table->getSelectedRow() == -1) {
        QMessageBox::warning(this, "Table error: ", "No Internship Selected");
        return;
    }
    int id = table->getSelectedId();
    QJsonObject object;
    object["id"] = id;
    if(QMessageBox::question(this, "Delete Internship", "Are you sure?") == QMessageBox::Yes)
        apiManager->deleteInternship(object);

    table->enableTableEditing(false);
}

void MainWindow::onTableItemChanged(const QJsonObject &data) {
    apiManager->editInternship(data);

    table->enableTableEditing(false);
}

void MainWindow::onCloudTableEdited(int id) {
    table->enableTableEditing(true);
}
