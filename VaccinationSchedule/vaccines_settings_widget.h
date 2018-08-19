#ifndef VACCINES_SETTINGS_H
#define VACCINES_SETTINGS_H

#include "settings_model.h"
#include "qlineedit_delegate.h"
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QGridLayout>
#include <QHBoxLayout>

/*Widget for setting calendar of vaccination*/

class VaccinesSettingsWidget: public QDialog
{
    Q_OBJECT

    QLineEdit *txt_name;        //name of vaccine
    QLineEdit *txt_mult;        //multiplicity of vaccination

    SettingsModel *model;
    QTableView *table;

public:
    explicit VaccinesSettingsWidget(QWidget *parent = nullptr);
    void setModel(SettingsModel*);

private slots:
    void addItem();             //add vaccine to vaccines settings repository
    void removeItem();          //remove selected row from table
    void saveSettings();        //save vaccine (emit save signal)
};

#endif // VACCINES_SETTINGS_H
