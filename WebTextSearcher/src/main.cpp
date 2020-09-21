#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVector>

#include <QThreadPool>
#include <QTimer>

#include "Dal/textsearcher.h"

#include "Utils/Utils.hpp"


void slotResult(TextSearcherResult res)
{
    DEBUG(res.url << ": " << res.status << res.error);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    {
        // set default font
        app.setFont({ "Ubuntu", 12 });
    }

    {
        // register qml types
    }

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/src/Qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QTimer::singleShot(0, &app, [&app] {
        size_t maxUrlNumber = 5;
        size_t scannedUrlNumber = 0;
        Utils::SafeUrlQueue queue;
        QThreadPool workers;
        workers.setMaxThreadCount(4);
        DEBUG("MAIN" << QThread::currentThreadId());

        queue.push("http://habr.com/");
        QString url;
        while (scannedUrlNumber < maxUrlNumber) {

            if (! queue.tryPop(url)) {
                if (workers.activeThreadCount() == 0) {
                    DEBUG("Fetched only" << scannedUrlNumber)
                    break; // means no more urls to fetch
                } else {
                    continue; // means queue is empty and we should wait for felling
                }
            }

            auto sercher = new TextSearcher(queue, std::move(url));
            app.connect(sercher, &TextSearcher::sigResult, &app, slotResult, Qt::QueuedConnection);
            workers.start(sercher);

            scannedUrlNumber++;
        }
    });

    return app.exec();
}
