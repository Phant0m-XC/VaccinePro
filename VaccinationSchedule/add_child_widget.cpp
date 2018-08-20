#include "add_child_widget.h"

AddChildWidget::AddChildWidget(QDateEditDelegate *delegate,
                               VaccinesModel *vaccines_model,
                               SettingsModel *settings_model,
                               QWidget *parent): QDialog(parent)
{   
    QLabel *lbl_f_name{new QLabel("Имя:")};
    QLabel *lbl_m_name{new QLabel("Отчество:")};
    QLabel *lbl_l_name{new QLabel("Фамилия:")};
    QLabel *lbl_birthday{new QLabel("Дата рождения:")};
    txt_f_name = new QLineEdit;
    txt_m_name = new QLineEdit;
    txt_l_name = new QLineEdit;
    cal_birthday = new QCalendarWidget;

    lbl_f_name->setBuddy(txt_f_name);
    lbl_m_name->setBuddy(txt_m_name);
    lbl_l_name->setBuddy(txt_l_name);
    lbl_birthday->setBuddy(cal_birthday);

    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    mapper = new QDataWidgetMapper(this);           //show data in appropriate widget
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    delegate = new QDateEditDelegate(this);         //delegate for showing date in table
    table->setItemDelegateForColumn(4, delegate);
    table->setItemDelegateForColumn(5, delegate);

    QGridLayout *grid_main_lay{new QGridLayout};
    QVBoxLayout *name_vlay{new QVBoxLayout};
    QVBoxLayout *calendar_vlay{new QVBoxLayout};

    name_vlay->addWidget(lbl_l_name);
    name_vlay->addWidget(txt_l_name);
    name_vlay->addWidget(lbl_f_name);
    name_vlay->addWidget(txt_f_name);
    name_vlay->addWidget(lbl_m_name);
    name_vlay->addWidget(txt_m_name);
    name_vlay->addStretch();
    calendar_vlay->addWidget(lbl_birthday);
    calendar_vlay->addWidget(cal_birthday);
    calendar_vlay->addStretch();

    grid_main_lay->addLayout(name_vlay, 0, 0);
    grid_main_lay->addLayout(calendar_vlay, 0, 1);

    QGroupBox *grb_vaccine{new QGroupBox("Вакцина")};
    radio1 = new QRadioButton("Наличие:");                      //toggled if adding vaccine from repository
    radio2 = new QRadioButton("Ручной ввод:");                  //toggled if adding custom vaccine's data

    comb_vacc_repository = new QComboBox;
    comb_vacc_settings = new QComboBox;
    comb_vacc_repository->setModel(vaccines_model);
    QTableView *view{new QTableView(this)};                     //for showing vaccines from repository in combobox
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->horizontalHeader()->hide();
    view->verticalHeader()->hide();
    view->setShowGrid(false);
    comb_vacc_repository->setView(view);
    comb_vacc_settings->setModel(settings_model);

    lbl_name = new QLabel("Название:");
    lbl_trade_name = new QLabel("Торговое название:");
    lbl_serial = new QLabel("Серия:");
    lbl_country = new QLabel("Страна изготовитель:");
    lbl_best_before = new QLabel("Срок годности:");
    lbl_vaccination_date = new QLabel("Дата выполнения:");
    txt_trade_name = new QLineEdit;
    txt_serial = new QLineEdit;
    txt_country = new QLineEdit;
    best_before = new QDateEdit(QDate::currentDate());
    vaccination_date = new QDateEdit(QDate::currentDate());
    lbl_name->setBuddy(comb_vacc_settings);
    lbl_trade_name->setBuddy(txt_trade_name);
    lbl_serial->setBuddy(txt_serial);
    lbl_country->setBuddy(txt_country);
    lbl_best_before->setBuddy(best_before);
    lbl_vaccination_date->setBuddy(vaccination_date);

    QGridLayout *vaccine_lay{new QGridLayout};
    vaccine_lay->addWidget(radio1, 0, 0);
    vaccine_lay->addWidget(comb_vacc_repository, 0, 1, 1, 7);
    vaccine_lay->addWidget(radio2, 1, 0);
    vaccine_lay->addWidget(lbl_name, 1, 1);
    vaccine_lay->addWidget(lbl_trade_name, 1, 2);
    vaccine_lay->addWidget(lbl_serial, 1, 3);
    vaccine_lay->addWidget(lbl_country, 1, 4);
    vaccine_lay->addWidget(lbl_best_before, 1, 5);
    vaccine_lay->addWidget(lbl_vaccination_date, 1, 6);
    vaccine_lay->addWidget(comb_vacc_settings, 2, 1);
    vaccine_lay->addWidget(txt_trade_name, 2, 2);
    vaccine_lay->addWidget(txt_serial, 2, 3);
    vaccine_lay->addWidget(txt_country, 2, 4);
    vaccine_lay->addWidget(best_before, 2, 5);
    vaccine_lay->addWidget(vaccination_date, 2, 6);

    grb_vaccine->setLayout(vaccine_lay);

    QPushButton *add{new QPushButton("Добавить")};
    QPushButton *remove{new QPushButton("Удалить")};
    QPushButton *save{new QPushButton("Сохранить")};
    QPushButton *cancel{new QPushButton("Отмена")};

    QHBoxLayout *h_lay{new QHBoxLayout};
    h_lay->addWidget(add);
    h_lay->addWidget(remove);
    h_lay->addWidget(save);
    h_lay->addWidget(cancel);

    grid_main_lay->addWidget(grb_vaccine, 6, 0, 3, 2);
    grid_main_lay->addWidget(table, 9, 0, 5, 2);
    grid_main_lay->addLayout(h_lay, 14, 0, 1, 2);

    setLayout(grid_main_lay);

    setModal(true);
    setFixedSize(QSize(850, 560));

    connect(radio1, SIGNAL(toggled(bool)), SLOT(radioButtonChange(bool)));
    connect(radio2, SIGNAL(toggled(bool)), SLOT(radioButtonChange(bool)));
    connect(add, SIGNAL(clicked()), SLOT(addVaccine()));
    connect(remove, SIGNAL(clicked()), SLOT(removeVaccine()));
    connect(save, SIGNAL(clicked()), SLOT(addChild()));
    connect(cancel, SIGNAL(clicked()), SLOT(reject()));

    radio1->setChecked(true);
    isEditMode = false;
}

