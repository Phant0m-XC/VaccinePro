#ifndef ADD_CHILD_WIDGET_H
#define ADD_CHILD_WIDGET_H

#include "children_model.h"
#include "settings_model.h"
#include "vaccines_model.h"
#include "qdateedit_delegate.h"
#include "proxysearchmodel.h"
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QTableView>
#include <QDataWidgetMapper>
#include <QHeaderView>
#include <QSortFilterProxyModel>

/*Widget for adding child to children model*/

class AddChildWidget: public QDialog
{
    Q_OBJECT

    ChildModel *model;

    QLineEdit *txt_f_name;                          //first name
    QLineEdit *txt_m_name;                          //middle name
    QLineEdit *txt_l_name;                          //last name
    QCalendarWidget *cal_birthday;

    QLabel *lbl_name;
    QLabel *lbl_trade_name;
    QLabel *lbl_serial;
    QLabel *lbl_country;
    QLabel *lbl_best_before;
    QLabel *lbl_vaccination_date;
    QRadioButton *radio1;                           //toggled if adding vaccine from repository
    QRadioButton *radio2;                           //toggled if adding custom vaccine's data
    QComboBox *comb_vacc_repository;
    QComboBox *comb_vacc_settings;
    QLineEdit *txt_trade_name;
    QLineEdit *txt_serial;
    QLineEdit *txt_country;
    QDateEdit *best_before;
    QDateEdit *vaccination_date;                    //date of vaccination

    QTableView *table;
    bool isEditMode;                                //add new data == false, correct data == true

public:
    QDataWidgetMapper *mapper;                      //show data in appropriate widget
    explicit AddChildWidget(QDateEditDelegate *, VaccinesModel *, SettingsModel *, QWidget *parent = nullptr);

signals:
    void applyChild(ChildModel *);

private slots:
    void showWidget();                  //show widget in mode "add new data"
    void showEditWidget(QModelIndex);   //show widget in mode "correct data"
    void radioButtonChange(bool);       //change enable/disable widgets
    void addVaccine();                  //add vaccine to child
    void removeVaccine();               //remove selected row from table
    void addChild();                    //add new child or correct current data

};

#endif // ADD_CHILD_WIDGET_H
