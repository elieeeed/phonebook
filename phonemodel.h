#ifndef PHONEMODEL_H
#define PHONEMODEL_H

#include <QAbstractItemModel>
#include <QDataStream>
#include <QSortFilterProxyModel>

class Person {
public:
    Person(const QString& name, const QString& phone, const QString& address);

    QString m_name;
    QString m_phone;
    QString m_address;

private:
    Person() = default;
};

QDataStream& operator<<(QDataStream& out, const Person& person);

class MyFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit MyFilterProxyModel(QObject* parent = nullptr)
        : QSortFilterProxyModel(parent) {}

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

public slots:
    void setTextFilter(const QString& text);

private:
    QString search_text;
};

class PhoneModel : public QAbstractItemModel {
    Q_PROPERTY(MyFilterProxyModel* proxy_model_property READ get_proxy_model NOTIFY proxy_model_propertyChanged)
    Q_PROPERTY(int curent_index_property READ get_curent_index WRITE set_curent_index NOTIFY curent_index_propertyChanged)
    Q_OBJECT
public:
    MyFilterProxyModel proxy_model;
    MyFilterProxyModel* get_proxy_model()
    {
        return &proxy_model;
    }
    int get_curent_index()
    {
        return curent_index;
    }

    void set_curent_index(int ind)
    {
        curent_index = ind;
    }

    enum PersonRoles {
        NameRole = Qt::UserRole + 1,
        PhoneRole,
        AddressRole,
        IndexRole
    };

    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex& child) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    explicit PhoneModel(QObject* parent = 0);
    virtual void addPerson(const Person& person);

    Q_INVOKABLE void update();
    Q_INVOKABLE void addPerson(const QString& name, const QString& phone, const QString& address);
    Q_INVOKABLE void remove_person(int row);
    Q_INVOKABLE bool setPerson(int row, const QString& name, const QString& phone, const QString& address);
    Q_INVOKABLE int get_last_index();
    Q_INVOKABLE QString get_person_name(int row);
    Q_INVOKABLE QString get_person_phone(int row);
    Q_INVOKABLE QString get_person_address(int row);
    Q_INVOKABLE void clear();
signals:
    void proxy_model_propertyChanged();
    void curent_index_propertyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;
    QList<Person> m_persons;
    int curent_index = -1;
};

#endif // PHONEMODEL_H
