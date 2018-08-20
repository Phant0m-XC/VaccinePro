#ifndef SCHEDULE_WIDGET_H
#define SCHEDULE_WIDGET_H

#include "schedule_model.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QCalendarWidget>
#include <QVBoxLayout>

/*Widget for show the list of children to be vaccinated*/

class ScheduleWidget: public QWidget
{
    Q_OBJECT

    QCalendarWidget *calendar;

    ChildrenModel *children_model;
    SettingsModel *settings_model;
    ScheduleModel *model;
    QTableView *table;

public:
    explicit ScheduleWidget(ChildrenModel *, SettingsModel *, QWidget *parent = nullptr);
    void setModel(ScheduleModel *);

private:
    void clearTable();

private slots:
    //start calculate in model
    void calculate();
};

#endif // SCHEDULE_WIDGET_H
