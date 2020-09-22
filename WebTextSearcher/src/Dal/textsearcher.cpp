#include "textsearcher.h"
#include "Net/idownloader.h"
#include "Net/downloader.h"
#include "Net/networkerror.h"

#include <QTimer>


TextSearcher::TextSearcher(std::shared_ptr<Utils::SafeUrlQueue> urlsQueue,
                           QString urlToFetch, QObject *parent) noexcept
    : QObject(parent)
    , urlsQueue_(std::move(urlsQueue))
    , urlToFetch_(std::move(urlToFetch))
{}

void TextSearcher::run()
{
    using namespace std::chrono_literals;
    using namespace Net;

    TextSearcherResult result;
    std::unique_ptr<IDownloader> downloader = std::make_unique<Downloader>();

    downloader->setTimeout(3s);

    QNetworkRequest request(urlToFetch_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::UserVerifiedRedirectPolicy);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setHeader(QNetworkRequest::UserAgentHeader, GetDefaultUserAgent());

    try {
        const QByteArray page = downloader->get(request);

        // parse response on urls and serching text
        {
            auto matchesBegin = std::regex_token_iterator(page.cbegin(), page.cend(), GetLinkRegex());
            auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();

            if (matchesBegin == matchesEnd) {
                result.status = TextSearcherStatus::NotFound;
            } else {
                result.status = TextSearcherStatus::Found;
                std::for_each(matchesBegin, matchesEnd, [this](const std::string url) {
                    urlsQueue_->push(std::move(url));
                });
            }
        }

    } catch (const NetworkError &ex) {
        result.status = TextSearcherStatus::Error;
        result.error = ex.what();
    }

    //DEBUG(result.status << result.url);

    result.url = std::move(urlToFetch_);
    emit sigResult(std::move(result));
}
