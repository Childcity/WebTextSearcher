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
    static std::atomic<int> r = 0;
    r++;
    DEBUG(res.url << ": " << res.status << res.error << r);
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

    Utils::SafeUrlQueue queue;
    std::thread { [&] {
        size_t maxUrlNumber = 1000;
        size_t scannedUrlNumber = 0;
        QThreadPool workers;
        workers.setMaxThreadCount(100);
        //DEBUG("MAIN" << QThread::currentThreadId());

        queue.push("http://www.google.com/search?q=qthread");
        QString url;
        while (scannedUrlNumber < maxUrlNumber) {

            if (! queue.tryPop(url)) {
                if (workers.activeThreadCount() == 0) {
                    break; // means no more urls to fetch
                } else {
                    continue; // means queue is empty and we should wait for felling
                }
            }

            auto sercher = new TextSearcher(queue, std::move(url));
            app.connect(sercher, &TextSearcher::sigResult, &app, &::slotResult, Qt::QueuedConnection);
            workers.start(sercher);

            scannedUrlNumber++;
        }
        workers.waitForDone();
    } }.detach();

    return app.exec();
}
