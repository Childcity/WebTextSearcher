#include "searchedurlsmodel.h"


QHash<int, QByteArray> Models::SerchedUrlsModel::roleNames() const
{
    return {
        { UrlRole, "url" },
        { StatusRole, "status" },
        { ErrorRole, "error" }
    };
}

int Models::SerchedUrlsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(urls_.size());
}

QVariant Models::SerchedUrlsModel::data(const QModelIndex &index, int role) const
{
    if (! hasIndex(index.row(), index.column(), index.parent()))
        return {};

    const Dal::TextSearcherStatus &item = urls_.at(static_cast<size_t>(index.row()));
    if (role == UrlRole) return item.url();
    if (role == StatusRole) return item.status;
    if (role == ErrorRole) return item.error;

    return {};
}

void Models::SerchedUrlsModel::emplaceBack(Dal::TextSearcherStatus item)
{
    int lastIndex = rowCount();
    beginInsertRows(QModelIndex(), lastIndex, lastIndex);
    urls_.emplace_back(std::move(item));
    endInsertRows();

    emit rowCountChanged();
}

void Models::SerchedUrlsModel::update(Dal::TextSearcherStatus item,  const QVector<int> &roles)
{
    auto itToUpdate = std::find_if(urls_.begin(), urls_.end(), [&item](const auto &res) {
        return res.hash() == item.hash();
    });

    if (itToUpdate == urls_.cend()) {
        DEBUG("Updating item not found!");
        return;
    }

    *itToUpdate = std::move(item);

    QModelIndex indx = index(static_cast<int>(std::distance(urls_.begin(), itToUpdate)));
    emit dataChanged(indx, indx, roles);
}

void Models::SerchedUrlsModel::reserve(size_t maxUrlsNumber)
{
    urls_.reserve(maxUrlsNumber);
}

void Models::SerchedUrlsModel::clear()
{
    beginResetModel();
    urls_.clear();
    endResetModel();
}
