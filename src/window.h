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
    void uploadFile();
    void fullScreenScreenshot();
    void selectAreaScreenshot();
    void activeWindowScreenshot();
    void puushStarted();
    void puushDone(int, QString);
    void screenshotDone(int, QString, QString);
    void authDone();
    void updateActiveMessage();
    void openSaveDirectory();

    void qualityChanged(int);
    void saveEnabledChanged(int);
    void savePathChanged();
    void saveNameChanged();

    void resetSettings();

private:
    void createGroupBoxes();
    void createActions();
    void createSettingsSlots();
    void createTrayIcon();
    void setDefaults();
    bool isLoggedIn();

    QGroupBox *iconGroupBox;

    QGroupBox *authGroupBox;
    QGroupBox *saveGroupBox;
    QGroupBox *puushGroupBox;

    QPushButton *submitButton;
    QPushButton *logoutButton;
    QPushButton *resetButton;

    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLabel *authMessage;

    QLabel *saveEnabledLabel;
    QCheckBox *saveEnabled;
    QLineEdit *savePathEdit;
    QLineEdit *saveNameEdit;

    QSlider *qualitySlider;

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

    QSettings s;

    QString getFileName();
    QString getSavePath();
    QString getSaveName();

    int numTime = 0;
};

#endif
