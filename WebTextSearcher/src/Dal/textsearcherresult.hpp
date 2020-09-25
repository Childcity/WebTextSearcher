#ifndef TEXTSEARCHERRESULT_H
#define TEXTSEARCHERRESULT_H

#include "textsearcherstatus.hpp"

#include <functional>


struct TextSearcherResult {
public:
    TextSearcherResult &setUrl(QString url)
    {
        url_ = std::move(url);
        urlHash_ = qHash(url);
        return *this;
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
