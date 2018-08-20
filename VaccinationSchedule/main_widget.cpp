#include "main_widget.h"

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
    QDateEditDelegate *delegate{new QDateEditDelegate(this)};

    children_model = new ChildrenModel(this);
    vaccines_model = new VaccinesModel(this);
    settings_model = new SettingsModel(this);
    schedule_model = new ScheduleModel(this);

    registry_widget = new RegistryWidget(this);
    vaccines_widget = new VaccinesWidget(this);
    schedule_widget = new ScheduleWidget(children_model, settings_model, this);

    vaccine_settings_widget = new VaccinesSettingsWidget;
    vaccine_settings_widget->setParent(this, Qt::Window);
    add_child_widget = new AddChildWidget(delegate, vaccines_model, settings_model);
    add_child_widget->setParent(this, Qt::Window);
    add_vaccine_widget = new AddVaccineWidget(settings_model);
    add_vaccine_widget->setParent(this, Qt::Window);

    QTabWidget *tab_widget{new QTabWidget(this)};
    tab_widget->addTab(registry_widget, "Регистратура");
    tab_widget->addTab(vaccines_widget, "Вакцина");
    tab_widget->addTab(schedule_widget, "Планирование");

    QMenuBar *bar{new QMenuBar};
    QMenu *menu_file{new QMenu("&Файл")};
    menu_file->addAction("&Печать...", registry_widget, SLOT(printDoc()), Qt::ALT + Qt::Key_P);
    menu_file->addAction("&Выход", this, SLOT(close()), Qt::ALT + Qt::Key_F4);
    QMenu *menu_settings{new QMenu("&Настройки")};
    menu_settings->addAction("&Вакцина...", vaccine_settings_widget, SLOT(show()));
    QMenu *menu_help{new QMenu("Помощь")};
    menu_help->addAction("&О приложении...", this, SLOT(help()));
    bar->addMenu(menu_file);
    bar->addMenu(menu_settings);
    bar->addMenu(menu_help);

    QVBoxLayout *v_layout{new QVBoxLayout};
    v_layout->addWidget(bar);
    v_layout->addWidget(tab_widget);
    setLayout(v_layout);

    connect(this, SIGNAL(startLoad()), children_model, SLOT(loadData()));
    connect(this, SIGNAL(startLoad()), vaccines_model, SLOT(loadData()));
    connect(this, SIGNAL(startLoad()), settings_model, SLOT(loadData()));
    connect(settings_model, SIGNAL(vaccinesSettingsEmpty()), vaccine_settings_widget, SLOT(show()));
    connect(vaccine_settings_widget, SIGNAL(accepted()), settings_model, SLOT(saveVaccinesSettings()));
    connect(registry_widget, SIGNAL(showAddChildWidget()), add_child_widget, SLOT(showWidget()));
    connect(registry_widget, SIGNAL(editChild(QModelIndex)), add_child_widget, SLOT(showEditWidget(QModelIndex)));
    connect(registry_widget, SIGNAL(saveData()), this, SLOT(saveData()));
    connect(registry_widget, SIGNAL(saveData()), schedule_widget, SLOT(calculate()));
    connect(add_child_widget, SIGNAL(applyChild(ChildModel &)), registry_widget, SLOT(addChild(ChildModel &)));
    connect(vaccines_widget, SIGNAL(showAddVaccineWidget()), add_vaccine_widget, SLOT(showWidget()));
    connect(vaccines_widget, SIGNAL(editVaccine(QModelIndex)), add_vaccine_widget, SLOT(showEditWidget(QModelIndex)));
    connect(vaccines_widget, SIGNAL(saveData()), this, SLOT(saveData()));
    connect(add_vaccine_widget, SIGNAL(applyVaccine(Vaccine &)), vaccines_widget, SLOT(addVaccine(Vaccine &)));

    emit startLoad();

    registry_widget->setModel(children_model);
    vaccines_widget->setModel(vaccines_model);
    vaccine_settings_widget->setModel(settings_model);
    schedule_widget->setModel(schedule_model);

    setAppStyle();
}

