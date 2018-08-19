#include "child_model.h"

ChildModel::ChildModel(QObject *parent) : QAbstractTableModel(parent)
{
    vaccines = new QList<Vaccine>;
}

ChildModel::ChildModel(QString _first_name, QString _middle_name, QString _last_name, QDate _birthday):
    first_name(_first_name), middle_name(_middle_name), last_name(_last_name), birthday(_birthday)
{
    vaccines = new QList<Vaccine>;
}

ChildModel::ChildModel(ChildModel const &child)
{
    first_name = child.first_name;
    middle_name = child.middle_name;
    last_name = child.last_name;
    birthday = child.birthday;
    vaccines = new QList<Vaccine>;
    for(Vaccine vaccine : *child.vaccines)
        vaccines->push_back(vaccine);
}

ChildModel& ChildModel::operator=(ChildModel const &child)
{
    if(this == &child)
        return *this;
    first_name = child.first_name;
    middle_name = child.middle_name;
    last_name = child.last_name;
    birthday = child.birthday;
    if(vaccines){
        if(!vaccines->isEmpty())
            vaccines->clear();
        delete vaccines;
    }
    vaccines = new QList<Vaccine>;
    for(Vaccine vaccine : *child.vaccines)
        vaccines->push_back(vaccine);
    return *this;
}

ChildModel::~ChildModel()
{
    if(vaccines){
        if(!vaccines->isEmpty())
            vaccines->clear();
        delete vaccines;
    }
}

QString ChildModel::getFirstName() const
{
    return first_name;
}

void ChildModel::setFirstName(QString const &value)
{
    first_name = value;
}

QString ChildModel::getMiddleName() const
{
    return middle_name;
}

void ChildModel::setMiddleName(QString const &value)
{
    middle_name = value;
}

QString ChildModel::getLastName() const
{
    return last_name;
}

void ChildModel::setLastName(QString const &value)
{
    last_name = value;
}

QDate ChildModel::getBirthday() const
{
    return birthday;
}

void ChildModel::setBirthday(QDate const &value)
{
    birthday = value;
}

int ChildModel::rowCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return vaccines->count();
}

int ChildModel::columnCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return 6;
}

QVariant ChildModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section){
            case 0:
                return "Название";
            case 1:
                return "Торговое название";
            case 2:
                return "Серия";
            case 3:
                return "Страна изготовитель";
            case 4:
                return "Срок годности";
            case 5:
                return "Дата проведения";
            default:
                return QString("Column %1").arg(section + 1);
            }
    return QVariant();
}

QVariant ChildModel::data(QModelIndex const &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch(role){
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column()){
        case 0:
            return vaccines->at(index.row()).getName();
        case 1:
            return vaccines->at(index.row()).getTradeName();
        case 2:
            return vaccines->at(index.row()).getSerial();
        case 3:
            return vaccines->at(index.row()).getCountry();
        case 4:
            return vaccines->at(index.row()).getBestBefore();
        case 5:
            return vaccines->at(index.row()).getVaccinationDate();
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }
}

bool ChildModel::setData(QModelIndex const &index, QVariant const &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::EditRole)
        switch(index.column()){
        case 0:
            const_cast<Vaccine&>(vaccines->at(index.row())).setName(value.toString());
            break;
        case 1:
            const_cast<Vaccine&>(vaccines->at(index.row())).setTradeName(value.toString());
            break;
        case 2:
            const_cast<Vaccine&>(vaccines->at(index.row())).setSerial(value.toString());
            break;
        case 3:
            const_cast<Vaccine&>(vaccines->at(index.row())).setCountry(value.toString());
            break;
        case 4:
            const_cast<Vaccine&>(vaccines->at(index.row())).setBestBefore(value.toDate());
            break;
        case 5:
            const_cast<Vaccine&>(vaccines->at(index.row())).setVaccinationDate(value.toDate());
            break;
        }
    return true;
}

bool ChildModel::removeRows(int position, int rows, QModelIndex const &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row{0}; row < rows; ++row)
        vaccines->removeAt(position);
    endRemoveRows();
    return true;
}

Qt::ItemFlags ChildModel::flags(QModelIndex const &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ChildModel::insertVaccine(QString const &name, QString const &trade_name, QString const &serial,
                               QString const &country, QDate const &best_before, QDate const &date)
{
    int row{rowCount()};
    Vaccine vaccine;
    vaccine.setName(name);
    vaccine.setTradeName(trade_name);
    vaccine.setSerial(serial);
    vaccine.setCountry(country);
    vaccine.setBestBefore(best_before);
    vaccine.setVaccinationDate(date);
    beginInsertRows(QModelIndex(), row, row);
    vaccines->push_back(vaccine);
    endInsertRows();
}

QList<Vaccine>* ChildModel::getDataList() const
{
    return vaccines;
}
