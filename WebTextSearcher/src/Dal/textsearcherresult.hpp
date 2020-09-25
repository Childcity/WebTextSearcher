#ifndef TEXTSEARCHERRESULT_H
#define TEXTSEARCHERRESULT_H

#include "textsearcherstatus.hpp"

#include <functional>


struct TextSearcherResult {
public:
    TextSearcherResult()
    {}

    TextSearcherResult(QString url, TextSearcherStatus status_, QString error_ = {})
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
    TextSearcherStatus status;
    QString error;

private:
    QString url_;
    size_t urlHash_; // hash for better performance in search
};

Q_DECLARE_METATYPE(TextSearcherResult);


#endif // TEXTSEARCHERRESULT_H
