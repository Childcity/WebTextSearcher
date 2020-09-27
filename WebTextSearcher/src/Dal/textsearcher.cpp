#include "textsearcher.h"

#include "Net/operationcancelederror.h"
#include "Net/networkerror.h"
#include "Net/idownloader.h"
#include "Net/downloader.h"


namespace Dal {


TextSearcher::TextSearcher(std::shared_ptr<Utils::SafeUrlQueue> urlsQueue,
                           QString urlToFetch, QString serchedText,
                           int urlDownloadingTimeout, const std::atomic_bool &isCanceled,
                           QObject *parent) noexcept
    : QObject(parent)
    , urlsQueue_(std::move(urlsQueue))
    , urlToFetch_(std::move(urlToFetch))
    , serchedText_(std::move(serchedText))
    , urlDownloadingTimeout_(urlDownloadingTimeout)
    , isCanceled_(isCanceled)
{}

TextSearcher::~TextSearcher()
{}

void TextSearcher::run()
{
    using namespace std::chrono;
    using namespace Net;

    TextSearcherStatus result;
    std::unique_ptr<IDownloader> downloader = std::make_unique<Downloader>(isCanceled_);

    downloader->setTimeout(milliseconds(urlDownloadingTimeout_));

    QNetworkRequest request(urlToFetch_);
    request.setMaximumRedirectsAllowed(5);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::UserVerifiedRedirectPolicy);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
    request.setHeader(QNetworkRequest::UserAgentHeader, GetDefaultUserAgent());

    try {
        if (isCanceled_.load(std::memory_order_relaxed))
            throw OperationCanceledError();

        const QByteArray page = downloader->get(request);


        // Search text in page
        {
            const QByteArray ba = serchedText_.toUtf8();
            auto found = std::search(page.cbegin(), page.cend(), ba.cbegin(), ba.cend());
            result.status = (found != page.cend())
                                ? SearchStatusType::Found
                                : SearchStatusType::NotFound;
        }

        // Search urls in page
        {
            auto matchesBegin = std::regex_token_iterator(page.cbegin(), page.cend(), GetLinkRegex());
            auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();

            std::for_each(matchesBegin, matchesEnd, [this](const std::string url) {
                urlsQueue_->push(std::move(url));
            });
        }

    } catch (const std::runtime_error &ex) {
        result.status = SearchStatusType::Error;
        result.error = ex.what();
    }

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

} // namespace Dal
