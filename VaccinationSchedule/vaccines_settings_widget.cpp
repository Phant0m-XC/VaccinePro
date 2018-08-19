#include "vaccines_settings_widget.h"

VaccinesSettingsWidget::VaccinesSettingsWidget(QWidget *parent): QDialog(parent)
{
    QLabel *lbl_name{new QLabel("Название")};
    QLabel *lbl_mult{new QLabel("Сроки введения(мес.)")};
    txt_name = new QLineEdit;
    txt_mult = new QLineEdit;
    QRegExp rx("[0-9,]*");                                      //let enter only numbers and commas
    txt_mult->setValidator(new QRegExpValidator(rx, this));     //
    lbl_name->setBuddy(txt_name);
    lbl_mult->setBuddy(txt_mult);

    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    QLineEditDelegate *delegate = new QLineEditDelegate(this);
    table->setItemDelegateForColumn(1, delegate);

    QGridLayout *grid_lay{new QGridLayout};
    grid_lay->addWidget(lbl_name, 0, 0);
    grid_lay->addWidget(txt_name, 1, 0);
    grid_lay->addWidget(lbl_mult, 2, 0);
    grid_lay->addWidget(txt_mult, 3, 0);
    grid_lay->addWidget(table, 4, 0, 5, 2);

    QPushButton *add{new QPushButton("Добавить")};
    QPushButton *remove{new QPushButton("Удалить")};
    QPushButton *save{new QPushButton("Сохранить")};
    QPushButton *cancel{new QPushButton("Отменить")};

    QHBoxLayout *h_lay{new QHBoxLayout};
    h_lay->addWidget(add);
    h_lay->addWidget(remove);
    h_lay->addWidget(save);
    h_lay->addWidget(cancel);

    grid_lay->addLayout(h_lay, 9, 0, 1, 2);
    setLayout(grid_lay);

    setModal(true);
    setFixedSize(QSize(340, 333));
    setWindowTitle("Календарь прививок");

    connect(add, SIGNAL(clicked()), SLOT(addItem()));
    connect(remove, SIGNAL(clicked()), SLOT(removeItem()));
    connect(save, SIGNAL(clicked()), SLOT(saveSettings()));
    connect(cancel, SIGNAL(clicked()), SLOT(reject()));
}

void VaccinesSettingsWidget::setModel(SettingsModel *value)
{
    model = value;
    table->setModel(model);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void VaccinesSettingsWidget::addItem()
{
    if(txt_name->text().isEmpty() || txt_mult->text().isEmpty()){
        QMessageBox message(QMessageBox::Information, "Внимание", "Заполните все поля");
        message.exec();
        return;
    }
    model->insertData(txt_name->text(), txt_mult->text());
    txt_name->clear();
    txt_mult->clear();
}

void VaccinesSettingsWidget::removeItem()
{
    QModelIndexList indexes{table->selectionModel()->selectedRows()};
    int count_rows{indexes.count()};
    for(int i{0}; i < count_rows; ++i)
        model->removeRows(indexes.at(i).row(), count_rows, QModelIndex());
}

void VaccinesSettingsWidget::saveSettings()
{
    emit accepted();
    hide();
}
