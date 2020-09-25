#include "parallelsearcher.h"

#include "textsearcher.h"
#include <unordered_set>


namespace Dal {


ParallelSearcher::ParallelSearcher(int maxThreadsNum, int maxUrlsNum_, int urlDownloadingTimeout, const QString &startUrl, QString serchedText, QObject *parent)
    : QObject(parent)
    , maxThreadsNum_(maxThreadsNum)
    , maxUrlsNum_(maxUrlsNum_)
    , urlDownloadingTimeout_(urlDownloadingTimeout)
    , startUrl_(startUrl.toStdString())
    , serchedText_(std::move(serchedText))
{}

ParallelSearcher::~ParallelSearcher()
{
    DEBUG("~ParallelSearcher")
}

void ParallelSearcher::run()
{
    int scannedUrlsNumber = 0;
    QThreadPool workers;
    workers.setMaxThreadCount(static_cast<int>(maxThreadsNum_));

    auto queue = std::make_shared<Utils::SafeUrlQueue>();
    queue->push(std::move(startUrl_)); // We don't need startUrl_ any more

    // This helpful hash_map for quick knowledge about unique next url or not
    std::unordered_set<std::string> uniqueUrls(static_cast<size_t>(maxUrlsNum_));

    std::string url;
    while (scannedUrlsNumber < maxUrlsNum_) {

        if (! queue->tryPop(url)) {
            if (workers.activeThreadCount() == 0) {
                break; // Means no more urls to fetch
            } else {
                QThread::currentThread()->yieldCurrentThread();
                continue; // Means queue is empty and we should wait for filling
            }
        }

        {
            bool isUnique = uniqueUrls.insert(url).second;
            if (! isUnique)
                continue;
        }

        startSearcher(queue, workers, url);

        scannedUrlsNumber++;
    }

    workers.waitForDone();
    //DEBUG("run()" << queue->size() << queue.use_count());
}

void ParallelSearcher::startSearcher(const std::shared_ptr<Utils::SafeUrlQueue> &queue, QThreadPool &workers, const std::string &url)
{
    QString qurl = QString::fromStdString(url);

    auto sercher = new TextSearcher(queue, qurl, serchedText_, urlDownloadingTimeout_);
    connect(sercher, &TextSearcher::sigResult, this, &ParallelSearcher::sigProgressChanged, Qt::QueuedConnection);
    workers.start(sercher);

    // Send status of current url
    emit sigProgressChanged({ std::move(qurl), SearchStatusType::Downloading });
}


}
