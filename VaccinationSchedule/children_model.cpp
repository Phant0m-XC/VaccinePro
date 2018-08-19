#include "children_model.h"

ChildrenModel::ChildrenModel(QObject *parent): QAbstractTableModel(parent)
{
    children = new QList<ChildModel>;
    file_children = new QFile(QString("children.xml"));
}

ChildrenModel::~ChildrenModel()
{
    if(children){
        if(!children->isEmpty())
            children->clear();
        delete children;
    }
    if(file_children->isOpen())
        file_children->close();
    if(file_children)
        delete file_children;
}

int ChildrenModel::rowCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return children->count();
}

int ChildrenModel::columnCount(QModelIndex const &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant ChildrenModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section){
            case 0:
                return "Фамилия";
            case 1:
                return "Имя";
            case 2:
                return "Отчество";
            case 3:
                return "Дата рождения";
            default:
                return QString("Column %1").arg(section + 1);
            }
    return QVariant();
}

QVariant ChildrenModel::data(QModelIndex const &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch(role){
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column()){
        case 0:
            return children->at(index.row()).getLastName();
        case 1:
            return children->at(index.row()).getFirstName();
        case 2:
            return children->at(index.row()).getMiddleName();
        case 3:
            return children->at(index.row()).getBirthday();
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }
}

bool ChildrenModel::setData(QModelIndex const &index, QVariant const &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::EditRole)
        switch(index.column()){
        case 0:
            const_cast<ChildModel&>(children->at(index.row())).setLastName(value.toString());
            break;
        case 1:
            const_cast<ChildModel&>(children->at(index.row())).setFirstName(value.toString());
            break;
        case 2:
            const_cast<ChildModel&>(children->at(index.row())).setMiddleName(value.toString());
            break;
        case 3:
            const_cast<ChildModel&>(children->at(index.row())).setBirthday(value.toDate());
            break;
        }
    return true;
}

bool ChildrenModel::removeRows(int position, int rows, QModelIndex const &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row{0}; row < rows; ++row)
        children->removeAt(position);
    endRemoveRows();
    return true;
}

Qt::ItemFlags ChildrenModel::flags(QModelIndex const &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ChildrenModel::insertData(ChildModel &child)
{
    int row{rowCount()};
    beginInsertRows(QModelIndex(), row, row);
    children->push_back(child);
    endInsertRows();
}

QList<ChildModel>* ChildrenModel::getDataList() const
{
    return children;
}

ChildModel const* ChildrenModel::getModel(int row)
{
    return &children->at(row);
}

void ChildrenModel::fillChildren(QDomDocument const &dom_doc)
{
    QDomElement dom_element{dom_doc.documentElement()};
    QDomNode child_node{dom_element.firstChild()};
    while(!child_node.isNull()){
        ChildModel child;
        if(child_node.isElement())
            dom_element = child_node.toElement();
        child.setLastName(dom_element.attribute("last_name"));
        child.setFirstName(dom_element.attribute("first_name"));
        child.setMiddleName(dom_element.attribute("middle_name"));
        QDomNode inner_node{dom_element.firstChild()};
        if(inner_node.isElement())
            dom_element = inner_node.toElement();
        child.setBirthday(QDate::fromString(dom_element.text(), "yyyy-MM-dd"));
        inner_node = dom_element.nextSibling();
        if(inner_node.isElement())
            dom_element = inner_node.toElement();
        while(dom_element.tagName() == "vaccine"){
            QString name{dom_element.attribute("name")};
            QDomNode vaccine_node{dom_element.firstChild()};
            if(vaccine_node.isElement())
                dom_element = vaccine_node.toElement();
            QString trade_name{dom_element.text()};
            QString serial{dom_element.nextSiblingElement("serial").text()};
            QString country{dom_element.nextSiblingElement("country").text()};
            QDate best_before{QDate::fromString(dom_element.nextSiblingElement("best_before").text(), "yyyy-MM-dd")};
            QDate date{QDate::fromString(dom_element.nextSiblingElement("date").text(), "yyyy-MM-dd")};
            child.insertVaccine(name, trade_name, serial, country, best_before, date);
            inner_node = inner_node.nextSibling();
            if(inner_node.isElement())
                dom_element = inner_node.toElement();
        }
        child_node = child_node.nextSibling();
        children->push_back(child);
    }
}

void ChildrenModel::loadData()
{
    QDomDocument dom_doc;
    if(file_children->open(QIODevice::ReadOnly)){
        dom_doc.setContent(file_children);
        fillChildren(dom_doc);
        file_children->close();
    }
}

void ChildrenModel::saveChildren() const
{
    QDomDocument doc("children");
    QDomElement dom_root{doc.createElement("children")};
    for(ChildModel child : *children){
        QDomElement dom_element{doc.createElement("child")};
        dom_element.setAttribute("last_name", child.getLastName());
        dom_element.setAttribute("first_name", child.getFirstName());
        dom_element.setAttribute("middle_name", child.getMiddleName());
        QDomElement birthday{doc.createElement("birthday")};
        QDomText birthday_value{doc.createTextNode(child.getBirthday().toString("yyyy-MM-dd"))};
        birthday.appendChild(birthday_value);
        dom_element.appendChild(birthday);

        for(int i{0}; i < child.rowCount(); ++i){
            QDomElement vaccine{doc.createElement("vaccine")};
            vaccine.setAttribute("name", child.data(createIndex(i, 0)).toString());

            QDomElement trade_name{doc.createElement("trade_name")};
            QDomText trade_name_value{doc.createTextNode(child.data(createIndex(i, 1)).toString())};
            trade_name.appendChild(trade_name_value);

            QDomElement serial{doc.createElement("serial")};
            QDomText serial_value{doc.createTextNode(child.data(createIndex(i, 2)).toString())};
            serial.appendChild(serial_value);

            QDomElement country{doc.createElement("country")};
            QDomText country_value{doc.createTextNode(child.data(createIndex(i, 3)).toString())};
            country.appendChild(country_value);

            QDomElement best_before{doc.createElement("best_before")};
            QDomText best_before_value{doc.createTextNode(child.data(createIndex(i, 4)).toString())};
            best_before.appendChild(best_before_value);

            QDomElement date{doc.createElement("date")};
            QDomText date_value{doc.createTextNode(child.data(createIndex(i, 5)).toString())};
            date.appendChild(date_value);

            vaccine.appendChild(trade_name);
            vaccine.appendChild(serial);
            vaccine.appendChild(country);
            vaccine.appendChild(best_before);
            vaccine.appendChild(date);
            dom_element.appendChild(vaccine);
        }
        dom_root.appendChild(dom_element);
    }
    doc.appendChild(dom_root);

    if(file_children->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream text_stream(file_children);
        text_stream.setCodec("UTF-8");
        text_stream << doc.toString();
        file_children->close();
    }
}
