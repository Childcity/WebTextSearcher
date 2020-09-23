#ifndef TEXTSEARCHERSTATUS_H
#define TEXTSEARCHERSTATUS_H

#include <QObject>


struct TextSearcherStatusClass {
    Q_GADGET

public:
    TextSearcherStatusClass() = delete;

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

using TextSearcherStatus = TextSearcherStatusClass::Value;


#endif // TEXTSEARCHERSTATUS_H
