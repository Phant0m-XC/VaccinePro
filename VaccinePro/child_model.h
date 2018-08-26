#ifndef CHILD_MODEL_H
#define CHILD_MODEL_H

#include "vaccine.h"
#include <QAbstractTableModel>
#include <QString>
#include <QDate>
#include <QList>

class ChildModel: public QAbstractTableModel
{
    Q_OBJECT

    QString first_name;                             //first name
    QString middle_name;                            //middle name
    QString last_name;                              //last name
    QDate birthday;                                 //birthday
    QList<Vaccine> *vaccines;                       //child's vaccines repository

public:
    explicit ChildModel(QObject *parent = nullptr);
    ChildModel(QString, QString, QString, QDate);
    //ChildModel(ChildModel const &) = delete;
    //ChildModel & operator=(ChildModel const &) = delete;
    virtual ~ChildModel();

    QString getFirstName() const;                   //getter, setter first name
    void setFirstName(QString const &);              //

    QString getMiddleName() const;                  //getter, setter middle name
    void setMiddleName(QString const &);             //

    QString getLastName() const;                    //getter, setter last name
    void setLastName(QString const &);               //

    QDate getBirthday() const;                      //getter, setter birthday
    void setBirthday(QDate const &);                 //

    //return count of vaccines
    int rowCount(QModelIndex const &parent = QModelIndex()) const;

    //return count of columns for table (matches vaccine fields)
    int columnCount(QModelIndex const &parent = QModelIndex()) const;

    //headers for table
    QVariant headerData(int, Qt::Orientation, int) const;

    //return data for each field of vaccine
    QVariant data(QModelIndex const &, int role = Qt::DisplayRole) const;

    //set data for each field of vaccine
    bool setData(QModelIndex const &, QVariant const&, int role = Qt::EditRole);

    //remove vaccine from child's vaccines repository
    bool removeRows(int, int, QModelIndex const &);

    //return itemflags for table view
    Qt::ItemFlags flags(QModelIndex const &) const;

    //insert vaccine in child's vaccines repository
    void insertVaccine(QString const &, QString const &, QString const &, QString const &, QDate const &, QDate const &);

    //get child's vaccines repository
    QList<Vaccine> * getDataList() const;
};

#endif // CHILD_MODEL_H
