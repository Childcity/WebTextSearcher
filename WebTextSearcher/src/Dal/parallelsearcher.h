#ifndef PARALLELSEARCHER_H
#define PARALLELSEARCHER_H

#include "Utils/Utils.hpp"
#include "textsearcherresult.hpp"

#include <QThreadPool>


class ParallelSearcher : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit ParallelSearcher(int maxThreadsNum, int maxUrlsNum_, int urlDownloadingTimeout,
                              const QString &startUrl, QString serchedText, QObject *parent = nullptr);

    ~ParallelSearcher() override;

    // QRunnable interface
public:
    void run() override;

signals:
    void sigProgressChanged(TextSearcherResult);

private:
    void startSearcher(const std::shared_ptr<Utils::SafeUrlQueue> &queue,
                       QThreadPool &workers, const std::string &url);

private:
    int maxThreadsNum_;
    int maxUrlsNum_;
    int urlDownloadingTimeout_;
    std::string startUrl_;
    QString serchedText_;
};


#endif // PARALLELSEARCHER_H
