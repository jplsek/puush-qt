#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDebug>

#include "api/apirequest.h"
#include "api/apiauth.h"
#include "api/apihist.h"

#include "systray.h"
#include "screenshot.h"
#include "upload.h"

Systray::Systray(QObject *parent) : QObject(parent) {
    s.setEmptyToDefaults();

    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    setTrayIcon(":/images/puush-qt.png");

    trayIcon->show();
}

/**
 * Set the tray icon
 * @brief Systray::setTrayIcon
 * @param image
 */
void Systray::setTrayIcon(QString image) {
    QIcon icon = QIcon(image), tr("Icon");
    trayIcon->setIcon(icon);
}

void Systray::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    //qDebug() << "icon activated";
    switch (reason) {
    // left click
    case QSystemTrayIcon::Trigger:
        trayIconMenu->popup(QCursor::pos());
        break;
    // double left click
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "dblclickA";
        doDoubleClickAction();
        break;
    default:
        break;
    }
}

void Systray::doDoubleClickAction() {
    qDebug() << "dblclick";
    if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS)) {
        openSaveDirectory();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS)) {
        openSettings();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_FILE)) {
        uploadFile();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_DESKTOP)) {
        fullScreenScreenshot();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_AREA)) {
        selectAreaScreenshot();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_WINDOW)) {
        activeWindowScreenshotTimed();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_ACCOUNT)) {
        openAccount();
    } else {
        qDebug() << "Option not recognized";
    }
}

/**
 * Event used when clicking the message window
 * @brief Systray::messageClicked
 */
void Systray::messageClicked() {
    if (lastUrl == QUrl(""))
        return;

    openUrl(lastUrl);

    // reset it so if an error popup comes up later, the url wont open
    lastUrl = QUrl("");
}

void Systray::openUrl(QUrl url) {
    bool response = QDesktopServices::openUrl(url);

    if (!response) {
        trayIcon->showMessage(tr("Error!"), tr("There was an issue opening the URL. Is your default browser set?"), QSystemTrayIcon::Warning);
    }
}

void Systray::createActions() {
    myAccountAction = new QAction(tr("&My Account"), this);
    connect(myAccountAction, SIGNAL(triggered()), this, SLOT(openAccount()));

    uploadFileAction = new QAction(tr("Upload &File..."), this);
    connect(uploadFileAction, SIGNAL(triggered()), this, SLOT(uploadFile()));

    uploadClipboardAction = new QAction(tr("Upload &Clipboard"), this);
    connect(uploadClipboardAction, SIGNAL(triggered()), this, SLOT(uploadClipboard()));

    fullScreenAction = new QAction(tr("Capture &Desktop"), this);
    connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreenScreenshot()));

    selectAreaAction = new QAction(tr("Capture &Area"), this);
    connect(selectAreaAction, SIGNAL(triggered()), this, SLOT(selectAreaScreenshot()));

    activeAction = new QAction(tr("Capture Current &Window"), this);
    connect(activeAction, SIGNAL(triggered()), this, SLOT(activeWindowScreenshotTimed()));

    settingsAction = new QAction(tr("&Settings..."), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    openSaveDirectoryAction = new QAction(tr("&Open Screenshot Directory"), this);
    connect(openSaveDirectoryAction, SIGNAL(triggered()), this, SLOT(openSaveDirectory()));
}

