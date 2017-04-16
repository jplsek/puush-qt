#include <QMessageBox>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "systray.h"
#include "information.h"
#include "authentication.h"
#include "history.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    QApplication::setOrganizationName("puush-qt");
    QApplication::setApplicationName("puush-qt");
    QApplication::setApplicationVersion("0.1");

    // TODO
    // keep only one instance of the application up at a time
    //{
        //QMessageBox::warning(0, "puush-qt", "puush-qt " + QObject::tr("is already running."), QObject::tr("Ok"));
        //return 2;
    //}

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, "puush-qt", QObject::tr("A system tray could not be detected on your system!"));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Systray *systray = new Systray();
    Information *information = new Information();
    Authentication *authentication = new Authentication();
    History *history = new History();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("information", information);
    engine.rootContext()->setContextProperty("systemTray", systray);
    engine.rootContext()->setContextProperty("authentication", authentication);
    engine.rootContext()->setContextProperty("history", history);
    engine.load(QUrl(QLatin1String("qrc:/ui/ApplicationWindow.qml")));

    return app.exec();
}
