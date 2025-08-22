#include "../include/internshiptable.h"
#include <QHeaderView>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QComboBox>

InternshipTable::InternshipTable(QWidget *parent) : QWidget(parent){
    table = new QTableWidget(0, 5, this);

    // Table settings
    table->setHorizontalHeaderLabels({"ID", "Name", "Link", "Status", "App Cycle"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::DoubleClicked);
    table->setColumnHidden(0, true); // Hide id column
    table->setCornerButtonEnabled(false);
    table->verticalHeader()->setVisible(false); // Hide row numbers
    table->horizontalHeader()->setMaximumSectionSize(400);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(table);
    setLayout(layout);
}

void InternshipTable::updateTable(const QJsonArray &rows) {
    table->clearContents();
    table->setRowCount(rows.size());

    QStringList statusOptions = {"Seen", "Applied", "Ghosted", "Rejected", "Interview", "Offer"};

    for (int i = 0; i < rows.size(); ++i) {
        auto obj = rows[i].toObject();
        table->setItem(i,0, new QTableWidgetItem(QString::number(obj["id"].toInt())));
        table->setItem(i,1, new QTableWidgetItem(obj["name"].toString()));
        table->setItem(i,2, new QTableWidgetItem(obj["link"].toString()));
        auto *statusCombo = new QComboBox;
        statusCombo->addItems(statusOptions);
        int statusIndex = statusOptions.indexOf(obj["status"].toString());
        statusCombo->setCurrentIndex(statusIndex >= 0 ? statusIndex : 0);
        table->setCellWidget(i,3, statusCombo);
        table->setItem(i,4, new QTableWidgetItem(obj["appCycle"].toString()));
    }
    table->resizeColumnsToContents();
}

int InternshipTable::getSelectedRow() {
    return table->currentRow();
}

int InternshipTable::getSelectedId() {
    return table->item(getSelectedRow(),0)->text().toInt();
}
