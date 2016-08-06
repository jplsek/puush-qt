#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QLabel>
#include <QAction>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QSettings>
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

private:
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();
    void setDefaults();
    bool isLoggedIn();

    QGroupBox *iconGroupBox;

    QGroupBox *messageGroupBox;
    QPushButton *submitButton;

    QLabel *emailLabel;
    QLabel *passwordLabel;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;

    QLabel *authMessage;

    QAction *uploadAction;
    QAction *fullScreenAction;
    QAction *selectAreaAction;
    QAction *activeAction;

    QAction *settingsAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QUrl lastUrl;

    QTimer *timer;

    QNetworkReply *authReply;

    QSettings s;

    QString getFileName();

    int numTime = 0;
};

#endif
