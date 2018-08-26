#ifndef VACCINES_MODEL_H
#define VACCINES_MODEL_H

#include "vaccine.h"
#include <QAbstractTableModel>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

class VaccinesModel: public QAbstractTableModel
{
    Q_OBJECT

    QFile *file_vaccines_repository;
    QList<Vaccine> *vaccines_repository;

public:
    explicit VaccinesModel(QObject *parent = nullptr);
    VaccinesModel(VaccinesModel const &) = delete;
    VaccinesModel(VaccinesModel &&) = delete;
    VaccinesModel & operator=(VaccinesModel const &) = delete;
    virtual ~VaccinesModel();

    //return count of vaccines
    int rowCount(QModelIndex const &parent = QModelIndex()) const;

    //return count of columns for table (matches vaccine fields)
    int columnCount(QModelIndex const &parent = QModelIndex()) const;

    //headers for table
    QVariant headerData(int, Qt::Orientation, int) const;

    //return data for each field of vaccine
    QVariant data(QModelIndex const &, int role = Qt::DisplayRole) const;

    //set data for each field of vaccine
    bool setData(QModelIndex const &, QVariant const &, int role = Qt::EditRole);

    //remove vaccine from vaccines repository
    bool removeRows(int, int, QModelIndex const &);

    //return itemflags for table view
    Qt::ItemFlags flags(QModelIndex const &) const;

    //insert vaccine in vaccines repository
    void insertData(Vaccine &);

private:
    //parse xml domdocmodel and fill field each vaccine
    void fillVaccinesRepository(QDomDocument const &);

private slots:
    //load data from file
    void loadData();

    //save data to file
    void saveVaccines() const;
};

#endif // VACCINES_MODEL_H
