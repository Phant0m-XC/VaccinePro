#include "add_vaccine_widget.h"

AddVaccineWidget::AddVaccineWidget(SettingsModel *settings_model,
                                   QWidget *parent): QDialog(parent)
{
    QLabel *lbl_name{new QLabel("Название:")};
    QLabel *lbl_trade_name{new QLabel("Торговое название:")};
    QLabel *lbl_serial{new QLabel("Серия:")};
    QLabel *lbl_country{new QLabel("Страна изготовитель:")};
    QLabel *lbl_best_before{new QLabel("Срок годности:")};

    mapper = new QDataWidgetMapper(this);       //show data in appropriate widget
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    comb_name = new QComboBox;
    comb_name->setModel(settings_model);
    comb_name->setCurrentIndex(0);
    txt_trade_name = new QLineEdit;
    txt_serial = new QLineEdit;
    txt_country = new QLineEdit;
    cal_best_before = new QCalendarWidget;

    lbl_name->setBuddy(comb_name);
    lbl_trade_name->setBuddy(txt_trade_name);
    lbl_serial->setBuddy(txt_serial);
    lbl_country->setBuddy(txt_country);
    lbl_best_before->setBuddy(cal_best_before);

    QGridLayout *grid_lay{new QGridLayout};

    grid_lay->addWidget(lbl_name, 0, 0);
    grid_lay->addWidget(comb_name, 1, 0);
    grid_lay->addWidget(lbl_trade_name, 2, 0);
    grid_lay->addWidget(txt_trade_name, 3, 0);
    grid_lay->addWidget(lbl_serial, 4, 0);
    grid_lay->addWidget(txt_serial, 5, 0);
    grid_lay->addWidget(lbl_country, 6, 0);
    grid_lay->addWidget(txt_country, 7, 0);
    grid_lay->addWidget(lbl_best_before, 0, 1, 1, 1);
    grid_lay->addWidget(cal_best_before, 1, 1, 7, 1);

    QPushButton *apply{new QPushButton("Сохранить")};
    QPushButton *cancel{new QPushButton("Отмена")};

    QHBoxLayout *h_lay{new QHBoxLayout};
    h_lay->addWidget(apply);
    h_lay->setAlignment(apply, Qt::AlignLeft);
    h_lay->addWidget(cancel);
    h_lay->setAlignment(cancel, Qt::AlignRight);

    grid_lay->addLayout(h_lay, 8, 0, 1, 2);
    setLayout(grid_lay);

    setModal(true);
    setFixedSize(QSize(500, 253));

    connect(apply, SIGNAL(clicked()), SLOT(addVaccine()));
    connect(cancel, SIGNAL(clicked()), SLOT(reject()));

    isEditMode = false;
}

void AddVaccineWidget::showWidget()
{
    setWindowTitle("Добавление вакцины");
    isEditMode = false;
    comb_name->setCurrentIndex(0);
    txt_trade_name->clear();
    txt_serial->clear();
    txt_country->clear();
    cal_best_before->setSelectedDate(QDate::currentDate());
    show();
}

void AddVaccineWidget::showEditWidget(QModelIndex index)
{
    setWindowTitle("Редактирование данных вакцины");
    isEditMode = true;
    mapper->setModel(const_cast<QAbstractItemModel*>(index.model()));
    mapper->addMapping(comb_name, 0);
    mapper->addMapping(txt_trade_name, 1);
    mapper->addMapping(txt_serial, 2);
    mapper->addMapping(txt_country, 3);
    mapper->addMapping(cal_best_before, 4, "selectedDate");
    mapper->setCurrentModelIndex(index);
    show();
}

void AddVaccineWidget::addVaccine()
{
    if(isEditMode)
        mapper->submit();
    else{
        Vaccine vaccine(comb_name->currentText(),
                        txt_trade_name->text(),
                        txt_serial->text(),
                        txt_country->text(),
                        cal_best_before->selectedDate());
        emit applyVaccine(vaccine);
    }
    hide();
}
