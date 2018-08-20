#ifndef VACCINES_WIDGET_H
#define VACCINES_WIDGET_H

#include "vaccines_model.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QDataWidgetMapper>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>

/*Widget for show vaccines in repository*/

class VaccinesWidget: public QWidget
{
    Q_OBJECT

    QPushButton *save;

    VaccinesModel *model;
    QTableView *table;

public:
    explicit VaccinesWidget(QWidget *parent = nullptr);
    void setModel(VaccinesModel *);

signals:
    void showAddVaccineWidget();        //signal for show add vaccine widget
    void editVaccine(QModelIndex);      //signal for show edit vaccine widget
    void saveData();                    //signal for show information dialog

private slots:
    void removeItem();                  //remove selected row from table
    void addVaccine(Vaccine &);         //add new vaccine in vaccines model

};

#endif // VACCINES_WIDGET_H
