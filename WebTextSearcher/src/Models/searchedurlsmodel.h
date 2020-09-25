#ifndef SCANNINGURLSMODEL_H
#define SCANNINGURLSMODEL_H

#include "Utils/Utils.hpp"
#include "Dal/textsearcherstatus.hpp"

#include <QAbstractListModel>
#include <vector>


namespace Models {


class SerchedUrlsModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum ScanningUrlRoles
    {
        UrlRole = Qt::UserRole + 1,
        StatusRole,
        ErrorRole
    };

    using QAbstractListModel::QAbstractListModel;

    SerchedUrlsModel(const SerchedUrlsModel &) = delete;

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

public:
    void emplaceBack(Dal::TextSearcherStatus item);

    void update(Dal::TextSearcherStatus item, const QVector<int> &roles = {});

    void reserve(size_t maxUrlsNumber);

    void clear();

private:
    std::vector<Dal::TextSearcherStatus> urls_;
};


} // namespace Models


#endif // SCANNINGURLSMODEL_H
