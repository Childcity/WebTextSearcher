#include "searchmanager.h"


namespace Dal {


SearchManager::SearchManager(QObject *parent)
    : QObject(parent)
    , status_(SearchManagerStatus::Stopped)
{}

SearchManager::~SearchManager()
{
    slotStopSearcher();
}

QVariant SearchManager::serchedUrlsModel() const
{
    return QVariant::fromValue(serchedUrlsModel_.data());
}

QString SearchManager::startUrl() const
{
    return startUrl_;
}

QString SearchManager::serchedText() const
{
    return serchedText_;
}

int SearchManager::maxThreadsNum() const
{
    return maxThreadsNum_;
}

int SearchManager::maxUrlsNum() const
{
    return maxUrlsNum_;
}

int SearchManager::urlDownloadingTimeout() const
{
    return urlDownloadingTimeout_;
}

SearchManagerStatus SearchManager::status() const
{
    return status_;
}

void SearchManager::setSerchedUrlsModel(const QVariant &serchedUrlsModel)
{
    auto ptr = serchedUrlsModel.value<Models::SerchedUrlsModel *>();

    if (serchedUrlsModel_ == ptr)
        return;

    serchedUrlsModel_ = ptr;
    emit serchedUrlsModelChanged(serchedUrlsModel);
}

void SearchManager::slotStartSearcher()
{
    // Stop and disconnect old searcher_
    if (status_ != SearchManagerStatus::Stopped) {
        if (searcher_)
            searcher_->disconnect(this);
        slotStopSearcher();
        setStatus(SearchManagerStatus::Stopped);
    }

    setStatus(SearchManagerStatus::Starting);

    if (serchedUrlsModel_) {
        serchedUrlsModel_->clear();
        serchedUrlsModel_->reserve(static_cast<size_t>(maxUrlsNum_));
    }

    startParallelSearcher();

    setStatus(SearchManagerStatus::Running);
}

void SearchManager::slotStopSearcher()
{
    setStatus(SearchManagerStatus::Stopping);

    if (searcher_) {
        if (searcher_->isRunning()) {
            searcher_->requestInterruption();
            if (! searcher_->wait(20000)) {
                DEBUG("Searcher thread will be terminated directly!");
                searcher_->terminate();
                searcher_->wait();
            } else {
                // If normal Interruption -> we just return.
                // Stopped will besited after  watcher will be stopped
                return;
            }
        }
    }

    setStatus(SearchManagerStatus::Stopped);
}

void SearchManager::setStartUrl(QString startUrl)
{
    startUrl_ = std::move(startUrl);
}

void SearchManager::setSerchedText(QString serchedText)
{
    serchedText_ = std::move(serchedText);
}

void SearchManager::setMaxThreadsNum(int maxThreadsNum)
{
    maxThreadsNum_ = maxThreadsNum;
}

void SearchManager::setMaxUrlsNum(int maxUrlsNum)
{
    if (maxUrlsNum_ == maxUrlsNum)
        return;

    maxUrlsNum_ = maxUrlsNum;
    emit maxUrlsNumChanged();
}

void SearchManager::setUrlDownloadingTimeout(int urlDownloadingTimeout)
{
    urlDownloadingTimeout_ = urlDownloadingTimeout;
}

void SearchManager::slotProgressChanged(TextSearcherStatus res)
{
    using namespace Models;

    if (serchedUrlsModel_) {
        if (res.status == SearchStatusType::Downloading) {
            serchedUrlsModel_->emplaceBack(std::move(res));
        } else {
            serchedUrlsModel_->update(std::move(res), { SerchedUrlsModel::StatusRole, SerchedUrlsModel::ErrorRole });
        }
    }
}

void SearchManager::setStatus(SearchManagerStatus status)
{
    if (status_ == status)
        return;

    status_ = status;
    emit statusChanged(status_);
}

void SearchManager::startParallelSearcher()
{

    searcher_ = Utils::qt_make_unique<ParallelSearcher>(maxThreadsNum_, maxUrlsNum_, urlDownloadingTimeout_,
                                                        startUrl_, serchedText_, this);

    connect(&*searcher_, &ParallelSearcher::sigProgressChanged,
            this, &SearchManager::slotProgressChanged, Qt::QueuedConnection);

    connect(
        &*searcher_, &ParallelSearcher::sigWorkIsDone, this,
        [this] { setStatus(SearchManagerStatus::Stopped); },
        Qt::QueuedConnection);

    searcher_->start();
}


} // namespace Dal
