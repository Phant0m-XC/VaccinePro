#ifndef SCHEDULE_MODEL_H
#define SCHEDULE_MODEL_H

#include "child_model.h"
#include "settings_model.h"
#include "children_model.h"
#include <QAbstractTableModel>
#include <QRegularExpression>

class ScheduleModel: public QAbstractTableModel
{
    QList<ChildModel *> *schedule_child;
    QList<Vaccine> *schedule_vaccine;

public:
    ScheduleModel(QObject *parent = nullptr);
    virtual ~ScheduleModel();

    //return count of planned children
    int rowCount(QModelIndex const &parent = QModelIndex()) const;

    //return count of columns for table
    int columnCount(QModelIndex const &parent = QModelIndex()) const;

    //headers for table
    QVariant headerData(int, Qt::Orientation, int) const;

    //return data for item of table
    QVariant data(QModelIndex const &, int role = Qt::DisplayRole) const;

    //remove rows from table
    bool removeRows(int, int, QModelIndex const &);

    //return itemflags for table view
    Qt::ItemFlags flags(QModelIndex const &) const;

    //calculate the list of children to be vaccinated
    void calculateSchedule(QDate const &, ChildrenModel *, SettingsModel *);

private:
    //calculate age of child
    int calculateAge(QDate const &, QDate const &) const;
};

#endif // SCHEDULE_MODEL_H
