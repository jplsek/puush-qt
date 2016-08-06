#include <QtGui>
#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "window.h"
#include "screenshot.h"
#include "upload.h"

QString puushUrl = "https://puush.me/api/";

Window::Window() {
    setDefaults();
    createMessageGroupBox();

    createActions();
    createTrayIcon();

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitInfo()));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    setTrayIcon(":/images/icon.svg.png");
    setAppIcon(":/images/icon.svg.png");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(messageGroupBox);
    setLayout(mainLayout);

    trayIcon->show();

    setWindowTitle(tr("puush-qt"));
    resize(400, 300);
}

void Window::setDefaults() {
    if (!s.contains("quality"))
        s.setValue("quality", "90");
}

void Window::setVisible(bool visible) {
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event) {
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Window::setTrayIcon(QString image) {
    QIcon icon = QIcon(image), tr("Icon");
    trayIcon->setIcon(icon);
}

void Window::setAppIcon(QString image) {
    QIcon icon = QIcon(image), tr("Icon");
    setWindowIcon(icon);
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        trayIconMenu->popup(QCursor::pos());
        break;
    default:
        break;
    }
}

void Window::submitInfo() {
    s.setValue("email", emailEdit->text());

    QString email = emailEdit->text();
    QString password = passwordEdit->text();

    QNetworkAccessManager *nm = new QNetworkAccessManager(this);
    QUrlQuery postData;
    postData.addQueryItem("e", QUrl::toPercentEncoding(email));
    postData.addQueryItem("p", QUrl::toPercentEncoding(password));
    QNetworkRequest request(QUrl(puushUrl + "auth"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    authReply = nm->post(request, postData.toString().toUtf8());
    connect(authReply, SIGNAL(finished()), this, SLOT(authDone()));
}

void Window::authDone() {
    QString output = authReply->readAll();
    authReply->deleteLater();
    //qDebug() << output;

    const char *text = "";

    if (output == "-1") {
        text = "Error sending credentials!";
    } else {
        text = "Authentication sucesssful!";
        QStringList pieces = output.split(",");
        QString key = pieces[1];
        s.setValue("key", key);
    }

    authMessage->setText(tr(text));
}

void Window::messageClicked() {
    bool response = QDesktopServices::openUrl(lastUrl);

    if (!response) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), tr("There was an issue opening the URL. Is your default browser set?"), icon);
    }
}

void Window::createMessageGroupBox() {
    messageGroupBox = new QGroupBox(tr("Settings"));

    emailLabel = new QLabel(tr("Email:"));

    if (s.contains("email")) {
        emailEdit = new QLineEdit(s.value("email").toString());
    } else {
        emailEdit = new QLineEdit();
    }

    passwordLabel = new QLabel(tr("Password:"));
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);

    authMessage = new QLabel();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);

    QGridLayout *messageLayout = new QGridLayout;
    messageLayout->addWidget(emailLabel, 0, 0);
    messageLayout->addWidget(emailEdit, 0, 1, 1, 4);
    messageLayout->addWidget(passwordLabel, 1, 0);
    messageLayout->addWidget(passwordEdit, 1, 1, 1, 4);
    messageLayout->addWidget(authMessage, 2, 0, 1, 2);
    messageLayout->addWidget(submitButton, 5, 4);

    messageGroupBox->setLayout(messageLayout);
}

void Window::createActions() {
    uploadAction = new QAction(tr("&Upload..."), this);
    connect(uploadAction, SIGNAL(triggered()), this, SLOT(uploadFile()));

    fullScreenAction = new QAction(tr("&Full screen"), this);
    connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreenScreenshot()));

    selectAreaAction = new QAction(tr("&Select area"), this);
    connect(selectAreaAction, SIGNAL(triggered()), this, SLOT(selectAreaScreenshot()));

    activeAction = new QAction(tr("&Ative window"), this);
    connect(activeAction, SIGNAL(triggered()), this, SLOT(activeWindowScreenshot()));

    settingsAction = new QAction(tr("S&ettings..."), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(uploadAction);
    trayIconMenu->addAction(fullScreenAction);
    trayIconMenu->addAction(selectAreaAction);
    trayIconMenu->addAction(activeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Window::openSettings() {
    showNormal();
    // if it's already open, it won't raise, so we force it.
    raise();
}

bool Window::isLoggedIn() {
    if (s.contains("key"))
        return true;

    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    trayIcon->showMessage(tr("API Key Not Set"), tr("Have you authenticated?"), icon);

    return false;
}

void Window::uploadFile() {
    if (!isLoggedIn()) return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Upload file"));

    if (fileName == "") return;

    Upload *u = new Upload(fileName);

    connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
    connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));
}

QString Window::getFileName() {
    return "/tmp/ss-" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
}

void Window::fullScreenScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getFileName();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->fullScreen();
}

void Window::selectAreaScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getFileName();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->selectArea();
}

void Window::activeWindowScreenshot() {
    if (!isLoggedIn()) return;

    numTime = 5;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateActiveMessage()));
    timer->start(1000);
}

void Window::updateActiveMessage() {
    if (numTime < 1) {
        timer->stop();

        QString fileName = getFileName();
        Screenshot *ss = new Screenshot(fileName);
        connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
        ss->activeWindow();

        return;
    }

    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();

    trayIcon->showMessage(tr("Select a window"), tr("Taking a screenshot in ") + QString::number(numTime), icon);
    --numTime;
}

void Window::puushStarted() {
    setTrayIcon(":/images/icon-uploading.svg.png");
}

void Window::screenshotDone(int returnCode, QString fileName, QString output) {
    if (returnCode != 0) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), output, icon);
        return;
    }

    Upload *u = new Upload(fileName);

    connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
    connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));
}

void Window::puushDone(int returnCode, QString output) {
    if (returnCode != 0) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), output, icon);
        return;
    }

    //qDebug() << output;

    QStringList pieces = output.split(",");
    QString code = pieces[0];
    QString url = "";

    if (pieces.length() > 1)
        url = pieces[1];

    setTrayIcon(":/images/icon.svg.png");

    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();

    // show error to user
    if (code == "-1") {
        trayIcon->showMessage(tr("Error!"), tr("Error uploading file. Have you authenticated?"), icon);
        return;
    } else if (code == "-2") {
        trayIcon->showMessage(tr("Error!"), tr("Error uploading file. This might be a bug with puush-qt."), icon);
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    lastUrl = QUrl(url);
    clipboard->setText(url);

    trayIcon->showMessage(tr("Success!"), url + tr("\nThe url was copied to your clipboard!"), icon);
}
