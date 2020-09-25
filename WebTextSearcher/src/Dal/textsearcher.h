#ifndef TEXTSEARCHER_H
#define TEXTSEARCHER_H

#include "Utils/Utils.hpp"
#include "textsearcherresult.hpp"

#include <QThread>
#include <QRunnable>
#include <regex>


class TextSearcher : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit TextSearcher(std::shared_ptr<Utils::SafeUrlQueue> urlsQueue,
                          QString urlToFetch, QString serchedText,
                          int urlDownloadingTimeout,
                          QObject *parent = nullptr) noexcept;

    ~TextSearcher() override;

    // QRunnable interface
public:
    void run() override;

signals:
    void sigResult(TextSearcherResult);

private:
    static std::regex &GetLinkRegex();

    static QString &GetDefaultUserAgent();

private:
    std::shared_ptr<Utils::SafeUrlQueue> urlsQueue_;
    QString urlToFetch_;
    QString serchedText_;
    int urlDownloadingTimeout_;
};


#endif // TEXTSEARCHER_H
