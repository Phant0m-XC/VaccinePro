#ifndef PROXYSEARCHMODEL_H
#define PROXYSEARCHMODEL_H

#include <QSortFilterProxyModel>

/*Proxy Model for search or sort in model*/

class ProxySearchModel: public QSortFilterProxyModel
{
    Q_OBJECT

    QRegExp last_name_reg_exp;
    QRegExp first_name_reg_exp;
    QRegExp middle_name_reg_exp;

public:
    explicit ProxySearchModel(QObject *parent = nullptr);

    //filter row (accept or not)
    bool filterAcceptsRow(int, QModelIndex const&) const;
    QVariant headerData(int, Qt::Orientation, int) const;

public slots:
    void setLastNameFilter(QString const&);     //set regexp pattern for last name
    void setFirstNameFilter(QString const&);    //set regexp pattern for first name
    void setMiddleNameFilter(QString const&);   //set regexp pattern for middle name

};

#endif // PROXYSEARCHMODEL_H
