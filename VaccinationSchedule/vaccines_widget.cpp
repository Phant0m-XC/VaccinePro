#include "vaccines_widget.h"

VaccinesWidget::VaccinesWidget(QWidget *parent): QWidget(parent)
{
    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    QPushButton *add{new QPushButton("Добавить")};
    QPushButton *remove{new QPushButton("Удалить")};
    save = new QPushButton("Сохранить");

    QHBoxLayout *h_lay{new QHBoxLayout};
    QVBoxLayout *v_lay{new QVBoxLayout};

    h_lay->addWidget(add);
    h_lay->addWidget(remove);
    h_lay->addWidget(save);
    v_lay->addWidget(table);
    v_lay->addLayout(h_lay);

    setLayout(v_lay);

    connect(add, SIGNAL(clicked()), SIGNAL(showAddVaccineWidget()));
    connect(remove, SIGNAL(clicked()), SLOT(removeItem()));
    connect(table, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(editVaccine(QModelIndex)));
}

void VaccinesWidget::setModel(VaccinesModel *value)
{
    model = value;
    QSortFilterProxyModel *proxy_model{new QSortFilterProxyModel(this)};
    proxy_model->setSourceModel(model);
    table->setModel(proxy_model);
    table->setSortingEnabled(true);
    table->sortByColumn(0, Qt::AscendingOrder);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    connect(save, SIGNAL(clicked()), model, SLOT(saveVaccines()));
    connect(save, SIGNAL(clicked()), this, SIGNAL(saveData()));
}

void VaccinesWidget::removeItem()
{
    QModelIndexList indexes{table->selectionModel()->selectedRows()};
    int count_rows{indexes.count()};
    for(int i{0}; i < count_rows; ++i){
        QAbstractItemModel *item_model{const_cast<QAbstractItemModel*>(indexes.at(i).model())};
        QSortFilterProxyModel *proxy_model{static_cast<QSortFilterProxyModel*>(item_model)};
        QModelIndex source_index{proxy_model->mapToSource(indexes.at(i))};
        int row{source_index.row()};
        model->removeRows(row, count_rows, QModelIndex());
    }
}

void VaccinesWidget::addVaccine(Vaccine &vaccine)
{
    model->insertData(vaccine);
}
