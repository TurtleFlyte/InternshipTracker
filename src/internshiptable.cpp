#include "../include/internshiptable.h"
#include <QHeaderView>
#include <QJsonObject>
#include <QVBoxLayout>

InternshipTable::InternshipTable(QWidget *parent) : QWidget(parent){
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"ID", "Name", "Link", "Status", "App Cycle"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setColumnHidden(0, true); // Hide id column
    table->setCornerButtonEnabled(false);
    table->verticalHeader()->setVisible(false); // Hide row numbers
    table->horizontalHeader()->setMaximumSectionSize(400);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(table);
    setLayout(layout);
}

void InternshipTable::updateTable(const QJsonArray &rows) {
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
