#include <QAbstractListModel>

//![0]
class Animal
{
public:
    Animal(const int &longitude, const int &latitude , const float &height_);
//![0]

    int longitude() const;
    int latitude() const;
    float height_() const;

private:
    int m_longitude;
    int m_latitude;
    float m_height;
//![1]
};

class AnimalModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AnimalRoles {
        longitudeRole = Qt::UserRole + 1,
        latitudeRole,
        heightRole
    };

    AnimalModel(QObject *parent = 0);
//![1]
    Q_INVOKABLE void cppMethod(const QString &msg);

    void addAnimal(const Animal &animal);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Animal> m_animals;
//![2]
};
//![2]
