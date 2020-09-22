#ifndef TEXTSEARCHERSTATUS_H
#define TEXTSEARCHERSTATUS_H

#include <QObject>


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


#endif // TEXTSEARCHERSTATUS_H
