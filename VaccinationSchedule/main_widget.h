#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include "vaccines_settings_widget.h"
#include "add_child_widget.h"
#include "add_vaccine_widget.h"
#include "registry_widget.h"
#include "vaccines_widget.h"
#include "schedule_widget.h"
#include "qdateedit_delegate.h"
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QVBoxLayout>

/*Main Widget*/

class MainWidget: public QWidget
{
    Q_OBJECT

    ChildrenModel *children_model;
    VaccinesModel *vaccines_model;
    SettingsModel *settings_model;
    ScheduleModel *schedule_model;

    RegistryWidget *registry_widget;
    VaccinesWidget *vaccines_widget;
    ScheduleWidget *schedule_widget;
    VaccinesSettingsWidget *vaccine_settings_widget;
    AddChildWidget *add_child_widget;
    AddVaccineWidget *add_vaccine_widget;

    QMenu *menu_file;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    //set application style
    void setAppStyle();

signals:
    //start load data in models
    void startLoad();

private slots:
    //change enamble/disable menu
    void itemMenuSwitch(int);

    //show about :)
    void help() const;

    //show dialog box "save done"
    void saveData() const;

};

#endif // MAIN_WIDGET_H
