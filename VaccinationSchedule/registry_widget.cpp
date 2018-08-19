#include "registry_widget.h"

RegistryWidget::RegistryWidget(QWidget *parent): QWidget(parent)
{
    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    QGroupBox *search_gr_box{new QGroupBox("Поиск")};
    QLabel *lbl_search_l_name{new QLabel("Фамилия:")};
    QLabel *lbl_search_f_name{new QLabel("Имя:")};
    QLabel *lbl_search_m_name{new QLabel("Отчество:")};
    txt_search_l_name = new QLineEdit;
    txt_search_f_name = new QLineEdit;
    txt_search_m_name = new QLineEdit;
    lbl_search_l_name->setBuddy(txt_search_l_name);
    lbl_search_f_name->setBuddy(txt_search_f_name);
    lbl_search_m_name->setBuddy(txt_search_m_name);
    QPushButton *clear{new QPushButton("Очистить")};
    QGridLayout *search_lay{new QGridLayout};
    search_lay->addWidget(lbl_search_l_name, 0, 0);
    search_lay->addWidget(lbl_search_f_name, 1, 0);
    search_lay->addWidget(lbl_search_m_name, 2, 0);
    search_lay->addWidget(txt_search_l_name, 0, 1);
    search_lay->addWidget(txt_search_f_name, 1, 1);
    search_lay->addWidget(txt_search_m_name, 2, 1);
    QHBoxLayout *clear_lay{new QHBoxLayout};
    clear_lay->addWidget(clear);
    clear_lay->setAlignment(clear, Qt::AlignRight);
    search_lay->addLayout(clear_lay, 3, 1);
    search_gr_box->setLayout(search_lay);

    QPushButton *add{new QPushButton("Добавить")};
    QPushButton *remove{new QPushButton("Удалить")};
    save = new QPushButton("Сохранить");
    QPushButton *print{new QPushButton("Печать")};

    QHBoxLayout *h_button_lay{new QHBoxLayout};
    QVBoxLayout *v_lay{new QVBoxLayout};

    h_button_lay->addWidget(add);
    h_button_lay->addWidget(remove);
    h_button_lay->addWidget(save);
    h_button_lay->addWidget(print);
    v_lay->addWidget(search_gr_box);
    v_lay->addWidget(table);
    v_lay->addLayout(h_button_lay);

    setLayout(v_lay);

    connect(add, SIGNAL(clicked()), SIGNAL(showAddChildWidget()));
    connect(remove, SIGNAL(clicked()), SLOT(removeItem()));
    connect(clear, SIGNAL(clicked()), SLOT(clearSearch()));
    connect(print, SIGNAL(clicked()), SLOT(printDoc()));
    connect(table, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(editChild(QModelIndex)));
}

void RegistryWidget::setModel(ChildrenModel *value)
{
    model = value;
    proxy_model = new ProxySearchModel(this);
    proxy_model->setSourceModel(model);
    table->setModel(proxy_model);
    table->setSortingEnabled(true);
    table->sortByColumn(0, Qt::AscendingOrder);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    connect(save, SIGNAL(clicked()), model, SLOT(saveChildren()));
    connect(save, SIGNAL(clicked()), this, SIGNAL(saveData()));
    connect(txt_search_l_name, SIGNAL(textChanged(QString)), proxy_model, SLOT(setLastNameFilter(QString)));
    connect(txt_search_f_name, SIGNAL(textChanged(QString)), proxy_model, SLOT(setFirstNameFilter(QString)));
    connect(txt_search_m_name, SIGNAL(textChanged(QString)), proxy_model, SLOT(setMiddleNameFilter(QString)));
}

void RegistryWidget::removeItem()
{
    QModelIndexList indexes{table->selectionModel()->selectedRows()};
    int count_rows{indexes.count()};
    for(int i{0}; i < count_rows; ++i){
        QAbstractItemModel *item_model{const_cast<QAbstractItemModel*>(indexes.at(i).model())};
        ProxySearchModel *proxy_model{static_cast<ProxySearchModel*>(item_model)};
        QModelIndex source_index{proxy_model->mapToSource(indexes.at(i))};
        int row{source_index.row()};
        model->removeRows(row, count_rows, QModelIndex());
    }
}

void RegistryWidget::addChild(ChildModel &child)
{
    model->insertData(child);
}

void RegistryWidget::clearSearch()
{
    txt_search_l_name->clear();
    txt_search_f_name->clear();
    txt_search_m_name->clear();
}

void RegistryWidget::printDoc()
{
    QModelIndexList indexes{table->selectionModel()->selectedRows()}; //if row not selected show warning and return
    if(!indexes.count()){
        QString style =
                "QMessageBox{background-color: #ccf2ff;}"
                "QPushButton{"
                "background-color: #80dfff;"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: #006080;"
                "font: bold 12px;"
                "min-width: 5em;"
                "padding: 3px;"
                "}"
                "QPushButton:pressed{"
                "background-color: #00ace6;"
                "border-style: inset;"
                "}";
        QMessageBox message(QMessageBox::Information, "Внимание", "Не выбраны данные для печати");
        message.setWindowIcon(QIcon(":icon.png"));
        message.setStyleSheet(style);
        message.exec();
        return;
    }
    QAbstractItemModel *item_model{const_cast<QAbstractItemModel*>(indexes.at(0).model())};
    ProxySearchModel *proxy_model{static_cast<ProxySearchModel*>(item_model)};
    QModelIndex source_index{proxy_model->mapToSource(indexes.at(0))};
    ChildrenModel *children{static_cast<ChildrenModel*>(proxy_model->sourceModel())};
    ChildModel const *child{children->getModel(source_index.row())};
    /*preparing document in html format*/
    QString html =
            "<div align=right>" +
            QDate::currentDate().toString("dd.MM.yyyy") +
            "</div>"
            "<h2 align=center>Выписка из карты профпрививок</h2>"
            "<h3 align=center>" +
            child->getLastName() + " " + child->getFirstName() + " " + child->getMiddleName() + ", " +
            child->getBirthday().toString("dd.MM.yyyy") +
            "</h3>"
            "<table border=1 cellpadding=0 cellspacing=0 style=border-collapse:collapse align=center>"
            "<tr>"
            "<th>Вакцина</th>"
            "<th>Название</th>"
            "<th>Серия</th>"
            "<th>Страна производителя</th>"
            "<th>Срок годности</th>"
            "<th>Дата проведения</th>"
            "</tr>";
    QList<Vaccine> *vaccines{child->getDataList()};
    for(int i{0}; i < vaccines->count(); ++i){
        html.append("<tr>");

        html.append("<td>");
        html.append(vaccines->at(i).getName());
        html.append("</td>");

        html.append("<td>");
        html.append(vaccines->at(i).getTradeName());
        html.append("</td>");

        html.append("<td>");
        html.append(vaccines->at(i).getSerial());
        html.append("</td>");

        html.append("<td>");
        html.append(vaccines->at(i).getCountry());
        html.append("</td>");

        html.append("<td>");
        html.append(vaccines->at(i).getBestBefore().toString("dd.MM.yyyy"));
        html.append("</td>");

        html.append("<td>");
        html.append(vaccines->at(i).getVaccinationDate().toString("dd.MM.yyyy"));
        html.append("</td>");

        html.append("</tr>");
    }

    html.append("</table>");

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer;
    QPrintDialog *dialog{new QPrintDialog(&printer, this)}; //print dialog
    dialog->setWindowTitle("Print Document");
    if (dialog->exec() == QDialog::Accepted)
        document.print(&printer);
    delete dialog;
}
