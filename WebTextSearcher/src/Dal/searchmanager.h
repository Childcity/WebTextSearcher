#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include "Utils/Utils.hpp"
#include "Dal/textsearcher.h"
#include "Models/searchedurlsmodel.h"
#include "parallelsearcher.h"
#include "searchmanagerstatus.h"

#include <QPointer>
#include <QThreadPool>


namespace Dal {


class SearchManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(SearchManagerStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariant serchedUrlsModel READ serchedUrlsModel WRITE setSerchedUrlsModel NOTIFY serchedUrlsModelChanged)
    Q_PROPERTY(QString startUrl READ startUrl WRITE setStartUrl)
    Q_PROPERTY(QString serchedText READ serchedText WRITE setSerchedText)
    Q_PROPERTY(int maxThreadsNum READ maxThreadsNum WRITE setMaxThreadsNum)
    Q_PROPERTY(int maxUrlsNum READ maxUrlsNum WRITE setMaxUrlsNum NOTIFY maxUrlsNumChanged)
    Q_PROPERTY(int urlDownloadingTimeout READ urlDownloadingTimeout WRITE setUrlDownloadingTimeout)

public:
    explicit SearchManager(QObject *parent = nullptr);

    ~SearchManager();

    SearchManager(const SearchManager &) = delete;

    QVariant serchedUrlsModel() const;

    QString startUrl() const;

    QString serchedText() const;

    int maxThreadsNum() const;

    int maxUrlsNum() const;

    int urlDownloadingTimeout() const;

    SearchManagerStatus status() const;

signals:
    void serchedUrlsModelChanged(QVariant serchedUrlsModel);

    void maxUrlsNumChanged();

    void statusChanged(SearchManagerStatus status);

public slots:
    void setSerchedUrlsModel(const QVariant &serchedUrlsModel);

    void slotStartSearcher();

    void slotStopSearcher();

    void setStartUrl(QString startUrl);

    void setSerchedText(QString serchedText);

    void setMaxThreadsNum(int maxThreadsNum);

    void setMaxUrlsNum(int maxUrlsNum);

    void setUrlDownloadingTimeout(int urlDownloadingTimeout);

private slots:
    void slotProgressChanged(TextSearcherStatus res);

private:
    void setStatus(SearchManagerStatus status);

    void startParallelSearcher();

private:
    SearchManagerStatus status_;
    QPointer<Models::SerchedUrlsModel> serchedUrlsModel_;
    Utils::qt_unique_ptr<ParallelSearcher> searcher_;
    QString startUrl_;
    QString serchedText_;
    int maxThreadsNum_;
    int maxUrlsNum_;
    int urlDownloadingTimeout_;
};


} // namespace Dal

#endif // SEARCHMANAGER_H