void AddChildWidget::showWidget()
{
    setWindowTitle("Добавление ребёнка");
    isEditMode = false;
    txt_l_name->clear();
    txt_f_name->clear();
    txt_m_name->clear();
    cal_birthday->setSelectedDate(QDate::currentDate());
    comb_vacc_repository->setCurrentIndex(0);
    comb_vacc_settings->setCurrentIndex(0);
    model = new ChildModel(this);               //if add new child create new model
    table->setModel(model);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    show();
}

void AddChildWidget::showEditWidget(QModelIndex index)
{
    setWindowTitle("Редактирование данных ребёнка");
    isEditMode = true;
    comb_vacc_repository->setCurrentIndex(0);
    comb_vacc_settings->setCurrentIndex(0);
    QAbstractItemModel *item_model{const_cast<QAbstractItemModel *>(index.model())};
    ProxySearchModel *proxy_model{static_cast<ProxySearchModel *>(item_model)};
    QModelIndex source_index{proxy_model->mapToSource(index)};
    ChildrenModel *children{static_cast<ChildrenModel *>(proxy_model->sourceModel())};
    int row{source_index.row()};
    ChildModel const *child{children->getModel(row)};
    model = const_cast<ChildModel *>(child);
    table->setModel(model);
    mapper->setModel(const_cast<QAbstractItemModel *>(source_index.model()));
    mapper->addMapping(txt_l_name, 0);
    mapper->addMapping(txt_f_name, 1);
    mapper->addMapping(txt_m_name, 2);
    mapper->addMapping(cal_birthday, 3, "selectedDate");
    mapper->setCurrentModelIndex(source_index);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    show();
}

void AddChildWidget::radioButtonChange(bool value)
{
    Q_UNUSED(value)
    if(radio1->isChecked()) {
        comb_vacc_repository->setEnabled(true);
        comb_vacc_settings->setEnabled(false);
        lbl_name->setEnabled(false);
        lbl_trade_name->setEnabled(false);
        lbl_serial->setEnabled(false);
        lbl_country->setEnabled(false);
        lbl_best_before->setEnabled(false);
        txt_trade_name->setEnabled(false);
        txt_serial->setEnabled(false);
        txt_country->setEnabled(false);
        best_before->setEnabled(false);
    }
    else
        if(radio2->isChecked()) {
            comb_vacc_repository->setEnabled(false);
            comb_vacc_settings->setEnabled(true);
            lbl_name->setEnabled(true);
            lbl_trade_name->setEnabled(true);
            lbl_serial->setEnabled(true);
            lbl_country->setEnabled(true);
            lbl_best_before->setEnabled(true);
            txt_trade_name->setEnabled(true);
            txt_serial->setEnabled(true);
            txt_country->setEnabled(true);
            best_before->setEnabled(true);
        }
}

void AddChildWidget::addVaccine()
{
    if(radio1->isChecked()) {
        QStringList list{comb_vacc_repository->currentData().toStringList()};
        QString name{list.at(0)};
        QString trade_name{list.at(1)};
        QString serial{list.at(2)};
        QString country{list.at(3)};
        QDate best_before_value{QDate::fromString(list.at(4), "dd.MM.yyyy")};
        QDate date_value{vaccination_date->date()};
        model->insertVaccine(name, trade_name, serial, country, best_before_value, date_value);
        comb_vacc_repository->setCurrentIndex(0);
    }
    else
        if(radio2->isChecked()) {
            QString name{comb_vacc_settings->currentText()};
            QString trade_name{txt_trade_name->text()};
            QString serial{txt_serial->text()};
            QString country{txt_country->text()};
            QDate best_before_value{best_before->date()};
            QDate date_value{vaccination_date->date()};
            model->insertVaccine(name, trade_name, serial, country, best_before_value, date_value);
            comb_vacc_settings->setCurrentIndex(0);
            txt_trade_name->clear();
            txt_serial->clear();
            txt_country->clear();
            best_before->setDate(QDate::currentDate());
            vaccination_date->setDate(QDate::currentDate());
        }
}

void AddChildWidget::removeVaccine()
{
    QModelIndexList indexes{table->selectionModel()->selectedRows()};
    int count_rows{indexes.count()};
    for(int i{0}; i < count_rows; ++i)
        model->removeRows(indexes.at(i).row(), count_rows, QModelIndex());
}

void AddChildWidget::addChild()
{
    if(txt_l_name->text().isEmpty() || txt_f_name->text().isEmpty()) {
        QMessageBox message(QMessageBox::Information, "Внимание", "Заполните поля \"Фамилия\" и \"Имя\"");
        message.exec();
        return;
    }
    if(isEditMode)
        mapper->submit();
    else {
        model->setLastName(txt_l_name->text());
        model->setFirstName(txt_f_name->text());
        model->setMiddleName(txt_m_name->text());
        model->setBirthday(cal_birthday->selectedDate());
        emit applyChild(*model);
    }
    hide();
}
