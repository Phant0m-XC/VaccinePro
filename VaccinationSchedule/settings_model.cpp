#include "settings_model.h"

SettingsModel::SettingsModel(QObject *parent) : QAbstractTableModel(parent)
{
    vaccines_settings = new QList<Vaccine>;
    file_vaccines_settings = new QFile(QString("settings.xml"));
}

SettingsModel::~SettingsModel()
{
    if(vaccines_settings){
        if(!vaccines_settings->isEmpty())
            vaccines_settings->clear();
        delete vaccines_settings;
    }
    if(file_vaccines_settings->isOpen())
        file_vaccines_settings->close();
    if(file_vaccines_settings)
        delete file_vaccines_settings;
}

int SettingsModel::rowCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return vaccines_settings->count();
}

int SettingsModel::columnCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant SettingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section){
            case 0:
                return "Название";
            case 1:
                return "Сроки введения(мес.)";
            default:
                return QString("Column %1").arg(section + 1);
            }
    return QVariant();
}

QVariant SettingsModel::data(QModelIndex const &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch(role){
    case Qt::DisplayRole:
    case Qt::EditRole:
        if(index.column() == 0)
            return vaccines_settings->at(index.row()).getName();
        else if(index.column() == 1)
            return vaccines_settings->at(index.row()).getMult();
    default:
        return QVariant();
    }
}

bool SettingsModel::setData(QModelIndex const &index, QVariant const &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::EditRole)
        if(index.column() == 0)
            const_cast<Vaccine&>(vaccines_settings->at(index.row())).setName(value.toString());
        else
            if(index.column() == 1)
                const_cast<Vaccine&>(vaccines_settings->at(index.row())).setMult(value.toString());
    return true;
}

bool SettingsModel::removeRows(int position, int rows, QModelIndex const &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row{0}; row < rows; ++row)
        vaccines_settings->removeAt(position);
    endRemoveRows();
    return true;
}

Qt::ItemFlags SettingsModel::flags(QModelIndex const &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void SettingsModel::insertData(QString const &name, QString const &mult)
{
    int row{rowCount()};
    Vaccine vaccine;
    vaccine.setName(name);
    vaccine.setMult(mult);
    beginInsertRows(QModelIndex(), row, row);
    vaccines_settings->push_back(vaccine);
    endInsertRows();
}

QList<Vaccine>* SettingsModel::getDataList() const
{
    return vaccines_settings;
}

void SettingsModel::fillVaccinesSettings(QDomDocument const &dom_doc)
{
    QDomElement dom_element{dom_doc.documentElement()};
    QDomNode node{dom_element.firstChild()};
    while(!node.isNull()){
        Vaccine vaccine;
        QString mult;
        if(node.isElement())
            dom_element = node.toElement();
        vaccine.setName(dom_element.attribute("name"));
        dom_element = dom_element.firstChild().toElement();
        mult = dom_element.text();
        vaccine.setMult(mult);
        node = node.nextSibling();
        vaccines_settings->push_back(vaccine);
    }
}

void SettingsModel::loadData()
{
    QDomDocument dom_doc;
    if(file_vaccines_settings->open(QIODevice::ReadOnly)){
        dom_doc.setContent(file_vaccines_settings);
        fillVaccinesSettings(dom_doc);
        if(vaccines_settings->isEmpty())
            emit vaccinesSettingsEmpty();
        file_vaccines_settings->close();
    }
    else
        emit vaccinesSettingsEmpty();
}

void SettingsModel::saveVaccinesSettings() const
{
    QDomDocument doc("settings");
    QDomElement dom_root{doc.createElement("settings")};
    for(Vaccine vaccine : *vaccines_settings){
        QDomElement dom_element{doc.createElement("vaccine")};
        dom_element.setAttribute("name", vaccine.getName());
        QDomElement mult{doc.createElement("mult")};
        QDomText mult_value{doc.createTextNode(vaccine.getMult())};
        mult.appendChild(mult_value);
        dom_element.appendChild(mult);
        dom_root.appendChild(dom_element);
    }
    doc.appendChild(dom_root);

    if(file_vaccines_settings->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream text_stream(file_vaccines_settings);
        text_stream.setCodec("UTF-8");
        text_stream << doc.toString();
        file_vaccines_settings->close();
    }
}
