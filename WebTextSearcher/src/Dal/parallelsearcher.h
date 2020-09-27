#ifndef PARALLELSEARCHER_H
#define PARALLELSEARCHER_H

#include "Utils/Utils.hpp"
#include "textsearcherstatus.hpp"

#include <QThreadPool>


namespace Dal {


class ParallelSearcher : public QThread {
    Q_OBJECT

public:
    explicit ParallelSearcher(int maxThreadsNum, int maxUrlsNum_, int urlDownloadingTimeout,
                              const QString &startUrl, QString serchedText, QObject *parent = nullptr);

    ~ParallelSearcher() override;

    void run() override;

signals:
    void sigProgressChanged(TextSearcherStatus);

private:
    void startSearcher(const std::shared_ptr<Utils::SafeUrlQueue> &queue,
                       QThreadPool &workers, const std::string &url);

    bool isCanceled() const;

    void waitForDoneOrCancel(QThreadPool &workers);

private:
    int maxThreadsNum_;
    int maxUrlsNum_;
    int urlDownloadingTimeout_;
    std::string startUrl_;
    QString serchedText_;

    std::atomic_bool isCanceletionRequested_;
};


} // namespace Dal

#endif // PARALLELSEARCHER_H
