#include "textsearcher.h"
#include "Net/downloader.h"
#include "Net/networkerror.h"

#include <QTimer>


TextSearcher::TextSearcher(Utils::SafeUrlQueue &urlsQueue, QString urlToFetch)
    : urlsQueue_(urlsQueue)
    , urlToFetch_(std::move(urlToFetch))
{}

void TextSearcher::run()
{
    using namespace std::chrono_literals;

    TextSearcherResult result;
    Net::Downloader downloader;

    downloader.setTimeout(5s);

    QNetworkRequest request(urlToFetch_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::UserVerifiedRedirectPolicy);
    request.setHeader(QNetworkRequest::UserAgentHeader, GetDefaultUserAgent());

    try {
        const QByteArray page = downloader.get(request);

        // parse response with links to different images and fill urls list
        {
            auto matchesBegin = std::regex_token_iterator(page.cbegin(), page.cend(), GetLinkRegex());
            auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();

            if (matchesBegin == matchesEnd) {
                result.status = TextSearcherStatus::NotFound;
            } else {
                result.status = TextSearcherStatus::Found;
                std::for_each(matchesBegin, matchesEnd, [this](const std::string url) {
                    urlsQueue_.push(QString::fromStdString(url));
                });
            }
        }

    } catch (const Net::NetworkError &ex) {
        result.status = TextSearcherStatus::Error;
        result.error = ex.what();
    }

    //DEBUG(result.status << result.url);

    result.url = std::move(urlToFetch_);
    emit sigResult(result);
}
