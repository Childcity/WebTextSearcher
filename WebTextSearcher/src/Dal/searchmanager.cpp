#include "searchmanager.h"


namespace Dal {


SearchManager::SearchManager(QObject *parent)
    : QObject(parent)
{}

QVariant SearchManager::serchedUrlsModel() const
{
    return QVariant::fromValue(serchedUrlsModel_);
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
    if (serchedUrlsModel_) {
        serchedUrlsModel_->clear();
        serchedUrlsModel_->reserve(static_cast<size_t>(maxUrlsNum_));
    }

    auto sercher = new ParallelSearcher(maxThreadsNum_, maxUrlsNum_, urlDownloadingTimeout_,
                                        startUrl_, serchedText_);

    connect(sercher, &ParallelSearcher::sigProgressChanged,
            this, &SearchManager::slotProgressChanged, Qt::QueuedConnection);

    QThreadPool::globalInstance()->start(sercher);
}

void SearchManager::slotStopSearcher()
{

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
    maxUrlsNum_ = maxUrlsNum;
}

void SearchManager::seUrlDownloadingTimeout(int urlDownloadingTimeout)
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


} // namespace Dal
