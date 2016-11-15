#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QLabel>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QSettings>
#include <QSlider>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QRadioButton>
#include <QStackedLayout>

#include "api/apiauth.h"
#include "settings.h"


class QTabWidget;

class Window : public QDialog {
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void setTrayIcon(QString);
    void setAppIcon(QString);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void submitInfo();
    void logout();
    void messageClicked();
    void openSettings();
    void openAccount();
    void uploadFile();
    void fullScreenScreenshot();
    void selectAreaScreenshot();
    void activeWindowScreenshot();
    void puushStarted();
    void puushDone(int, QString);
    void screenshotDone(int, QString, QString);
    void authDone(ApiAuth *);
    void updateActiveMessage();
    void openSaveDirectory();

    void qualityChanged(int);
    void saveEnabledChanged(int);
    void saveNameChanged();
    void emailChanged();
    void openSavePath();

    void resetSettings();

private:
    void createGroupBoxes();
    void createActions();
    void createSettingsSlots();
    void createTrayIcon();
    void setDefaults();
    bool isLoggedIn();
    void openUrl(QUrl);
    void setSavePath(QString);
    void createLoginBox();
    void createLoggedinBox();
    void selectLoginBox();
    void selectLoggedinBox();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ethans new section
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    enum TrayAction {
        INVALID_TRAY_ACTION = 0, // the settings should default to this
        OPEN_SETTINGS = 1,
        OPEN_CAPTURE  = 2,
        OPEN_UPLOADS  = 3,
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

    QTabWidget *createTabs();
    QWidget *createTabGeneral();
    QWidget *createTabKeyBindings();
    QWidget *createTabAccount();
    QWidget *createTabAdvanced();
    QWidget *createTabHistory();
    QWidget *createTabAbout();

    void connectSignals();

    QTabWidget *tabs;

    QCheckBox *enablePuushSound;
    QCheckBox *enableLocalSave;
    QLineEdit *localSaveLocation;
    QCheckBox *enableLinkToClipboard;
    QCheckBox *enableLinkToBrowser;

    QRadioButton *trayDoubleClickSettings;
    QRadioButton *trayDoubleClickCapture;
    QRadioButton *trayDoubleClickUpload;

    QRadioButton *compressionAlways;
    QRadioButton *compressionSmart;

    QCheckBox *contextShowExplorerContext;

    QRadioButton *fullscreenCaptureAll;
    QRadioButton *fullscreenCaptureCursor;
    QRadioButton *fullscreenCapturePrimary;

    QCheckBox *dangerousExperimentalEnable;
    QCheckBox *dangerousNoSelectionRect;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    QStackedLayout *accountBox;

private slots:
    void soundEnabledChanged(bool);
    void enableLocalSaveChanged(bool);

    void localSaveLocChanged();

    void enableLinkToClipboardChanged(bool);
    void enableLinkToBrowserChanged(bool);

    void trayDoubleClickedSettingsChanged(bool);
    void trayDoubleClickedCaptureChanged(bool);
    void trayDoubleClickedUploadChanged(bool);

    void compressionAlwaysChanged(bool);
    void compressionSmartChanged(bool);

    void contextShowExplorerChanged(bool);

    void fullscreenCaptureAllChanged(bool);
    void fullscreenCaptureCursorChanged(bool);
    void fullscreenCapturePrimaryChanged(bool);

    void dangerousExperimentalEnableChanged(bool);
    void dangerousNoSelectionRectChanged(bool);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:

    QGroupBox *iconGroupBox;

    QGroupBox *authGroupBox;
    QGroupBox *saveGroupBox;
    QGroupBox *puushGroupBox;

    QPushButton *submitButton;
    QPushButton *logoutButton;
    QPushButton *resetButton;
    QPushButton *selectSavePathButton;
    QPushButton *aboutQt;
    QPushButton *myAccount;

    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLabel *authMessage;
    QLabel *emailLabel;
    QLabel *apiKeyLabel;
    QLabel *accountTypeLabel;
    QLabel *expiryLabel;
    QLabel *diskLabel;

    QLabel *saveEnabledLabel;
    QCheckBox *saveEnabled;
    QLineEdit *saveNameEdit;

    QSlider *qualitySlider;

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

    QUrl lastUrl;

    QTimer *timer;

    QNetworkReply *authReply;

    Settings s;
    ApiAuth::AuthData userData;

    QString getFileName();
    QString getSavePath();
    QString getSaveName();

    int numTime = 0;
};

#endif
