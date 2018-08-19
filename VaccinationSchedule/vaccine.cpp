#include "vaccine.h"

Vaccine::Vaccine(QString _name, QString _trade_name, QString _serial, QString _country, QDate _best_before):
    name(_name), trade_name(_trade_name), serial(_serial), country(_country), best_before(_best_before)
{

}

QString Vaccine::getName() const
{
    return name;
}

void Vaccine::setName(QString const &value)
{
    name = value;
}

QString Vaccine::getTradeName() const
{
    return trade_name;
}

void Vaccine::setTradeName(QString const &value)
{
    trade_name = value;
}

QString Vaccine::getSerial() const
{
    return serial;
}

void Vaccine::setSerial(QString const &value)
{
    serial = value;
}

QString Vaccine::getCountry() const
{
    return country;
}

void Vaccine::setCountry(QString const &value)
{
    country = value;
}

QDate Vaccine::getBestBefore() const
{
    return best_before;
}

void Vaccine::setBestBefore(QDate const &value)
{
    best_before = value;
}

QDate Vaccine::getVaccinationDate() const
{
    return vaccination_date;
}

void Vaccine::setVaccinationDate(const QDate &value)
{
    vaccination_date = value;
}

QString Vaccine::getMult() const
{
    return mult;
}

void Vaccine::setMult(QString const &value)
{
    mult = value;
}

int Vaccine::getCount() const
{
    return count;
}

void Vaccine::setCount(int const &value)
{
    count = value;
}
