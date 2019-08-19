#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "GOAP/ExamplesRunner.h"



int main(int argc, char *argv[])
{
    ExamplesRunner::getInstance()->runMe();


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
