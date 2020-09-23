#ifndef TEXTSEARCHERRESULT_H
#define TEXTSEARCHERRESULT_H

#include "textsearcherstatus.hpp"


struct TextSearcherResult {
//public:
//    explicit TextSearcherResult() noexcept = default;
//
//public:
    QString url;
    TextSearcherStatus::Value status;
    QString error;
};

Q_DECLARE_METATYPE(TextSearcherResult);


#endif // TEXTSEARCHERRESULT_H
