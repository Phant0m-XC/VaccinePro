#ifndef ADD_VACCINE_WIDGET_H
#define ADD_VACCINE_WIDGET_H

#include "vaccine.h"
#include "settings_model.h"
#include <QDialog>
#include <QDataWidgetMapper>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCalendarWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>

/*Widget for adding vaccine to vaccines model*/

class AddVaccineWidget: public QDialog
{
    Q_OBJECT

    QComboBox *comb_name;                       //name of vaccine
    QLineEdit *txt_trade_name;                  //trade name of vaccine
    QLineEdit *txt_serial;                      //serial number of vaccine
    QLineEdit *txt_country;                     //made in...
    QCalendarWidget *cal_best_before;           //best before
    bool isEditMode;                            //add new data == false, correct data == true

public:
    QDataWidgetMapper *mapper;                  //show data in appropriate widget
    explicit AddVaccineWidget(SettingsModel*, QWidget *parent = nullptr);

signals:
    void applyVaccine(Vaccine&);

private slots:
    void showWidget();                      //show widget in mode "add new data"
    void showEditWidget(QModelIndex);       //show widget in mode "correct data"
    void addVaccine();                      //add vaccine to repository
};

#endif // ADD_VACCINE_WIDGET_H
