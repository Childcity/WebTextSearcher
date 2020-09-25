#include "textsearcher.h"

#include "Net/idownloader.h"
#include "Net/downloader.h"
#include "Net/networkerror.h"

#include <QTimer>


TextSearcher::TextSearcher(std::shared_ptr<Utils::SafeUrlQueue> urlsQueue,
                           QString urlToFetch, QString serchedText,
                           int urlDownloadingTimeout,
                           QObject *parent) noexcept
    : QObject(parent)
    , urlsQueue_(std::move(urlsQueue))
    , urlToFetch_(std::move(urlToFetch))
    , serchedText_(std::move(serchedText))
    , urlDownloadingTimeout_(urlDownloadingTimeout)
{}

TextSearcher::~TextSearcher()
{
    //DEBUG("~TextSearcher")
}

void TextSearcher::run()
{
    using namespace std::chrono;
    using namespace Net;

    TextSearcherResult result;
    std::unique_ptr<IDownloader> downloader = std::make_unique<Downloader>();

    downloader->setTimeout(milliseconds(urlDownloadingTimeout_));

    QNetworkRequest request(urlToFetch_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::UserVerifiedRedirectPolicy);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
    request.setHeader(QNetworkRequest::UserAgentHeader, GetDefaultUserAgent());

    try {
        const QByteArray page = downloader->get(request);


        // Search text in page
        {
            const QByteArray ba = serchedText_.toUtf8();
            auto found = std::search(page.cbegin(), page.cend(), ba.cbegin(), ba.cend());
            result.status = (found != page.cend())
                                ? TextSearcherStatus::Found
                                : TextSearcherStatus::NotFound;
        }

        // Search urls in page
        {
            auto matchesBegin = std::regex_token_iterator(page.cbegin(), page.cend(), GetLinkRegex());
            auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();

            std::for_each(matchesBegin, matchesEnd, [this](const std::string url) {
                urlsQueue_->push(std::move(url));
            });
        }

    } catch (const NetworkError &ex) {
        result.status = TextSearcherStatus::Error;
        result.error = ex.what();
    }

    //DEBUG(result.status << result.url);

    result.setUrl(std::move(urlToFetch_));
    emit sigResult(std::move(result));
}

std::regex &TextSearcher::GetLinkRegex()
{
    static std::regex imgUrlTemplate {
        R"_(https*:\/\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\/\/=]*))_"
    };
    return imgUrlTemplate;
}

QString &TextSearcher::GetDefaultUserAgent()
{
    static QString userAgent = QLatin1Literal("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) "
                                              "Gecko/20100101 Firefox/15.0.1");
    return userAgent;
}
