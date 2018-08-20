#include "schedule_model.h"

ScheduleModel::ScheduleModel(QObject *parent): QAbstractTableModel(parent)
{
    schedule_child = new QList<ChildModel>;
    schedule_vaccine = new QList<Vaccine>;
}

ScheduleModel::~ScheduleModel()
{
    if(schedule_child) {
        if(!schedule_child->isEmpty())
            schedule_child->clear();
        delete schedule_child;
    }
    if(schedule_vaccine) {
        if(!schedule_vaccine->isEmpty())
            schedule_vaccine->clear();
        delete schedule_vaccine;
    }
}

int ScheduleModel::rowCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return schedule_child->count();
}

int ScheduleModel::columnCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant ScheduleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section) {
            case 0:
                return "Фамилия";
            case 1:
                return "Имя";
            case 2:
                return "Отчество";
            case 3:
                return "Вакцина";
            default:
                return QString("Column %1").arg(section + 1);
            }
    return QVariant();
}

QVariant ScheduleModel::data(QModelIndex const &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch(role) {
    case Qt::DisplayRole:
        switch(index.column()) {
        case 0:
            return schedule_child->at(index.row()).getLastName();
        case 1:
            return schedule_child->at(index.row()).getFirstName();
        case 2:
            return schedule_child->at(index.row()).getMiddleName();
        case 3:
            return schedule_vaccine->at(index.row()).getName();
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }
}

bool ScheduleModel::removeRows(int position, int rows, QModelIndex const &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row{0}; row < rows; ++row) {
        schedule_child->removeAt(position);
        schedule_vaccine->removeAt(position);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags ScheduleModel::flags(QModelIndex const &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ScheduleModel::calculateSchedule(QDate const &date, ChildrenModel *children_model, SettingsModel *settings_model)
{
    QList<ChildModel> *children{children_model->getDataList()};
    QList<Vaccine> *vaccines{settings_model->getDataList()};
    for(ChildModel const child : *children) {
        int month_age{calculateAge(date, child.getBirthday())};
        QList<Vaccine> *child_vaccines{child.getDataList()};
        for(Vaccine const vaccine : *vaccines) {
            QStringList mult_list{vaccine.getMult().split(QRegularExpression(","), QString::SplitBehavior::SkipEmptyParts)};
            int count{0};
            QDate max_date{QDate::fromString("01.01.1900", "dd.MM.yyyy")};
            for(Vaccine const child_vaccine : *child_vaccines) {
                if(vaccine.getName() == child_vaccine.getName())
                    ++count;
                if(max_date < child_vaccine.getVaccinationDate())
                    max_date = child_vaccine.getVaccinationDate();
            }
            if(mult_list.count() == count)
                continue;
            if(count < mult_list.count() && month_age >= mult_list[count].toInt()) {
                int row{rowCount()};
                beginInsertRows(QModelIndex(), row, row);
                schedule_child->push_back(child);
                schedule_vaccine->push_back(vaccine);
                endInsertRows();
            }
        }
    }
}

/*calculate age of child in months*/
int ScheduleModel::calculateAge(QDate const &selected_date, QDate const &birthday) const
{
    int years;
    int month;
    years = selected_date.year() - birthday.year();
    month = selected_date.month() - birthday.month();
    if(selected_date.month() < birthday.month()){
        --years;
        month += 12;
    }
    if(years)
        month += years * 12;
    if(selected_date.day() < birthday.day())
        --month;
    return month;
}
