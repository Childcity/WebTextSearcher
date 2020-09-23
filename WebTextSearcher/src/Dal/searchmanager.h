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
    explicit ParallelSearcher(QObject *parent = nullptr)
        : QObject(parent)
    {}

    ~ParallelSearcher() override
    {
        DEBUG("~ParallelSearcher")
    }

    // QRunnable interface
public:
    void run() override
    {
        size_t maxUrlNumber = 1000;
        size_t scannedUrlNumber = 0;
        QThreadPool workers;
        auto queue = std::make_shared<Utils::SafeUrlQueue>();
        workers.setMaxThreadCount(100);
        //DEBUG("MAIN" << QThread::currentThreadId());

        queue->push("http://www.google.com/search?q=qthread");
        std::string url;
        QString qurl;
        while (scannedUrlNumber < maxUrlNumber) {

            if (! queue->tryPop(url)) {
                if (workers.activeThreadCount() == 0) {
                    break; // means no more urls to fetch
                } else {
                    QThread::currentThread()->yieldCurrentThread();
                    continue; // means queue is empty and we should wait for felling
                }
            }

            qurl = QString::fromStdString(url);
            auto sercher = new TextSearcher(queue, qurl);
            connect(sercher, &TextSearcher::sigResult, this, &ParallelSearcher::sigProgressChanged, Qt::QueuedConnection);
            workers.start(sercher);
            //emit sigProgressChanged({ qurl/*std::move(qurl)*/, TextSearcherStatus::Downloading, {} });

            scannedUrlNumber++;
        }
        workers.waitForDone();
        DEBUG("deeeeeeeeeeeeeeeee;" << queue->size()<<queue.use_count());
    }

signals:
    void sigProgressChanged(TextSearcherResult);
};

class SearchManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariant serchedUrlsModel READ serchedUrlsModel WRITE setSerchedUrlsModel NOTIFY serchedUrlsModelChanged)

public:
    explicit SearchManager(QObject *parent = nullptr)
        : QObject(parent)
    {}

    SearchManager(const SearchManager &) = delete;

    QVariant serchedUrlsModel() const
    {
        return QVariant::fromValue(serchedUrlsModel_);
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
        auto sercher = new ParallelSearcher();
        connect(sercher, &ParallelSearcher::sigProgressChanged, this, &SearchManager::slotProgressChanged, Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(sercher);
    }

    void slotStopSearcher()
    {
    }

private slots:
    void slotProgressChanged(TextSearcherResult res)
    {
//        if (serchedUrlsModel_)
//            serchedUrlsModel_->emplaceBack(std::move(res));
        static std::atomic<int> r = 0;
        r++;
        DEBUG(/*res.url << ": " << res.status << res.error <<*/ r);
    }

private:
    QPointer<Models::SerchedUrlsModel> serchedUrlsModel_;
};

#endif // SEARCHMANAGER_H
