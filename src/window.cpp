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

#include <iostream>

#include "api/apirequest.h"
#include "api/apiauth.h"
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

/**
 * Set the tray icon
 * @brief Window::setTrayIcon
 * @param image
 */
void Window::setTrayIcon(QString image) {
    QIcon icon = QIcon(image), tr("Icon");
    trayIcon->setIcon(icon);
}

/**
 * Set the window icon.
 * @brief Window::setAppIcon
 * @param image
 */
void Window::setAppIcon(QString image) {
    QIcon icon = QIcon(image), tr("Icon");
    setWindowIcon(icon);
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    // left click
    case QSystemTrayIcon::Trigger:
        trayIconMenu->popup(QCursor::pos());
        break;
    default:
        break;
    }
}

/**
 * Event when clicking the log in button.
 * @brief Window::submitInfo
 */
void Window::submitInfo() {
    std::cout << "Window::submitInfo()" << std::endl;
    QString email    = emailEdit->text();
    QString password = passwordEdit->text();

    ApiAuth *api = new ApiAuth(email, password);
    QMetaObject::Connection r = connect(api, SIGNAL(done(ApiAuth *)), this, SLOT(authDone(ApiAuth *)));
    std::cout << "submitInfo connect == " << r << std::endl;
    api->start();
}

/**
 * Event when clicking the log out button.
 * @brief Window::submitInfo
 */
void Window::logout(){
    s.remove("key");
    authMessage->setText(tr("Logged out"));
}

/**
 * Event when authentication through Puush has finished.
 * @brief Window::submitInfo
 */
void Window::authDone(ApiAuth *req) {
    std::cout << "Window::authDone()" << std::endl;

    if(req->failed()){
        authMessage->setText(req->errorStr());
        return;
    }

    s.setValue("key", req->apikey());
    authMessage->setText(tr("Authentication Successfull"));

    userData = req->allData();

    delete req;
}

/**
 * Event used when clicking the message window
 * @brief Window::messageClicked
 */
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
    if (!s.contains("key"))
        authMessage->setText("Not logged in");
    else
        authMessage->setText("Logged in");

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
    selectSavePathButton = new QPushButton(tr("..."));
    saveNameEdit = new QLineEdit(s.value("save-name").toString());

    saveEnabled->setCheckState(s.value("save-enabled").toBool() ? Qt::Checked : Qt::Unchecked);
    savePathEdit->setEnabled(s.value("save-enabled").toBool());
    selectSavePathButton->setEnabled(s.value("save-enabled").toBool());
    // saveNameEdit->setEnabled(s.value("save-enabled").toBool()); // disabled because NYI
    saveNameEdit->setEnabled(false);

    QFormLayout *saveLayout = new QFormLayout();
    saveLayout->addRow(saveEnabled);
    QHBoxLayout *locLayout = new QHBoxLayout();
    locLayout->addWidget(savePathEdit);
    locLayout->addWidget(selectSavePathButton);
    saveLayout->addRow(tr("Location:"),  locLayout);
    //saveLayout->addRow(tr("File Name:"), saveNameEdit);

    authGroupBox-> setLayout(authLayout);
    puushGroupBox->setLayout(puushLayout);
    saveGroupBox-> setLayout(saveLayout);
}

void Window::createActions() {
    uploadAction = new QAction(tr("&Upload File..."), this);
    connect(uploadAction, SIGNAL(triggered()), this, SLOT(uploadFile()));

    fullScreenAction = new QAction(tr("Capture &Desktop"), this);
    connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreenScreenshot()));

    selectAreaAction = new QAction(tr("Capture &Area"), this);
    connect(selectAreaAction, SIGNAL(triggered()), this, SLOT(selectAreaScreenshot()));

    activeAction = new QAction(tr("Capture &Current Window"), this);
    connect(activeAction, SIGNAL(triggered()), this, SLOT(activeWindowScreenshot()));

    settingsAction = new QAction(tr("&Settings..."), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    openSaveDirectoryAction = new QAction(tr("&Open Screenshot Directory"), this);
    connect(openSaveDirectoryAction, SIGNAL(triggered()), this, SLOT(openSaveDirectory()));
}

void Window::createSettingsSlots(){
    connect(emailEdit, SIGNAL(editingFinished()), this, SLOT(emailChanged()));

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitInfo()));
    connect(logoutButton, SIGNAL(clicked()), this, SLOT(logout()));

    connect(qualitySlider, SIGNAL(valueChanged(int)), this, SLOT(qualityChanged(int)));

    connect(saveEnabled,          SIGNAL(stateChanged(int)), this, SLOT(saveEnabledChanged(int)));
    connect(savePathEdit,         SIGNAL(editingFinished()), this, SLOT(savePathChanged()));
    connect(selectSavePathButton, SIGNAL(clicked()),         this, SLOT(openSavePath()));
    connect(saveNameEdit,         SIGNAL(editingFinished()), this, SLOT(saveNameChanged()));

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetSettings()));
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

    openSaveDirectoryAction->setEnabled(s.value("save-enabled").toBool());

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

/**
 * Open the settings window.
 * @brief Window::openSettings
 */
void Window::openSettings() {
    showNormal();
    // if it's already open, it won't raise, so we force it.
    raise();
}

