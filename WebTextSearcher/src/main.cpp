#include <QFont>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVector>

#include <QtConcurrent>

int main(int argc, char *argv[])
{
    //QtConcurrent::mappedReduced()

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

    return app.exec();
}
