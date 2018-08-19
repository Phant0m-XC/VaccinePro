#ifndef VACCINE_H
#define VACCINE_H

#include <QString>
#include <QDate>

class Vaccine
{
    QString name;                       //name of vaccine
    QString trade_name;                 //trade name of vaccine
    QString serial;                     //serial of vaccine
    QString country;                    //manufacturer country of vaccine
    QDate best_before;                  //best before
    QDate vaccination_date;             //vaccination date
    QString mult;                       //multiplicity of vaccination (for settings)
    int count;                          //count of vaccine in refrigerator (in the future release)

public:
    Vaccine() = default;
    Vaccine(QString, QString, QString, QString, QDate);
    Vaccine(Vaccine const&) = default;
    Vaccine& operator=(Vaccine const&) = default;
    ~Vaccine() = default;

    QString getName() const;                    //getter, setter name
    void setName(QString const&);               //

    QString getTradeName() const;               //getter, setter trade name
    void setTradeName(QString const&);          //

    QString getSerial() const;                  //getter, setter serial
    void setSerial(QString const&);             //

    QString getCountry() const;                 //getter, setter country
    void setCountry(QString const&);            //

    QDate getBestBefore() const;                //getter, setter best before
    void setBestBefore(QDate const&);           //

    QDate getVaccinationDate() const;           //getter, setter vaccination date
    void setVaccinationDate(QDate const&);      //

    QString getMult() const;                    //getter, setter multiplicity of vaccination
    void setMult(QString const&);               //

    int getCount() const;                       //getter, setter count of vaccines
    void setCount(const int&);                  //

};

#endif // VACCINE_H
