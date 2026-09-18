#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlextensionplugin.h>

#include "core/config.h"
#include "core/logger.h"

using devops::core::Config;
using devops::core::Logger;

Q_IMPORT_QML_PLUGIN(DevopsUiPlugin)

using namespace devops::core;

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    Config::instance().loadFromFile(".env");
    Logger::instance().info("main", "Starting devops-console-app");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.loadFromModule("DevopsApp", "Main");

    return app.exec();
}
