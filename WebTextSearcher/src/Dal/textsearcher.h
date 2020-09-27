#ifndef TEXTSEARCHER_H
#define TEXTSEARCHER_H

#include "Utils/Utils.hpp"
#include "textsearcherstatus.hpp"

#include <QThread>
#include <QRunnable>
#include <regex>


namespace Dal {


class TextSearcher : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit TextSearcher(std::shared_ptr<Utils::SafeUrlQueue> urlsQueue,
                          QString urlToFetch, QString serchedText,
                          int urlDownloadingTimeout, const std::atomic_bool &isCanceled,
                          QObject *parent = nullptr) noexcept;

    ~TextSearcher() override;

    // QRunnable interface
public:
    void run() override;

signals:
    void sigResult(TextSearcherStatus);

private:
    static std::regex &GetLinkRegex();

    static QString &GetDefaultUserAgent();

private:
    std::shared_ptr<Utils::SafeUrlQueue> urlsQueue_;
    QString urlToFetch_;
    QString serchedText_;
    int urlDownloadingTimeout_;
    const std::atomic_bool &isCanceled_;
};


} // namespace Dal

#endif // TEXTSEARCHER_H
