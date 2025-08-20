#ifndef INTERNSHIPTRACKER_INTERNSHIPTABLE_H
#define INTERNSHIPTRACKER_INTERNSHIPTABLE_H

#pragma once
#include <QTableWidget>

class InternshipTable : QWidget {
    Q_OBJECT
public:
    explicit InternshipTable(QWidget *parent = nullptr);
    void updateTable(const QJsonArray &rows);

private:
    QTableWidget *table;
};

#endif //INTERNSHIPTRACKER_INTERNSHIPTABLE_H
