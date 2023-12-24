#include "phonemodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    //    QStringList paths = QCoreApplication::libraryPaths();
    //    paths.append(".");
    //    paths.append("imageformats");
    //    paths.append("platforms");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<PhoneModel>("MyModule", 1, 0, "PhoneModel");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
