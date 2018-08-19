#include "qdateedit_delegate.h"

QDateEditDelegate::QDateEditDelegate(QObject *parent): QStyledItemDelegate(parent)
{

}

QWidget* QDateEditDelegate::createEditor(QWidget *parent, QStyleOptionViewItem const &option, QModelIndex const &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QDateEdit *editor{new QDateEdit(parent)};
    return editor;
}

void QDateEditDelegate::setEditorData(QWidget *editor, QModelIndex const &index) const
{
    QDateEdit *date_edit{static_cast<QDateEdit*>(editor)};
    date_edit->setDate(index.model()->data(index).toDate());
}

void QDateEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, QModelIndex const &index) const
{
    QDateEdit *date_edit{static_cast<QDateEdit*>(editor)};
    model->setData(index, date_edit->date());
}
