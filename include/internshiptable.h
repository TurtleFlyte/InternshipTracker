#ifndef INTERNSHIPTRACKER_INTERNSHIPTABLE_H
#define INTERNSHIPTRACKER_INTERNSHIPTABLE_H

#pragma once
#include <QTableWidget>
#include <QJsonArray>

class InternshipTable : public QWidget {
    Q_OBJECT
public:
    explicit InternshipTable(QWidget *parent = nullptr);
    void updateTable(const QJsonArray &rows);
    void enableTableEditing(bool enabled);
    int getSelectedRow();
    int getSelectedId();

signals:
    void internshipEdited(const QJsonObject &data);

private slots:
    void onInternshipEdited(QTableWidgetItem *item);

private:
    QTableWidget *table;
};

#endif //INTERNSHIPTRACKER_INTERNSHIPTABLE_H
