//
// Created by childcity on 12.09.20.
//

#ifndef WEBTEXTSEARCHER_UTILS_H
#define WEBTEXTSEARCHER_UTILS_H

#include "ThreadSafeQueue.hpp"

#include <QDebug>
#include <QDateTime>
#include <memory>


#define DEBUG(msg_) qDebug() << Utils::DebugPrinter(__FILE__, __LINE__) << msg_ << "<";
//#define DEBUGGAM(msg_) // Off debug


namespace Utils {


// We store std::string instead of QString, because it much faster and take less memory
using SafeUrlQueue = childcity::threadsafeqeue::ThreadSafeQueue<std::string>;

struct DebugPrinter {
    const char *file;
    int line;

    DebugPrinter(const char *file_, int line_)
        : file(file_)
        , line(line_)
    {}

    friend QDebug operator<<(QDebug stream, const DebugPrinter printer)
    {
        stream << QDateTime::currentDateTime().toString("mm:ss:zzz").toLatin1().data()
               << (QByteArray(printer.file) + '[' + QByteArray::number(printer.line) + "]>").data();
        return stream;
    }
};

struct QObjectDeleteLater {
    void operator()(QObject *o)
    {
        if (o->parent()) {
            o->deleteLater();
        } else {
            delete o;
        }
    }
};

template<typename T>
using qt_unique_ptr = std::unique_ptr<T, QObjectDeleteLater>;

template<class T, class... _Types, std::enable_if_t<! std::is_array<T>::value, int> = 0>
inline qt_unique_ptr<T> qt_make_unique(_Types &&... _Args)
{
    return (qt_unique_ptr<T>(new T(std::forward<_Types>(_Args)...)));
}


template<typename Functor>
struct OnExit {
    Functor func;
    OnExit(Functor &&f)
        : func(std::forward<Functor>(f))
    {}
    ~OnExit()
    {
        func();
    }
};

template<typename Functor>
OnExit(Functor &&frv) -> OnExit<Functor>;


// Default message handler to be called to bypass all other warnings.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);


struct CustomMessageHandler {
    static void Handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        // Suppress warning with cache, as it is bug: https://github.com/ariya/phantomjs/issues/13165
        // Suppress warning with QNetworkReplyImplPrivate https://github.com/raelgc/scudcloud/issues/546
        switch (type) {
            case QtDebugMsg: {
                if (! msg.contains("setCachingEnabled"))
                    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
            } break;
            case QtWarningMsg: {
                if (! msg.contains("QNetworkReplyImpl", Qt::CaseInsensitive))
                    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
            } break;
            case QtCriticalMsg: {
                if (! msg.contains("QNetworkReplyImpl", Qt::CaseInsensitive))
                    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
            } break;
            default:
                (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
                break;
        }
    }
};


} // namespace Utils


#endif //WEBTEXTSEARCHER_UTILS_H
