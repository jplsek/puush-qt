#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QtQuick>

#include "api/apihist.h"
#include "api/apidel.h"

#include "settings.h"
#include "history.h"

class Systray : public QObject {
    Q_OBJECT

public:
    explicit Systray(QObject *parent = 0);

    Q_INVOKABLE void openAccount();

public slots:
    Q_INVOKABLE void uploadFile();
    Q_INVOKABLE void fullScreenScreenshot();
    Q_INVOKABLE void selectAreaScreenshot();
    Q_INVOKABLE void activeWindowScreenshot();
    Q_INVOKABLE void activeWindowScreenshotTimed();
    Q_INVOKABLE void uploadClipboard();
    Q_INVOKABLE void togglePuush();
    Q_INVOKABLE void openSaveDirectorySetEnabled(bool);
    Q_INVOKABLE void updateHistory();

private slots:
    void setTrayIcon(QString);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void openSettings(int = 0);

    void puushStarted();
    void puushDone(QString);
    void screenshotDone(int, QString, QString);
    void updateActiveMessage();

    void openSaveDirectory();

    void updateHistoryMenu(QList<ApiHist::HistData>);
    void deleteDone(ApiDel *);

private:
    void createActions();
    void createTrayIcon();
    bool isLoggedIn(bool);
    void openUrl(QUrl);

    History *history;

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
    QAction *uploadFileAction;
    QAction *uploadClipboardAction;
    QAction *fullScreenAction;
    QAction *selectAreaAction;
    QAction *activeAction;

    QMenu *historyMenu;

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

    void historyPlaceholder();
    void updateHistoryAfterTimeout();

    int defaultSelectionTimeout = 5;
    int numTime = 0;

signals:
    void signalOpenSettings(int tab);
};

#endif
