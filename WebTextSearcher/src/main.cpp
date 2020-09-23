#include "Models/searchedurlsmodel.h"
#include "Dal/downloadmanager.h"

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>


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
        qmlRegisterType<Models::SerchedUrlsModel>("SerchedUrlsModel", 1, 0, "SerchedUrlsModel");
        qmlRegisterUncreatableType<TextSearcherStatus>("TextSearcherStatus", 1, 0, "TextSearcherStatus", "TextSearcherStatus is an Enum and can't b instantiated!");
        qRegisterMetaType<TextSearcherStatus::Value>("TextSearcherStatus::Value");
    }

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/src/Qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    DownloadManager m;
    //QTimer::singleShot(0, &m, &DownloadManager::slotStartSearcher);

    return app.exec();
}
