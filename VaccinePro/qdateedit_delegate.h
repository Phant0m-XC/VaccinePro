#ifndef QDATEEDIT_DELEGATE_H
#define QDATEEDIT_DELEGATE_H

#include "child_model.h"
#include <QStyledItemDelegate>
#include <QDateEdit>

/*dateEdit delegate for showing date in table*/

class QDateEditDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QDateEditDelegate(QObject *parent = nullptr);
    QWidget * createEditor(QWidget *, QStyleOptionViewItem const &, QModelIndex const &) const;

    //put data in widget
    void setEditorData(QWidget *, QModelIndex const &) const;

    //save data to model
    void setModelData(QWidget *, QAbstractItemModel *, QModelIndex const &) const;
};

#endif // QDATEEDIT_DELEGATE_H
