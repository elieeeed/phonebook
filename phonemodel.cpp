#include "phonemodel.h"

#include <QFile>
#include <QUrl>

QDataStream& operator<<(QDataStream& out, const Person& person)
{
    out << person.m_name << person.m_phone << person.m_address;
    return out;
}

PhoneModel::PhoneModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    proxy_model.setSourceModel(this);
    proxy_model.setSortRole(NameRole);
}

QHash<int, QByteArray> PhoneModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PhoneRole] = "phone";
    roles[AddressRole] = "address";
    roles[IndexRole] = "index";
    return roles;
}

QModelIndex PhoneModel::index(int row, int column, const QModelIndex& parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex PhoneModel::parent(const QModelIndex& child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int PhoneModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : 1;
}

bool PhoneModel::setPerson(int row, const QString& name, const QString& phone, const QString& address)
{
    QModelIndex index_ = proxy_model.mapToSource(proxy_model.index(row, 0));
    int row_ = index_.row();
    if (row_ > m_persons.count() || row_ < 0)
        return false;

    m_persons[row_].m_name = name;
    m_persons[row_].m_phone = phone;
    m_persons[row_].m_address = address;

    update();

    curent_index = proxy_model.mapFromSource(index(row_, 0)).row();
    emit curent_index_propertyChanged();

    return true;
}

void PhoneModel::addPerson(const Person& person)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_persons << person;
    endInsertRows();
}

void PhoneModel::addPerson(const QString& name, const QString& phone, const QString& address)
{
    addPerson({ name, phone, address });
}

int PhoneModel::get_last_index()
{
    return proxy_model.mapFromSource(index(m_persons.size() - 1, 0)).row();
}

void PhoneModel::update()
{
    beginResetModel();
    endResetModel();
}

void PhoneModel::clear()
{
    beginResetModel();
    m_persons.clear();
    endResetModel();
}

QString PhoneModel::get_person_name(int row)
{
    int index = proxy_model.mapToSource(proxy_model.index(row, 0)).row();
    return m_persons[index].m_name;
}

QString PhoneModel::get_person_phone(int row)
{
    int index = proxy_model.mapToSource(proxy_model.index(row, 0)).row();
    return m_persons[index].m_phone;
}

QString PhoneModel::get_person_address(int row)
{
    int index = proxy_model.mapToSource(proxy_model.index(row, 0)).row();
    return m_persons[index].m_address;
}

void PhoneModel::remove_person(int row)
{
    int index = proxy_model.mapToSource(proxy_model.index(row, 0)).row();

    beginRemoveRows(QModelIndex(), index, index);
    m_persons.removeAt(index);
    endRemoveRows();
}

int PhoneModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_persons.count();
}

QVariant PhoneModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_persons.count())
        return QVariant();

    const Person& person = m_persons[index.row()];
    if (role == NameRole)
        return person.m_name;
    else if (role == PhoneRole)
        return person.m_phone;
    else if (role == AddressRole)
        return person.m_address;
    return QVariant();
}

QVariant MyFilterProxyModel::data(const QModelIndex& index, int role) const
{
    if (role == PhoneModel::IndexRole)
        return index.row();
    return QSortFilterProxyModel::data(index, role);
}

bool MyFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    QString name = sourceModel()->data(index, PhoneModel::NameRole).toString().toLower();
    QString phone = sourceModel()->data(index, PhoneModel::PhoneRole).toString().toLower();

    return (name.contains(search_text) || phone.contains(search_text));
}

void MyFilterProxyModel::setTextFilter(const QString& text)
{
    search_text = text.toLower();
    invalidateFilter();
}

Person::Person(const QString& name, const QString& phone, const QString& address)
    : m_name(name)
    , m_phone(phone)
    , m_address(address)
{
}
