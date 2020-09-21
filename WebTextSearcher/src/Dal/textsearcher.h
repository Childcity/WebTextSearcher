#ifndef TEXTSEARCHER_H
#define TEXTSEARCHER_H


#include "Utils/Utils.hpp"

#include <QThread>
#include <QRunnable>

#include <regex>

struct TextSearcherStatus {
    Q_GADGET

public:
    TextSearcherStatus() = delete;

    enum Value
    {
        NotSet,
        Found,
        NotFound,
        Downloading,
        Error
    };
    Q_ENUM(Value)
};
//qmlRegisterUncreatableType<ImageProviderType>("ImageProviderType", 1, 0, "ImageProviderType", "ImageProviderType is a Enum and can't b instantiated!");
//qRegisterMetaType<ImageProviderType::Value>("ImageProviderType::Value");
//Q_DECLARE_METATYPE(TextSearcherStatus);

struct TextSearcherResult {
public:
    TextSearcherResult() noexcept = default;
    TextSearcherResult(const TextSearcherResult &) noexcept = default;
    TextSearcherResult(TextSearcherResult &&) noexcept = default;

public:
    QString url;
    TextSearcherStatus::Value status;
    QString error;
};
Q_DECLARE_METATYPE(TextSearcherResult);

class TextSearcher : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit TextSearcher(Utils::SafeUrlQueue &urlsQueue, QString urlToFetch);

    //~TextSearcher() override;

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
    Utils::SafeUrlQueue &urlsQueue_;
    QString urlToFetch_;
};


#endif // TEXTSEARCHER_H