MainWidget::~MainWidget()
{

}

void MainWidget::setAppStyle()
{
    setWindowTitle("ВакцинаПро");
    setFixedSize(QSize(500, 550));
    setWindowIcon(QIcon(":icon.png"));
    QString style {
        "MainWidget {background: #ccf2ff;}"

        "QDialog {background: #ccf2ff;}"

        "QMenuBar {"
        "background-color: #ccf2ff;"
        "font: bold 12px;"
        "width: 80px;"
        "padding: 3px;"
        "}"

        "QMenuBar:item {"
        "background-color: #80dfff;"
        "border-style: outset;"
        "border-width: 1px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "font: bold 12px;"
        "min-width: 5em;"
        "padding: 3px;"
        "}"

        "QMenuBar:item:selected {"
        "background-color: #33ccff;"
        "border-style: inset;"
        "}"

        "QMenuBar:item:pressed {"
        "background-color: #00ace6;"
        "border-style: inset;"
        "}"

        "QPushButton {"
        "background-color: #80dfff;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "font: bold 12px;"
        "min-width: 5em;"
        "padding: 3px;"
        "}"

        "QPushButton:pressed {"
        "background-color: #00ace6;"
        "border-style: inset;"
        "}"

        "QLineEdit {"
        "background-color: white;"
        "border-style: outset;"
        "border-width: 1px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "font: 12px;"
        "min-width: 5em;"
        "padding: 3px;"
        "}"

        "QLineEdit:disabled {"
        "background-color: #d1d1e0;"
        "border-color: #676798;"
        "}"

        "QComboBox {"
        "background-color: white;"
        "border-style: outset;"
        "border-width: 1px;"
        "border-right-width: 0px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "font: 12px;"
        "min-width: 5em;"
        "padding: 3px;"
        "}"

        "QComboBox:disabled {"
        "background-color: #d1d1e0;"
        "border-color: #676798;"
        "}"

        "QComboBox:drop-down {"
        "subcontrol-origin: padding;"
        "subcontrol-position: top right;"
        "background-color: #80dfff;"
        "border-width: 1px;"
        "border-color: #006080;"
        "border-style: outset;"
        "border-top-right-radius: 10px;"
        "border-bottom-right-radius: 10px;"
        "}"

        "QComboBox:drop-down:pressed {"
        "background-color: #00ace6;"
        "border-style: inset;"
        "}"

        "QComboBox:drop-down:disabled {"
        "background-color: #a4a4c1;"
        "border-color: #676798;"
        "}"

        "QComboBox:down-arrow {"
        "image: url(:arrow-down-enable.png);"
        "margin: 0px 0px 0px -3px;"
        "}"

        "QComboBox:down-arrow:disabled {"
        "image: url(:arrow-down-disable.png);"
        "margin: 0px 0px 0px -3px;"
        "}"

        "QGroupBox {"
        "border-style: outset;"
        "border-width: 1px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "margin: 5px 0px 0px 0px"
        "}"

        "QGroupBox:title {"
        "subcontrol-origin: border;"
        "subcontrol-position: top center;"
        "margin: -13px 0px 0px 5px;"
        "}"

        "QTableView {"
        "border-style: solid;"
        "border-width: 1px;"
        "border-color: #006080;"
        "}"

        "QDateEdit {"
        "background-color: white;"
        "border-style: outset;"
        "border-width: 1px;"
        "border-right-width: 0px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "font: 12px;"
        "min-width: 5em;"
        "padding: 3px;"
        "}"

        "QDateEdit:disabled {"
        "background-color: #d1d1e0;"
        "border-color: #676798;"
        "}"

        "QDateEdit:up-button {"
        "subcontrol-origin: padding;"
        "subcontrol-position: top right;"
        "background-color: #80dfff;"
        "border-width: 1px;"
        "border-color: #006080;"
        "border-style: outset;"
        "border-top-right-radius: 10px;"
        "}"

        "QDateEdit:down-button {"
        "subcontrol-origin: padding;"
        "subcontrol-position: bottom right;"
        "background-color: #80dfff;"
        "border-width: 1px;"
        "border-color: #006080;"
        "border-style: outset;"
        "border-bottom-right-radius: 10px;"
        "}"

        "QDateEdit:up-button:pressed, QDateEdit:down-button:pressed {"
        "background-color: #00ace6;"
        "border-style: inset;"
        "}"

        "QDateEdit:up-button:disabled, QDateEdit:down-button:disabled {"
        "background-color: #a4a4c1;"
        "border-color: #676798;"
        "}"

        "QDateEdit:down-arrow {"
        "image: url(:arrow-down-enable.png);"
        "margin: 0px 0px 0px -3px;"
        "}"

        "QDateEdit:up-arrow {"
        "image: url(:arrow-up-enable.png);"
        "margin: 0px 0px 0px -3px;"
        "}"

        "QDateEdit:down-arrow:disabled {"
        "image: url(:arrow-down-disable.png);"
        "margin: 0px 0px 0px -3px;"
        "}"

        "QDateEdit:up-arrow:disabled {"
        "image: url(:arrow-up-disable.png);"
        "margin: 0px 0px 0px -3px;"
        "}"

        "QTabWidget:pane {"
        "background-color: #ccf2ff;"
        "border-style: outset;"
        "border-width: 1px;"
        "border-radius: 10px;"
        "border-color: #006080;"
        "font: 12px;"
        "min-width: 5em;"
        "padding: 3px;"
        "}"

        "QTabWidget:tab-bar {"
        "left: 8px;"
        "}"

        "QTabBar:tab {"
        "background-color: #00bfff;"
        "width: 95px;"
        "border-style: inset;"
        "border-width: 2px;"
        "border-bottom-width: 0px;"
        "border-color: #006080;"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "font: bold 12px;"
        "min-width: 5ex;"
        "padding: 3px;"
        "}"

        "QTabBar:tab:hover {"
        "border-style: outset;"
        "background-color: #4dd2ff;"
        "}"

        "QTabBar:tab:selected {"
        "border-style: outset;"
        "background-color: #80dfff;"
        "}"

        "QTabBar:tab:!selected {"
        "margin-top: 3px;"
        "}"

        "QScrollBar:horizontal, QScrollBar:vertical {"
        "background-color: #80dfff;"
        "}"
    };
    setStyleSheet(style);
}