/**
 * Checks if the user is logged in, and if not, display the log in window.
 * @brief Window::isLoggedIn
 * @return
 */
bool Window::isLoggedIn() {
    if (s.contains("key"))
        return true;

    openSettings();

    return false;
}

/**
 * Puush upload a file via the open file dialog.
 * @brief Window::uploadFile
 */
void Window::uploadFile() {
    if (!isLoggedIn()) return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Upload file"));

    if (fileName == "") return;

    Upload *u = new Upload(fileName);

    connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
    connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));
}

/**
 * Generates the temporary screenshot file names.
 * @brief Window::getFileName
 * @return
 */
QString Window::getFileName() {
    return "/tmp/ss-" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
}

/**
 * Opens a folder dialog to set the screenshot save path.
 * @brief Window::openSavePath
 */
void Window::openSavePath(){
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Set Screenshot Directory"),
                QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir != "") savePathEdit->setText(dir);

    setSavePath(dir);
}

/**
 * Parses the screenshot save path.
 * @brief Window::getSavePath
 * @return
 */
QString Window::getSavePath(){
    QString path = s.value("save-path").toString();
    if(path.startsWith("~/")){
        path.remove(0, 1); // remove "~"
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + path; // add "/home/user"
    }
    return path;
}

/**
 * Generates the screenshot name based on the current settings.
 * @brief Window::getSaveName
 * @return
 */
QString Window::getSaveName() {
    QString saveName = "ss-";
    saveName += QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    return getSavePath() + "/" + saveName;
}

/**
 * Puush capture desktop screenshot.
 * @brief Window::fullScreenScreenshot
 */
void Window::fullScreenScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getFileName();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->fullScreen();
}

/**
 * Puush capture area screenshot.
 * @brief Window::selectAreaScreenshot
 */
void Window::selectAreaScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getFileName();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->selectArea();
}

/**
 * Initiate the Puush current window screenshot.
 * @brief Window::activeWindowScreenshot
 */
void Window::activeWindowScreenshot() {
    if (!isLoggedIn()) return;

    numTime = 5;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateActiveMessage()));
    timer->start(1000);
}

/**
 * Shows the countdown message leading to the current window screenshot.
 * Once it is done, it will create the Puush current window screenshot.
 * @brief Window::updateActiveMessage
 */
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
/**
 * Opens the screenshot directory if the user has enabled saving local screenshots.
 * @brief Window::openSaveDirectory
 */
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

/**
 * Notify the user when a Puush upload has started.
 * @brief Window::puushStarted
 */
void Window::puushStarted() {
    setTrayIcon(":/images/icon-uploading.svg.png");
}

/**
 * Initiate the upload process when the screenshot has been taken.
 * @brief Window::screenshotDone
 * @param returnCode
 * @param fileName
 * @param output
 */
void Window::screenshotDone(int returnCode, QString fileName, QString output) {
    if (returnCode != 0) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), output, icon);
        return;
    }

    Upload *u = new Upload(fileName);

    connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
    connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));

    if(s.value("save-enabled").toBool()){
        QDir root = QDir::root();
        root.mkpath(getSavePath());
        QFile::copy(fileName, getSaveName());
    }
}

/**
 * Notify the user when the upload to Puush has been completed.
 * @brief Window::puushDone
 * @param returnCode
 * @param output
 */
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

/**
 * Set the quality of the screenshots
 * @brief Window::qualityChanged
 * @param val
 */
void Window::qualityChanged(int val){
    s.setValue("quality", val);
}

/**
 * Event checking when the save enabled button was clicked.
 * @brief Window::saveEnabledChanged
 * @param val
 */
void Window::saveEnabledChanged(int val){
    // 0 = unchecked, 1 = unchecked & disabled?, 2 = checked, 3 = checked & disabled?
    s.setValue("save-enabled", val == 2);
    savePathEdit->setEnabled(val == 2);
    selectSavePathButton->setEnabled(val == 2);
    openSaveDirectoryAction->setEnabled(val == 2);
    // saveNameEdit->setEnabled(val == 2); // disabled because NYI
}

/**
 * Event for the path text box changing.
 * @brief Window::savePathChanged
 */
void Window::savePathChanged(){
    setSavePath(savePathEdit->text());
}

/**
 * Save the save screenshot directory path to the settings.
 * @brief Window::setSavePath
 * @param path
 */
void Window::setSavePath(QString path){
    s.setValue("save-path", path);
}

void Window::saveNameChanged(){
    // s.setValue("save-name", saveNameEdit->text()); //disabled because NYI
}

void Window::resetSettings(){
    s.setValue("quality", 90);
    s.setValue("save-enabled", false);
    s.setValue("save-path", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    s.setValue("save-name", "ss-yyyy-MM-dd_hh-mm-ss");

    qualitySlider->setValue(s.value("quality").toInt());
    saveEnabled->setChecked(s.value("save-enabled").toBool() ? Qt::Checked : Qt::Unchecked);
    savePathEdit->setText(s.value("save-path").toString());
    saveNameEdit->setText(s.value("save-name").toString());
}

void Window::emailChanged(){
    s.setValue("email", emailEdit->text());
}
