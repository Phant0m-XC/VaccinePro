#include "proxysearchmodel.h"

ProxySearchModel::ProxySearchModel(QObject *parent): QSortFilterProxyModel(parent)
{
    last_name_reg_exp.setCaseSensitivity(Qt::CaseInsensitive);
    first_name_reg_exp.setCaseSensitivity(Qt::CaseInsensitive);
    middle_name_reg_exp.setCaseSensitivity(Qt::CaseInsensitive);
    last_name_reg_exp.setPatternSyntax(QRegExp::RegExp);
    first_name_reg_exp.setPatternSyntax(QRegExp::RegExp);
    middle_name_reg_exp.setPatternSyntax(QRegExp::RegExp);
}

bool ProxySearchModel::filterAcceptsRow(int source_row, QModelIndex const &source_parent) const
{

    QModelIndex last_name_index{sourceModel()->index(source_row, 0, source_parent)};
    QModelIndex first_name_index{sourceModel()->index(source_row, 1, source_parent)};
    QModelIndex middle_name_index{sourceModel()->index(source_row, 2, source_parent)};

    QString last_name{sourceModel()->data(last_name_index).toString()};
    QString first_name{sourceModel()->data(first_name_index).toString()};
    QString middle_name{sourceModel()->data(middle_name_index).toString()};

    return (last_name.contains(last_name_reg_exp) && first_name.contains(first_name_reg_exp) && middle_name.contains(middle_name_reg_exp));
}

QVariant ProxySearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return sourceModel()->headerData(section, orientation, role);
}

void ProxySearchModel::setLastNameFilter(QString const &reg_exp)
{
    last_name_reg_exp.setPattern(reg_exp);
    invalidateFilter();
}

void ProxySearchModel::setFirstNameFilter(QString const &reg_exp)
{
    first_name_reg_exp.setPattern(reg_exp);
    invalidateFilter();
}

void ProxySearchModel::setMiddleNameFilter(QString const &reg_exp)
{
    middle_name_reg_exp.setPattern(reg_exp);
    invalidateFilter();
}
