#include <QtGui>
#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QProcess>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "window.h"
#include "screenshot.h"
#include "upload.h"

QString puushUrl = "https://puush.me/api/";

Window::Window() {
    setDefaults();
    createGroupBoxes();

    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    setTrayIcon(":/images/icon.svg.png");
    setAppIcon(":/images/icon.svg.png");

    resetButton = new QPushButton(tr("Reset Settings"));
    resetButton->setDefault(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(authGroupBox);
    mainLayout->addWidget(puushGroupBox);
    mainLayout->addWidget(saveGroupBox);
    mainLayout->addWidget(resetButton);
    setLayout(mainLayout);

    createSettingsSlots();

    trayIcon->show();

    setWindowTitle(tr("puush-qt"));
    resize(400, 300);
}

void Window::setDefaults() {
    if (!s.contains("key"))
        s.setValue("key", "");
    if (!s.contains("quality"))
        s.setValue("quality", 90);
    if (!s.contains("save-path"))
        s.setValue("save-path", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    if (!s.contains("save-name"))
        s.setValue("save-name", "ss-yyyy-MM-dd_hh-mm-ss");
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

    QString email    = emailEdit->text();
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

void Window::logout(){
    s.setValue("key", "");
    authMessage->setText(tr("Logged out"));
}

void Window::authDone() {
    if(authReply->error() != QNetworkReply::NoError){
        authMessage->setText(tr("Error sending auth info: ") + authReply->errorString());
        return;
    }

    QString output = authReply->readAll();
    authReply->deleteLater();

    if (output == "-1") {
        authMessage->setText(tr("Invalid credentials or error sending them!"));
    } else {
        authMessage->setText(tr("Authentication sucesssful!"));
        QStringList pieces = output.split(",");
        if(pieces.length() > 1){
            QString key = pieces[1];
            s.setValue("key", key);
        } else {
            authMessage->setText(tr("Error: Malformed response from Puush"));
        }
    }
}

void Window::messageClicked() {
    bool response = QDesktopServices::openUrl(lastUrl);

    if (!response) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), tr("There was an issue opening the URL. Is your default browser set?"), icon);
    }
}

void Window::createGroupBoxes() {
    authGroupBox  = new QGroupBox(tr("Authentication"));
    puushGroupBox = new QGroupBox(tr("Puush Settings"));
    saveGroupBox  = new QGroupBox(tr("Local Save Settings"));

    // Auth Settings

    emailEdit = new QLineEdit(s.value("email", "").toString());

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);

    authMessage = new QLabel();
    if(s.value("key") == "")
        authMessage->setText("Not logged in");

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    logoutButton = new QPushButton(tr("Logout"));
    logoutButton->setDefault(true);

    QHBoxLayout *buttonPair = new QHBoxLayout();
    buttonPair->addWidget(logoutButton);
    buttonPair->addWidget(submitButton);

    QFormLayout *authLayout = new QFormLayout();
    authLayout->addRow(tr("Email:"),    emailEdit);
    authLayout->addRow(tr("Password:"), passwordEdit);
    authLayout->addRow(authMessage);
    authLayout->addRow(buttonPair);

    // Puush settings

    qualitySlider = new QSlider(Qt::Horizontal);
    qualitySlider->setRange(0, 100);
    qualitySlider->setTickPosition(QSlider::TicksBelow);
    qualitySlider->setTickInterval(10);
    qualitySlider->setValue(s.value("quality", 90).toInt());

    QFormLayout *puushLayout = new QFormLayout();
    puushLayout->addRow(tr("Quality:"), qualitySlider);

    // Save settings

    saveEnabled = new QCheckBox("Save screenshot to file");
    savePathEdit = new QLineEdit(s.value("save-path").toString());
    saveNameEdit = new QLineEdit(s.value("save-name").toString());

    saveEnabled->setCheckState(s.value("save-enabled").toBool() ? Qt::Checked : Qt::Unchecked);
    savePathEdit->setEnabled(s.value("save-enabled").toBool());
    // saveNameEdit->setEnabled(s.value("save-enabled").toBool()); // disabled because NYI
    saveNameEdit->setEnabled(false);

    QFormLayout *saveLayout = new QFormLayout();
    saveLayout->addRow(saveEnabled);
    saveLayout->addRow(tr("Location:"),  savePathEdit);
    saveLayout->addRow(tr("File Name:"), saveNameEdit);

     authGroupBox->setLayout( authLayout);
    puushGroupBox->setLayout(puushLayout);
     saveGroupBox->setLayout( saveLayout);
}