void Systray::createTrayIcon() {
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(myAccountAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(uploadFileAction);
    trayIconMenu->addAction(uploadClipboardAction);
    trayIconMenu->addAction(fullScreenAction);
    trayIconMenu->addAction(selectAreaAction);
    trayIconMenu->addAction(activeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(openSaveDirectoryAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    openSaveDirectoryAction->setEnabled(s.value(Settings::LOCAL_SAVE_ENABLED).toBool());

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

/**
 * Show the account login tab
 * @brief Systray::openSettings
 */
void Systray::openSettings() {
    emit signalOpenSettings();
}

/**
 * Checks if the user is logged in, and if not, display the log in window.
 * @brief Systray::isLoggedIn
 * @return
 */
bool Systray::isLoggedIn() {
    if (s.value(Settings::ACCOUNT_API_KEY) != "")
        return true;

    trayIcon->showMessage("puush-qt", tr("You are not logged into Puush..."), QSystemTrayIcon::Warning);
    openSettings();

    return false;
}

void Systray::openAccount() {
    if (!isLoggedIn()) return;

    QString key = s.value(Settings::ACCOUNT_API_KEY).toString();
    openUrl(QUrl(s.value(Settings::BASE_URL).toString() + "login/go/?k=" + key));
}

/**
 * Puush upload a file via the open file dialog.
 * @brief Systray::uploadFile
 */
void Systray::uploadFile() {
    if (!isLoggedIn()) return;

    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Upload file"));

    if (fileName == "") return;

    Upload *u = new Upload(fileName);

    connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
    connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));
}

/**
 * Puush upload the clipboard
 * @brief Systray::uploadClipboard
 */
void Systray::uploadClipboard() {
    if (!isLoggedIn()) return;

    QString text = QApplication::clipboard()->text();

    // just look for "file://" and upload it, else just upload the text itself as a text file
    if (text.isEmpty()) {
        return;
    } else if (text.contains("file://")) {
        Upload *u = new Upload(text);

        connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
        connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));
    } else {
        QTemporaryFile file;
        // The file is deleted too soon before it can be uploaded since the upload is in a callback.
        // Since it's in a temporary directory it'll get deleted eventually anyways...
        file.setAutoRemove(false);

        if (file.open()) {
            file.write(text.toLocal8Bit().data());

            Upload *u = new Upload(file.fileName());

            connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
            connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));
        } else {
            trayIcon->showMessage("puush-qt", tr("Error opening temporary file for writing!"), QSystemTrayIcon::Warning);
        }
    }
}

/**
 * Puush capture desktop screenshot.
 * @brief Systray::fullScreenScreenshot
 */
void Systray::fullScreenScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getTempPath();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->fullScreen();
}

/**
 * Puush capture area screenshot.
 * @brief Systray::selectAreaScreenshot
 */
void Systray::selectAreaScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getTempPath();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->selectArea();
}

void Systray::activeWindowScreenshotTimed() {
    if (!isLoggedIn()) return;

    trayIcon->showMessage(tr("Select a window"),
                          tr("Taking a screenshot in ") + QString::number(defaultSelectionTimeout) + tr(" seconds..."),
                          QSystemTrayIcon::Information);

    numTime = defaultSelectionTimeout;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateActiveMessage()));
    timer->start(1000);
}

/**
 * Initiate the Puush current window screenshot.
 * @brief Systray::activeSystrayScreenshot
 */
void Systray::activeWindowScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getTempPath();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->activeWindow();
}

/**
 * Toggle puush functionality
 * @brief Systray::togglePuush
 */
void Systray::togglePuush() {
    trayIcon->showMessage("puush-qt", tr("Not implemented"), QSystemTrayIcon::Information);
}

/**
 * Shows the countdown message leading to the current window screenshot.
 * Once it is done, it will create the Puush current window screenshot.
 * @brief Systray::updateActiveMessage
 */
void Systray::updateActiveMessage() {
    if (numTime < 1) {
        timer->stop();
        activeWindowScreenshot();
        return;
    }

    --numTime;
}

/**
 * Notify the user when a Puush upload has started.
 * @brief Systray::puushStarted
 */
void Systray::puushStarted() {
    setTrayIcon(":/images/puush-qt-uploading.png");
}

/**
 * Initiate the upload process when the screenshot has been taken.
 * @brief Systray::screenshotDone
 * @param returnCode
 * @param fileName
 * @param output
 */
