#ifndef SCANNINGURLSMODEL_H
#define SCANNINGURLSMODEL_H

#include "Dal/textsearcherresult.hpp"

#include <QAbstractListModel>
#include <vector>

namespace Models {


class ScanningUrlsModel : public QAbstractListModel {
    Q_OBJECT

    enum ScanningUrlRoles
    {
        UrlRole = Qt::UserRole + 1,
        StatusRole,
        ErrorRole
    };

public:
    using QAbstractListModel::QAbstractListModel;

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            { UrlRole, "url" },
            { StatusRole, "status" },
            { ErrorRole, "error" }
        };
    }

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return parent.isValid() ? 0 : static_cast<int>(urls_.size());
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (! hasIndex(index.row(), index.column(), index.parent()))
            return {};

        const TextSearcherResult &item = urls_.at(static_cast<size_t>(index.row()));
        if (role == UrlRole) return item.url;
        if (role == StatusRole) return item.status;
        if (role == ErrorRole) return item.error;

        return {};
    }

public:
    void emplaceBack(TextSearcherResult item)
    {
        urls_.emplace_back(std::move(item));
    }

private:
    std::vector<TextSearcherResult> urls_;
};


} // namespace Models


#endif // SCANNINGURLSMODEL_H
