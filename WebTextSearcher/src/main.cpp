#include "Models/searchedurlsmodel.h"
#include "Dal/searchmanager.h"

#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    qInstallMessageHandler(Utils::CustomMessageHandler::Handler);
    app.setOrganizationName("Childcity");
    app.setApplicationName("Web Text Searcher");

    {
        // set default font
        app.setFont({ "Ubuntu", 12 });
    }

    {
        // register qml types
        qmlRegisterType<Dal::SearchManager>("SearchManager", 1, 0, "SearchManager");
        qmlRegisterType<Models::SerchedUrlsModel>("SerchedUrlsModel", 1, 0, "SerchedUrlsModel");
        qmlRegisterUncreatableType<Dal::SearchStatusTypeClass>("SearchStatusType", 1, 0, "SearchStatusType", "SearchStatusType is an Enum and can't b instantiated!");
        qRegisterMetaType<Dal::SearchStatusType>("SearchStatusType");
        qmlRegisterUncreatableType<Dal::SearchManagerStatusClass>("SearchManagerStatus", 1, 0, "SearchManagerStatus", "SearchManagerStatus is an Enum and can't b instantiated!");
        qRegisterMetaType<Dal::SearchManagerStatus>("SearchManagerStatus");
    }

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/src/Qml/MainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}



