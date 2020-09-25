#ifndef SCANNINGURLSMODEL_H
#define SCANNINGURLSMODEL_H
#include "Utils/Utils.hpp"
#include "Dal/textsearcherresult.hpp"

#include <QAbstractListModel>
#include <vector>


namespace Models {


class SerchedUrlsModel : public QAbstractListModel {
    Q_OBJECT

    enum ScanningUrlRoles
    {
        UrlRole = Qt::UserRole + 1,
        StatusRole,
        ErrorRole
    };

public:
    using QAbstractListModel::QAbstractListModel;

    SerchedUrlsModel(const SerchedUrlsModel &) = delete;

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override
    {
        return {
            { UrlRole, "url" },
            { StatusRole, "status" },
            { ErrorRole, "error" }
        };
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return parent.isValid() ? 0 : static_cast<int>(urls_.size());
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (! hasIndex(index.row(), index.column(), index.parent()))
            return {};

        const TextSearcherResult &item = urls_.at(static_cast<size_t>(index.row()));
        if (role == UrlRole) return item.url();
        if (role == StatusRole) return item.status;
        if (role == ErrorRole) return item.error;

        return {};
    }

public:
    void emplaceBack(TextSearcherResult item)
    {
        int lastIndex = rowCount();
        beginInsertRows(QModelIndex(), lastIndex, lastIndex);
        urls_.emplace_back(std::move(item));
        endInsertRows();
    }

    void update(TextSearcherResult item)
    {
        auto itToUpdate = std::find_if(urls_.cbegin(), urls_.cend(), [&item](const auto &res) {
            return res.hash() == item.hash();
        });

DEBUG(itToUpdate->hash() <<item.hash())
        DEBUG((itToUpdate == urls_.cend())<< itToUpdate->status<< item.status);
    }

    void reserve(size_t maxUrlsNumber)
    {
        urls_.reserve(maxUrlsNumber);
    }

    void clear()
    {
        urls_.clear();
    }

private:
    std::vector<TextSearcherResult> urls_;
};


} // namespace Models


#endif // SCANNINGURLSMODEL_H
