
#include "model.h"
//#include <iostream>
#include<QString>
extern QStringList Read (QString filename);
Animal::Animal(const int &longitude, const int &latitude,const float &height_)
    : m_longitude(longitude), m_latitude(latitude) ,m_height(height_)
{
}

int Animal::longitude() const
{
    return m_longitude;
}

int Animal::latitude() const
{
    return m_latitude;
}
float Animal::height_() const
{
    return m_height;
}
AnimalModel::AnimalModel(QObject *parent)
    : QAbstractListModel(parent)
{
}
Q_INVOKABLE void AnimalModel::cppMethod(const QString &msg) {
    QString filename =msg;
    QStringList outputList=Read(filename.remove(0,8));
    m_animals.clear();
    for ( int i =0;i<outputList.size() ; i++){

       addAnimal(Animal(outputList[i].split(",")[0].toInt(), outputList[i].split(",")[1].toInt(),outputList[i].split(",")[2].toFloat()));
    }
    }
void AnimalModel::addAnimal(const Animal &animal)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_animals << animal;
    endInsertRows();
}

int AnimalModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_animals.count();
}

QVariant AnimalModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_animals.count())
        return QVariant();

    const Animal &animal = m_animals[index.row()];
    if (role == longitudeRole)
        return animal.longitude();
    else if (role == latitudeRole)
        return animal.latitude();
    else if (role == heightRole)
        return animal.height_();
    return QVariant();
}
//![0]
QHash<int, QByteArray> AnimalModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[longitudeRole] = "longitude";
    roles[latitudeRole] = "latitude";
    roles[heightRole] = "height_";
    return roles;
}
//![0]
