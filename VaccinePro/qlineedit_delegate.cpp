#include "qlineedit_delegate.h"

QLineEditDelegate::QLineEditDelegate(QObject *parent): QStyledItemDelegate(parent)
{

}

QWidget * QLineEditDelegate::createEditor(QWidget *parent, QStyleOptionViewItem const &option, QModelIndex const &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QLineEdit *editor{new QLineEdit(parent)};
    editor->setToolTip("Месяц через запятую");
    QRegExp rx("[0-9,]*");                                  //let enter only numbers and commas
    editor->setValidator(new QRegExpValidator(rx, parent)); //
    return editor;
}

void QLineEditDelegate::setEditorData(QWidget *editor, QModelIndex const &index) const
{
    QLineEdit *line_edit{static_cast<QLineEdit *>(editor)};
    line_edit->setText(index.model()->data(index).toString());
}

void QLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, QModelIndex const &index) const
{
    QLineEdit *line_edit{static_cast<QLineEdit *>(editor)};
    model->setData(index, line_edit->text());
}
