#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QtQuick>

#include "api/apiauth.h"
#include "api/apihist.h"

#include "settings.h"

class Systray : public QObject {
    Q_OBJECT

public:
    explicit Systray(QObject *parent = 0);

    Q_INVOKABLE void openAccount();

private slots:
    void setTrayIcon(QString);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void openSettings();
    void uploadFile();
    void fullScreenScreenshot();
    void selectAreaScreenshot();
    void activeWindowScreenshot();
    void activeWindowScreenshotTimed();

    void puushStarted();
    void puushDone(int, QString);
    void screenshotDone(int, QString, QString);
    void updateActiveMessage();

    void openSaveDirectory();

private:
    void createActions();
    void createTrayIcon();
    bool isLoggedIn();
    void openUrl(QUrl);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ethans new section
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    enum TrayAction {
        INVALID_TRAY_ACTION = 0, // the settings should default to this
        OPEN_SETTINGS = 1,
        OPEN_UPLOADS  = 2,
    };
    enum CompressionPhilosophy {
        PNG_ALWAYS,
        SMALLER,
        JPG_ALWAYS,
    };
    enum FullscreenCaptureMode {
        ALL_DESKTOPS,
        CURRENT_DESKTOP, // the desktop the cursor is on
        PRIMARY_DESKTOP,
    };
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void connectSignals();
    void doDoubleClickAction();

    QAction *myAccountAction;
    QAction *uploadAction;
    QAction *fullScreenAction;
    QAction *selectAreaAction;
    QAction *activeAction;

    QAction *settingsAction;
    QAction *openSaveDirectoryAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QUrl lastUrl = QUrl("");

    QTimer *timer;

    Settings s;

    QString getFileName();
    QString getTempPath();
    QString getSaveDirectory();
    QString getSavePath();

    int defaultSelectionTimeout = 5;
    int numTime = 0;

signals:
    void signalOpenSettings();
};

#endif
