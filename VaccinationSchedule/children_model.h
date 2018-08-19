#ifndef CHILDREN_MODEL_H
#define CHILDREN_MODEL_H

#include "child_model.h"
#include <QAbstractTableModel>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QDate>

class ChildrenModel : public QAbstractTableModel
{
    Q_OBJECT

    QFile *file_children;
    QList<ChildModel> *children;

public:
    explicit ChildrenModel(QObject *parent = nullptr);
    ChildrenModel(ChildrenModel const&) = delete;
    ChildrenModel(ChildrenModel&&) = delete;
    ChildrenModel& operator=(ChildrenModel const&) = delete;
    virtual ~ChildrenModel();

    //return count of childrens
    int rowCount(QModelIndex const &parent = QModelIndex()) const;

    //return count of columns for table (matches child fields)
    int columnCount(QModelIndex const &parent = QModelIndex()) const;

    //headers for table
    QVariant headerData(int, Qt::Orientation, int) const;

    //return data for each field of child
    QVariant data(QModelIndex const&, int role = Qt::DisplayRole) const;

    //set data for each field of child
    bool setData(QModelIndex const&, QVariant const&, int role = Qt::EditRole);

    //remove child from children repository
    bool removeRows(int, int, QModelIndex const&);

    //return itemflags for table view
    Qt::ItemFlags flags(QModelIndex const&) const;

    //insert child to children repository
    void insertData(ChildModel&);

    //get children repository
    QList<ChildModel>* getDataList() const;

    //get child by index
    ChildModel const* getModel(int);

private:
    //parse xml domdocmodel and fill field each child
    void fillChildren(QDomDocument const&);

private slots:
    //load data from file
    void loadData();

    //save data to file
    void saveChildren() const;

};

#endif // CHILDREN_MODEL_H