void Window::createActions() {
    uploadAction = new QAction(tr("&Upload..."), this);
    connect(uploadAction, SIGNAL(triggered()), this, SLOT(uploadFile()));

    fullScreenAction = new QAction(tr("&Full screen"), this);
    connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreenScreenshot()));

    selectAreaAction = new QAction(tr("&Select area"), this);
    connect(selectAreaAction, SIGNAL(triggered()), this, SLOT(selectAreaScreenshot()));

    activeAction = new QAction(tr("&Active window"), this);
    connect(activeAction, SIGNAL(triggered()), this, SLOT(activeWindowScreenshot()));

    settingsAction = new QAction(tr("S&ettings..."), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    openSaveDirectoryAction = new QAction(tr("&Open screenshot directory"), this);
    connect(openSaveDirectoryAction, SIGNAL(triggered()), this, SLOT(openSaveDirectory()));
}

void Window::createSettingsSlots(){
    connect(submitButton,  SIGNAL(clicked()),         this, SLOT(submitInfo()));
    connect(logoutButton,  SIGNAL(clicked()),         this, SLOT(logout()));

    connect(qualitySlider, SIGNAL(valueChanged(int)), this, SLOT(qualityChanged(int)));

    connect(saveEnabled,   SIGNAL(stateChanged(int)), this, SLOT(saveEnabledChanged(int)));
    connect(savePathEdit,  SIGNAL(editingFinished()), this, SLOT(savePathChanged()));
    connect(saveNameEdit,  SIGNAL(editingFinished()), this, SLOT(saveNameChanged()));

    connect(resetButton,   SIGNAL(clicked()),         this, SLOT(resetSettings()));
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
    trayIconMenu->addAction(openSaveDirectoryAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    if(s.value("save-path").toString().isEmpty())
        openSaveDirectoryAction->setDisabled(true);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Window::openSettings() {
    showNormal();
    // if it's already open, it won't raise, so we force it.
    raise();
}

bool Window::isLoggedIn() {
    if (s.value("key", "") != "")
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

QString Window::getSavePath(){
    QString path = s.value("save-path").toString();
    if(path.startsWith('~')){
        path.remove(0, 1); // remove "~"
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + path; // add "/home/user"
    }
    return path;
}

QString Window::getSaveName() {
    QString saveName = "ss-";
    saveName += QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    return getSavePath() + "/" + saveName;
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

// note: this fails with ~user
void Window::openSaveDirectory() {
    bool response = true;
    if(s.contains("save-path")){
        QString path = getSavePath();
        response = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }

    if (!response) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), tr("Error opening save directory"), icon);
    }
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

    QDir::mkpath(getSavePath());
    QFile::copy(fileName, getSaveName());
}

void Window::puushDone(int returnCode, QString output) {
    if (returnCode != 0) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), output, icon);
        return;
    }

    QStringList pieces = output.split(",");
    QString code = pieces[0];
    QString url = "";

    if (pieces.length() > 1)
        url = pieces[1];

    setTrayIcon(":/images/icon.svg.png");

    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();

    // code can only be 0 = success, or these errors.
    if (code == "-1") {
        trayIcon->showMessage(tr("Error!"), tr("Uploading failed. Have you authenticated?"), icon);
        return;
    } else if (code == "-2") {
        trayIcon->showMessage(tr("Error!"), tr("Uploading failed. This might be a bug with puush-qt."), icon);
        return;
    } else if(code == "-3"){
        trayIcon->showMessage(tr("Error!"), tr("Uploading failed due invalid md5."), icon);
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    lastUrl = QUrl(url);
    clipboard->setText(url);

    trayIcon->showMessage(tr("Success!"), url + tr("\nThe url was copied to your clipboard!"), icon);
}

void Window::qualityChanged(int val){
    s.setValue("quality", val);
}

void Window::saveEnabledChanged(int val){
    // 0 = unchecked, 1 = unchecked & disabled?, 2 = checked, 3 = checked & disabled?
    s.setValue("save-enabled", val == 2);
    savePathEdit->setEnabled(val == 2);
    // saveNameEdit->setEnabled(val == 2); // disabled because NYI
}

void Window::savePathChanged(){
    s.setValue("save-path", savePathEdit->text());
}

void Window::saveNameChanged(){
    // s.setValue("save-name", saveNameEdit->text()); //disabled because NYI
}

void Window::resetSettings(){
    s.setValue("quality", 90);
    s.setValue("save-enabled", true);
    s.setValue("save-path", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    s.setValue("save-name", "ss-yyyy-MM-dd_hh-mm-ss");

    qualitySlider->setValue(s.value("quality").toInt());
    saveEnabled->setChecked(s.value("save-enabled").toBool() ? Qt::Checked : Qt::Unchecked);
    savePathEdit->setText(s.value("save-path").toString());
    saveNameEdit->setText(s.value("save-name").toString());
}
