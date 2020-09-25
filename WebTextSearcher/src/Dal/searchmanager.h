#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include "Utils/Utils.hpp"
#include "Dal/textsearcher.h"
#include "Models/searchedurlsmodel.h"

#include <QPointer>
#include <QThreadPool>
#include <QTimer>


class ParallelSearcher : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit ParallelSearcher(int maxThreadsNum, int maxUrlsNum_, int urlDownloadingTimeout,
                              const QString &startUrl, QObject *parent = nullptr)
        : QObject(parent)
        , maxThreadsNum_(maxThreadsNum)
        , maxUrlsNum_(maxUrlsNum_)
        , urlDownloadingTimeout_(urlDownloadingTimeout)
        , startUrl_(startUrl.toStdString())
    {}

    ~ParallelSearcher() override
    {
        DEBUG("~ParallelSearcher")
    }

    // QRunnable interface
public:
    void run() override
    {
        QThreadPool workers;
        workers.setMaxThreadCount(static_cast<int>(maxThreadsNum_));

        auto queue = std::make_shared<Utils::SafeUrlQueue>();
        queue->push(std::move(startUrl_)); // We don't need startUrl_ any more

        std::string url;
        QString qurl;
        while (int scannedUrlsNumber = 0 < maxUrlsNum_) {

            if (! queue->tryPop(url)) {
                if (workers.activeThreadCount() == 0) {
                    break; // Means no more urls to fetch
                } else {
                    QThread::currentThread()->yieldCurrentThread();
                    continue; // Means queue is empty and we should wait for filling
                }
            }

            qurl = QString::fromStdString(url);
            auto sercher = new TextSearcher(queue, qurl, urlDownloadingTimeout_);
            connect(sercher, &TextSearcher::sigResult, this, &ParallelSearcher::sigProgressChanged, Qt::QueuedConnection);
            workers.start(sercher);

            // Send status of current url
            TextSearcherStatus newUrl;
            emit sigProgressChanged();

            scannedUrlsNumber++;
        }

        workers.waitForDone();
        //DEBUG("run()" << queue->size() << queue.use_count());
    }

signals:
    void sigProgressChanged(TextSearcherResult);

private:
    int maxThreadsNum_;
    int maxUrlsNum_;
    int urlDownloadingTimeout_;
    std::string startUrl_;
};

class SearchManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariant serchedUrlsModel READ serchedUrlsModel WRITE setSerchedUrlsModel NOTIFY serchedUrlsModelChanged)
    Q_PROPERTY(QString startUrl READ startUrl WRITE setStartUrl)
    Q_PROPERTY(QString serchedText READ serchedText WRITE setSerchedText)
    Q_PROPERTY(int maxThreadsNum READ maxThreadsNum WRITE setMaxThreadsNum)
    Q_PROPERTY(int maxUrlsNum READ maxUrlsNum WRITE setMaxUrlsNum)
    Q_PROPERTY(int urlDownloadingTimeout READ urlDownloadingTimeout WRITE seUrlDownloadingTimeout)

public:
    explicit SearchManager(QObject *parent = nullptr) noexcept
        : QObject(parent)
    {}

    SearchManager(const SearchManager &) = delete;

    QVariant serchedUrlsModel() const
    {
        return QVariant::fromValue(serchedUrlsModel_);
    }

    QString startUrl() const
    {
        return startUrl_;
    }

    QString serchedText() const
    {
        return serchedText_;
    }

    int maxThreadsNum() const
    {
        return maxThreadsNum_;
    }

    int maxUrlsNum() const
    {
        return maxUrlsNum_;
    }

    int urlDownloadingTimeout() const
    {
        return urlDownloadingTimeout_;
    }

signals:
    void serchedUrlsModelChanged(QVariant serchedUrlsModel);

public slots:
    void setSerchedUrlsModel(const QVariant &serchedUrlsModel)
    {
        auto ptr = serchedUrlsModel.value<Models::SerchedUrlsModel *>();

        if (serchedUrlsModel_ == ptr)
            return;

        serchedUrlsModel_ = ptr;
        emit serchedUrlsModelChanged(serchedUrlsModel);
    }

    void slotStartSearcher()
    {
        if (serchedUrlsModel_)
            serchedUrlsModel_->reserve(static_cast<size_t>(maxUrlsNum_));

        auto sercher = new ParallelSearcher(maxThreadsNum_, maxUrlsNum_,
                                            urlDownloadingTimeout_, startUrl_);
        connect(sercher, &ParallelSearcher::sigProgressChanged,
                this, &SearchManager::slotProgressChanged, Qt::QueuedConnection);

        QThreadPool::globalInstance()->start(sercher);
    }

    void slotStopSearcher()
    {
    }

    void setStartUrl(QString startUrl)
    {
        startUrl_ = std::move(startUrl);
    }

    void setSerchedText(QString serchedText)
    {
        serchedText_ = std::move(serchedText);
    }

    void setMaxThreadsNum(int maxThreadsNum)
    {
        maxThreadsNum_ = maxThreadsNum;
    }

    void setMaxUrlsNum(int maxUrlsNum)
    {
        maxUrlsNum_ = maxUrlsNum;
    }

    void seUrlDownloadingTimeout(int urlDownloadingTimeout)
    {
        urlDownloadingTimeout_ = urlDownloadingTimeout;
    }

private slots:
    void slotProgressChanged(TextSearcherResult res)
    {
        //static std::atomic<int> r = 0;
        //r++;
        //DEBUG(res.url << ": " << res.status << res.error << r << QThread::currentThreadId());
        if (serchedUrlsModel_) {
            if (res.status == TextSearcherStatus::Downloading) {
                serchedUrlsModel_->emplaceBack(std::move(res));
            } else {
                serchedUrlsModel_->update(std::move(res));
            }
        }
    }

private:
    QPointer<Models::SerchedUrlsModel> serchedUrlsModel_;
    QString startUrl_;
    QString serchedText_;
    int maxThreadsNum_;
    int maxUrlsNum_;
    int urlDownloadingTimeout_;
};

#endif // SEARCHMANAGER_H
