#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "vaccine.h"
#include <QAbstractTableModel>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

class SettingsModel : public QAbstractTableModel
{
    Q_OBJECT

    QFile *file_vaccines_settings;
    QList<Vaccine> *vaccines_settings;

public:
    explicit SettingsModel(QObject *parent = nullptr);
    SettingsModel(SettingsModel const&) = delete;
    SettingsModel(SettingsModel&&) = delete;
    SettingsModel& operator=(SettingsModel const&) = delete;
    virtual ~SettingsModel();

    //return count of vaccines in settings
    int rowCount(QModelIndex const &parent = QModelIndex()) const;

    //return count of columns for table
    int columnCount(QModelIndex const &parent = QModelIndex()) const;

    //headers for table
    QVariant headerData(int, Qt::Orientation, int) const;

    //return data for item of table
    QVariant data(QModelIndex const&, int role = Qt::DisplayRole) const;

    //set data for field of vaccines settings
    bool setData(QModelIndex const&, QVariant const&, int role = Qt::EditRole);

    //remove vaccine from vaccines settings
    bool removeRows(int, int, QModelIndex const&);

    //return itemflags for table view
    Qt::ItemFlags flags(QModelIndex const&) const;

    //insert vaccine in vaccines settings
    void insertData(QString const&, QString const&);

    //get vaccines settings
    QList<Vaccine>* getDataList() const;

private:
    //parse xml domdocmodel and fill vaccines settings
    void fillVaccinesSettings(QDomDocument const&);

signals:
    //if settings empty
    void vaccinesSettingsEmpty();

private slots:
    //load data from file
    void loadData();

    //save data to file
    void saveVaccinesSettings() const;

};

#endif // SETTINGSCONTROLLER_H
