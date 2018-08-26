#include "vaccines_model.h"

VaccinesModel::VaccinesModel(QObject *parent): QAbstractTableModel(parent)
{
    vaccines_repository = new QList<Vaccine>;
    file_vaccines_repository = new QFile(QString("vaccines.xml"));
}

VaccinesModel::~VaccinesModel()
{
    if(vaccines_repository) {
        if(!vaccines_repository->isEmpty())
            vaccines_repository->clear();
        delete vaccines_repository;
    }
    if(file_vaccines_repository->isOpen())
        file_vaccines_repository->close();
    if(file_vaccines_repository)
        delete file_vaccines_repository;
}

int VaccinesModel::rowCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return vaccines_repository->count();
}

int VaccinesModel::columnCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

QVariant VaccinesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section) {
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
            default:
                return QString("Column %1").arg(section + 1);
            }
    return QVariant();
}

QVariant VaccinesModel::data(QModelIndex const &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch(role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column()) {
        case 0:
            return vaccines_repository->at(index.row()).getName();
        case 1:
            return vaccines_repository->at(index.row()).getTradeName();
        case 2:
            return vaccines_repository->at(index.row()).getSerial();
        case 3:
            return vaccines_repository->at(index.row()).getCountry();
        case 4:
            return vaccines_repository->at(index.row()).getBestBefore();
        default:
            return QVariant();
        }
    case Qt::UserRole:
        return QVariantList() << vaccines_repository->at(index.row()).getName() <<
                                 vaccines_repository->at(index.row()).getTradeName() <<
                                 vaccines_repository->at(index.row()).getSerial() <<
                                 vaccines_repository->at(index.row()).getCountry() <<
                                 vaccines_repository->at(index.row()).getBestBefore().toString("dd.MM.yyyy");
    default:
        return QVariant();
    }
}

bool VaccinesModel::setData(QModelIndex const &index, QVariant const &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::EditRole)
        switch(index.column()) {
        case 0:
            const_cast<Vaccine &>(vaccines_repository->at(index.row())).setName(value.toString());
            break;
        case 1:
            const_cast<Vaccine &>(vaccines_repository->at(index.row())).setTradeName(value.toString());
            break;
        case 2:
            const_cast<Vaccine &>(vaccines_repository->at(index.row())).setSerial(value.toString());
            break;
        case 3:
            const_cast<Vaccine &>(vaccines_repository->at(index.row())).setCountry(value.toString());
            break;
        case 4:
            const_cast<Vaccine &>(vaccines_repository->at(index.row())).setBestBefore(value.toDate());
            break;
        }
    return true;
}

bool VaccinesModel::removeRows(int position, int rows, QModelIndex const &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row{0}; row < rows; ++row)
        vaccines_repository->removeAt(position);
    endRemoveRows();
    return true;
}

Qt::ItemFlags VaccinesModel::flags(QModelIndex const &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void VaccinesModel::insertData(Vaccine &vaccine)
{
    int row{rowCount()};
    beginInsertRows(QModelIndex(), row, row);
    vaccines_repository->push_back(vaccine);
    endInsertRows();
}

void VaccinesModel::fillVaccinesRepository(QDomDocument const &dom_doc)
{
    QDomElement dom_element{dom_doc.documentElement()};
    QDomNode node{dom_element.firstChild()};
    while(!node.isNull()) {
        Vaccine vaccine;
        if(node.isElement())
            dom_element = node.toElement();
        vaccine.setName(dom_element.attribute("name"));
        dom_element = dom_element.firstChild().toElement();
        vaccine.setTradeName(dom_element.text());
        vaccine.setSerial(dom_element.nextSiblingElement("serial").text());
        vaccine.setCountry(dom_element.nextSiblingElement("country").text());
        vaccine.setBestBefore(QDate::fromString(dom_element.nextSiblingElement("best_before").text(), "yyyy-MM-dd"));
        node = node.nextSibling();
        vaccines_repository->push_back(vaccine);
    }
}

void VaccinesModel::loadData()
{
    QDomDocument dom_doc;
    if(file_vaccines_repository->open(QIODevice::ReadOnly)) {
        dom_doc.setContent(file_vaccines_repository);
        fillVaccinesRepository(dom_doc);
        file_vaccines_repository->close();
    }
}

void VaccinesModel::saveVaccines() const
{
    QDomDocument doc("vaccines");
    QDomElement dom_root{doc.createElement("vaccines")};
    for(Vaccine vaccine : *vaccines_repository) {
        QDomElement dom_element{doc.createElement("vaccine")};
        dom_element.setAttribute("name", vaccine.getName());

        QDomElement trade_name{doc.createElement("trade_name")};
        QDomText trade_name_value{doc.createTextNode(vaccine.getTradeName())};
        trade_name.appendChild(trade_name_value);

        QDomElement serial{doc.createElement("serial")};
        QDomText serial_value{doc.createTextNode(vaccine.getSerial())};
        serial.appendChild(serial_value);

        QDomElement country{doc.createElement("country")};
        QDomText country_value{doc.createTextNode(vaccine.getCountry())};
        country.appendChild(country_value);

        QDomElement best_before{doc.createElement("best_before")};
        QDomText best_before_value{doc.createTextNode(vaccine.getBestBefore().toString("yyyy-MM-dd"))};
        best_before.appendChild(best_before_value);

        dom_element.appendChild(trade_name);
        dom_element.appendChild(serial);
        dom_element.appendChild(country);
        dom_element.appendChild(best_before);

        dom_root.appendChild(dom_element);
    }
    doc.appendChild(dom_root);

    if(file_vaccines_repository->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream text_stream(file_vaccines_repository);
        text_stream.setCodec("UTF-8");
        text_stream << doc.toString();
        file_vaccines_repository->close();
    }
}
