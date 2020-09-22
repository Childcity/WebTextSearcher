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
                          QString urlToFetch, QObject *parent = nullptr) noexcept;

    //~TextSearcher() override {DEBUG("~TextSearcher")}

    // QRunnable interface
public:
    void run() override;

signals:
    void sigResult(TextSearcherResult);

private:
    static std::regex &GetLinkRegex()
    {
        static std::regex imgUrlTemplate {
            R"_(https*:\/\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\/\/=]*))_"
        };
        return imgUrlTemplate;
    }

    static QString &GetDefaultUserAgent()
    {
        static QString userAgent = QLatin1Literal("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) "
                                                  "Gecko/20100101 Firefox/15.0.1");
        return userAgent;
    }

private:
    std::shared_ptr<Utils::SafeUrlQueue> urlsQueue_;
    QString urlToFetch_;
};


#endif // TEXTSEARCHER_H