void MainWidget::help() const
{
    //style of help dialog window
    QString style =
            "QMessageBox {background-color: #ccf2ff;}"
            "QPushButton {"
            "background-color: #80dfff;"
            "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 10px;"
            "border-color: #006080;"
            "font: bold 12px;"
            "min-width: 5em;"
            "padding: 3px;"
            "}"
            "QPushButton:pressed {"
            "background-color: #00ace6;"
            "border-style: inset;"
            "}";
    QMessageBox message(QMessageBox::Information, "О приложении", "Версия: 1.0");
    message.setWindowIcon(QIcon(":icon.png"));
    message.setStyleSheet(style);
    message.exec();
}

void MainWidget::saveData() const
{
    //style of save dialog window
    QString style =
            "QMessageBox {background-color: #ccf2ff;}"
            "QPushButton {"
            "background-color: #80dfff;"
            "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 10px;"
            "border-color: #006080;"
            "font: bold 12px;"
            "min-width: 5em;"
            "padding: 3px;"
            "}"
            "QPushButton:pressed {"
            "background-color: #00ace6;"
            "border-style: inset;"
            "}";
    QMessageBox message(QMessageBox::Information, "Сохранение", "Данные сохранены");
    message.setWindowIcon(QIcon(":icon.png"));
    message.setStyleSheet(style);
    message.exec();
}
