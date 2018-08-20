#ifndef QLINEEDIT_DELEGATE_H
#define QLINEEDIT_DELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>

/*lineEdit delegate for showing data in table*/

class QLineEditDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    QLineEditDelegate(QObject *parent = nullptr);
    QWidget * createEditor(QWidget *, QStyleOptionViewItem const &, QModelIndex const &) const;

    //put data in widget
    void setEditorData(QWidget *, QModelIndex const &) const;

    //save data to model
    void setModelData(QWidget *, QAbstractItemModel *, QModelIndex const &) const;
};

#endif // QLINEEDIT_DELEGATE_H
