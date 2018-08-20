#ifndef REGISTRY_WIDGET_H
#define REGISTRY_WIDGET_H

#include "children_model.h"
#include "proxysearchmodel.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QPrintDialog>
#include <QMessageBox>

/*Widget for show children*/

class RegistryWidget: public QWidget
{
    Q_OBJECT

    QLineEdit *txt_search_l_name;   //search by last name
    QLineEdit *txt_search_f_name;   //search by first name
    QLineEdit *txt_search_m_name;   //search by middle name
    QPushButton *save;

    ChildrenModel *model;
    ProxySearchModel *proxy_model;  //for search and sort
    QTableView *table;

public:
    explicit RegistryWidget(QWidget *parent = nullptr);
    void setModel(ChildrenModel *);

signals:
    void showAddChildWidget();      //signal for show add child widget
    void editChild(QModelIndex);    //signal for show edit child widget
    void saveData();                //signal for show information dialog

private slots:
    void removeItem();              //remove selected row from table
    void addChild(ChildModel &);    //add child to children model
    void clearSearch();             //clear search fields
    void printDoc();                //print child's vaccines data

};

#endif // REGISTRY_WIDGET_H