void Systray::screenshotDone(int returnCode, QString fileName, QString output) {
    if (returnCode != 0) {
        trayIcon->showMessage(tr("Error!"), output, QSystemTrayIcon::Warning);
        return;
    }

    Upload *u = new Upload(fileName);

    connect(u, SIGNAL(started()), this, SLOT(puushStarted()));
    connect(u, SIGNAL(finished(int, QString)), this, SLOT(puushDone(int, QString)));

    if(s.value(Settings::LOCAL_SAVE_ENABLED).toBool()){
        QDir root = QDir::root();
        root.mkpath(getSaveDirectory());
        QFile::copy(fileName, getSavePath());
    }
}

/**
 * Generates the temporary screenshot file names.
 * @brief Systray::getTempPath
 * @return
 */
QString Systray::getTempPath() {
    return QDir::tempPath() + QDir::separator() + getFileName();
}

/**
 * Parses the screenshot save path.
 * @brief Systray::getSaveDirectory
 * @return
 */
QString Systray::getSaveDirectory(){
    QString path = s.value(Settings::LOCAL_SAVE_PATH).toString();

    if(path.startsWith("~/")){
        path.remove(0, 1); // remove "~"
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + path; // add "/home/user"
    }

    return path;
}

/**
 * Gets the file name to use for screenshots
 * @brief Systray::getFileName
 * @return
 */
QString Systray::getFileName() {
    return QDateTime::currentDateTime().toString(s.value(Settings::LOCAL_SAVE_NAME).toString()) + ".png";
}

/**
 * Generates the screenshot name based on the current settings.
 * @brief Systray::getSavePath
 * @return
 */
QString Systray::getSavePath() {
    return getSaveDirectory() + "/" + getFileName();
}

/**
 * Opens the screenshot directory if the user has enabled saving local screenshots.
 * @brief Systray::openSaveDirectory
 */
void Systray::openSaveDirectory() {
    QString path = getSaveDirectory();

    bool response = QDesktopServices::openUrl(QUrl::fromLocalFile(path));

    if (!response) {
        trayIcon->showMessage(tr("Error!"), tr("Error opening save directory"), QSystemTrayIcon::Warning);
    }
}

/**
 * Notify the user when the upload to Puush has been completed.
 * @brief Systray::puushDone
 * @param returnCode
 * @param output
 */
void Systray::puushDone(int returnCode, QString output) {
    setTrayIcon(":/images/puush-qt.png");

    if (returnCode != 0) {
        trayIcon->showMessage(tr("Error!"), output, QSystemTrayIcon::Warning);
        qDebug() << "puushDone error: " << returnCode;
        return;
    }

    QStringList pieces = output.split(",");
    QString code = pieces[0];
    QString url = "";

    if (pieces.length() > 1)
        url = pieces[1];

    QSystemTrayIcon::MessageIcon warningIcon = QSystemTrayIcon::Warning;
    QSystemTrayIcon::MessageIcon infoIcon = QSystemTrayIcon::Information;

    // code can only be 0 = success, or these errors.
    if (code == "-1") {
        trayIcon->showMessage(tr("Error!"), tr("Uploading failed. Have you authenticated?"), warningIcon);
        return;
    } else if (code == "-2") {
        trayIcon->showMessage(tr("Error!"), tr("Uploading failed. This might be a bug with puush-qt."), warningIcon);
        return;
    } else if(code == "-3"){
        trayIcon->showMessage(tr("Error!"), tr("Uploading failed due invalid md5."), warningIcon);
        return;
    }

    lastUrl = QUrl(url);

    if (s.value(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD).toBool()) {
        QApplication::clipboard()->setText(url);
        trayIcon->showMessage(tr("Success!"), url + tr("\nThe url was copied to your clipboard!"), infoIcon);
    } else {
        trayIcon->showMessage(tr("Success!"), url, infoIcon);
    }

    if (s.value(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER).toBool()) {
        openUrl(url);
    }
}
