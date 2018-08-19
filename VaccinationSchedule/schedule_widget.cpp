#include "schedule_widget.h"

ScheduleWidget::ScheduleWidget(ChildrenModel *_children_model, SettingsModel *_settings_model, QWidget *parent):
    QWidget(parent), children_model(_children_model), settings_model(_settings_model)
{
    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    calendar = new QCalendarWidget;

    QVBoxLayout *layout{new QVBoxLayout};

    layout->addWidget(calendar);
    layout->addWidget(table);
    setLayout(layout);
}

void ScheduleWidget::setModel(ScheduleModel *value)
{
    model = value;
    table->setModel(model);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(calendar, SIGNAL(selectionChanged()), SLOT(calculate()));
    emit calendar->selectionChanged();
}

void ScheduleWidget::clearTable()
{
    int rows{model->rowCount()};
    for(int row{0}; row < rows; ++row)
        model->removeRows(row, rows, QModelIndex());
}

void ScheduleWidget::calculate()
{
    clearTable();
    model->calculateSchedule(calendar->selectedDate(), children_model, settings_model);
}
