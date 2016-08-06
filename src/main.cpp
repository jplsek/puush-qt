#include <QtGui>
#include <QMessageBox>
#include <QApplication>

#include "window.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(systray);

    QApplication app(argc, argv);
    QApplication::setOrganizationName("puush-qt");
    QApplication::setApplicationName("puush-qt");

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

    Window window;

    return app.exec();
}
