#ifndef TEXTSEARCHERSTATUS_H
#define TEXTSEARCHERSTATUS_H

#include "searchstatustype.hpp"
#include <functional>


namespace Dal {


struct TextSearcherStatus {
public:
    TextSearcherStatus()
    {}

    TextSearcherStatus(QString url, SearchStatusType status_, QString error_ = {})
        : status(status_)
        , error(std::move(error_))
    {
        setUrl(std::move(url));
    }

    void setUrl(QString url)
    {
        url_ = std::move(url);
        urlHash_ = qHash(url_);
        assert(urlHash_ != 0); // url and urlHash_ mustn't be Empty!
    }

    QString url() const
    {
        return url_;
    }

    size_t hash() const
    {
        return urlHash_;
    }

public:
    SearchStatusType status;
    QString error;

private:
    QString url_;
    size_t urlHash_; // hash for better performance in search
};


} // namespace Dal

Q_DECLARE_METATYPE(Dal::TextSearcherStatus);

#endif // TEXTSEARCHERSTATUS_H
