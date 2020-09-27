#include "parallelsearcher.h"

#include "textsearcher.h"
#include <unordered_set>


namespace Dal {


ParallelSearcher::ParallelSearcher(int maxThreadsNum, int maxUrlsNum_, int urlDownloadingTimeout, const QString &startUrl, QString serchedText, QObject *parent)
    : QThread(parent)
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
    if (isCanceled())
        return;

    int scannedUrlsNumber = 0;
    QThreadPool workers;
    workers.setMaxThreadCount(static_cast<int>(maxThreadsNum_));
    std::atomic_bool isCanceledDownload = false;

    // Create thread safe queue of urls to be downloaded later
    auto queue = std::make_shared<Utils::SafeUrlQueue>();
    queue->push(startUrl_);

    // This helpful hash_map for quick knowledge about unique next url or not
    std::unordered_set<std::string> uniqueUrls(static_cast<size_t>(maxUrlsNum_));


    std::string nextUrl;
    while (scannedUrlsNumber < maxUrlsNum_) {

        if (isCanceled()) {
            isCanceledDownload.store(true, std::memory_order_relaxed);
            break;
        }

        if (! queue->tryPop(nextUrl)) {
            if (workers.activeThreadCount() == 0) {
                break; // Means no more urls to fetch
            } else {
                QThread::currentThread()->yieldCurrentThread();
                continue; // Means queue is empty and we should wait for filling
            }
        }

        {
            bool isUnique = uniqueUrls.insert(nextUrl).second;
            if (! isUnique)
                continue;
        }

        startSearcher(queue, workers, nextUrl, isCanceledDownload);

        scannedUrlsNumber++;
    }

    workers.waitForDone();
    DEBUG("run()" << queue->size() << queue.use_count() << "isCanceled()" << isCanceled());
}

void ParallelSearcher::startSearcher(const std::shared_ptr<Utils::SafeUrlQueue> &queue,
                                     QThreadPool &workers, const std::string &url,
                                     const std::atomic_bool &isCanceledDownload)
{
    Utils::OnExit _([this] { this->setTerminationEnabled(true); });
    setTerminationEnabled(false);

    QString qurl = QString::fromStdString(url);
    auto sercher = new TextSearcher(queue, qurl, serchedText_, urlDownloadingTimeout_, isCanceledDownload);
    connect(sercher, &TextSearcher::sigResult, this, &ParallelSearcher::sigProgressChanged, Qt::QueuedConnection);
    workers.start(sercher);

    // Send status of current url
    emit sigProgressChanged({ std::move(qurl), SearchStatusType::Downloading });
}

bool ParallelSearcher::isCanceled() const
{
    return QThread::isInterruptionRequested();
}


} // namespace Dal
