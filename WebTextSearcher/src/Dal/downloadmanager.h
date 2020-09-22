#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "Utils/Utils.hpp"
#include "Dal/textsearcher.h"

#include <QThreadPool>
#include <QTimer>


class ParallelSearcher : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit ParallelSearcher() {}

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
        while (scannedUrlNumber < maxUrlNumber) {

            if (! queue->tryPop(url)) {
                if (workers.activeThreadCount() == 0) {
                    break; // means no more urls to fetch
                } else {
                    QThread::currentThread()->yieldCurrentThread();
                    continue; // means queue is empty and we should wait for felling
                }
            }

            auto sercher = new TextSearcher(queue, QString::fromStdString(url));
            connect(sercher, &TextSearcher::sigResult, this, &ParallelSearcher::sigProgressChanged, Qt::QueuedConnection);
            workers.start(sercher);

            scannedUrlNumber++;
        }
        workers.waitForDone();
        DEBUG("deeeeeeeeeeeeeeeee;" << queue->size());
    }

signals:
    void sigProgressChanged(TextSearcherResult);
};

class DownloadManager : public QObject {
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr)
        : QObject(parent)
    {}

signals:

public slots:
    void slotProgressChanged(TextSearcherResult res)
    {
        static std::atomic<int> r = 0;
        r++;
        DEBUG(res.url << ": " << res.status << res.error << r);
    }

    void slotStartSearcher()
    {
        auto sercher = new ParallelSearcher();
        connect(sercher, &ParallelSearcher::sigProgressChanged, this, &DownloadManager::slotProgressChanged, Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(sercher);
    }

    void slotStopSearcher()
    {
    }

private:
};

#endif // DOWNLOADMANAGER_H
