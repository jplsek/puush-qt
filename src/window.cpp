#include <QtGui>
#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QProcess>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTabWidget>
#include <QRadioButton>
#include <QStackedLayout>
#include <iostream>
#include <QDebug>

#include "api/apirequest.h"
#include "api/apiauth.h"
#include "api/apihist.h"

#include "window.h"
#include "screenshot.h"
#include "upload.h"

Window::Window() {
    s.setEmptyToDefaults();

    tabs = createTabs();

    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    setTrayIcon(":/images/puush-qt.png");
    setAppIcon(":/images/puush-qt.png");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);

    trayIcon->show();

    setWindowTitle(tr("puush-qt"));

    // resize window based on tab layout size
    resize(tabs->width(), height());

    connectSignals();
}

QTabWidget *Window::createTabs(){
    QTabWidget *tabs = new QTabWidget();
    tabs->addTab(createTabGeneral(),     "General");
    tabs->addTab(createTabKeyBindings(), "Key Bindings"); // this should be handled by a library that interfaces with the desktop environment
    tabs->addTab(createTabAccount(),     "Account");
    tabs->addTab(createTabAdvanced(),    "Advanced");
    tabs->addTab(createTabHistory(),     "History");
    tabs->addTab(createTabAbout(),       "About");
    return tabs;
}

QWidget *Window::createTabGeneral(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *puushGroup = new QGroupBox("On Successful Puush");
    QVBoxLayout *puushLayout = new QVBoxLayout();

    enablePuushSound = new QCheckBox("Play a sound");
    enablePuushSound->setChecked(s.value(Settings::ON_PUUSH_SOUND).toBool());
    puushLayout->addWidget(enablePuushSound);

    enableLocalSave = new QCheckBox("Enable local save");
    enableLocalSave->setChecked(s.value(Settings::LOCAL_SAVE_ENABLED).toBool());
    puushLayout->addWidget(enableLocalSave);

    QWidget *saveLocation = new QWidget();
    QHBoxLayout *saveLocationLayout = new QHBoxLayout();
    localSaveLocation = new QLineEdit(s.value(Settings::LOCAL_SAVE_PATH).toString());
    selectSavePathButton = new QPushButton(tr("..."));
    saveLocationLayout->addWidget(localSaveLocation);
    saveLocationLayout->addWidget(selectSavePathButton);
    saveLocation->setLayout(saveLocationLayout);
    puushLayout->addWidget(saveLocation);

    enableLinkToClipboard = new QCheckBox("Copy link to clipboard");
    enableLinkToClipboard->setChecked(s.value(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD).toBool());
    puushLayout->addWidget(enableLinkToClipboard);

    enableLinkToBrowser = new QCheckBox("Open link in browser");
    enableLinkToBrowser->setChecked(s.value(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER).toBool());
    puushLayout->addWidget(enableLinkToBrowser);

    puushGroup->setLayout(puushLayout);
    qhb->addWidget(puushGroup);

    QGroupBox *trayGroup = new QGroupBox("Tray Icon Behavior on Double Click");
    QVBoxLayout *trayLayout = new QVBoxLayout();
    trayDoubleClickSettings = new QRadioButton("Show settings window");
    trayDoubleClickSettings->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS));
    trayDoubleClickCapture  = new QRadioButton("Begin screen capture mode");
    trayDoubleClickCapture->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_CAPTURE));
    trayDoubleClickUpload   = new QRadioButton("Open upload file window");
    trayDoubleClickUpload->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS));
    trayLayout->addWidget(trayDoubleClickSettings);
    trayLayout->addWidget(trayDoubleClickCapture);
    trayLayout->addWidget(trayDoubleClickUpload);
    trayGroup->setLayout(trayLayout);
    qhb->addWidget(trayGroup);

    qhb->addStretch();

    resetGeneralButton = new QPushButton("Reset General Settings");
    qhb->addWidget(resetGeneralButton);

    w->setLayout(qhb);
    return w;
}

QWidget *Window::createTabKeyBindings(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *keyBindings = new QGroupBox("Key Bindings");
    QVBoxLayout *keyBindingsLayout = new QVBoxLayout();
    keyBindings->setLayout(keyBindingsLayout);

    QLabel *tmp = new QLabel(tr("Not implemented..."));

    keyBindingsLayout->addWidget(tmp);

    qhb->addWidget(keyBindings);

    qhb->addStretch();
    w->setLayout(qhb);
    return w;
}

QWidget *Window::createTabAccount(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *account = new QGroupBox("Account Details");
    accountBox = new QStackedLayout();
    account->setLayout(accountBox);
    qhb->addWidget(account);

    myAccount = new QPushButton(tr("My Account"));
    emailEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    submitButton = new QPushButton(tr("Submit"));
    logoutButton = new QPushButton(tr("Logout"));

    createLoginBox();
    createLoggedinBox();

    if (s.value(Settings::ACCOUNT_API_KEY).toString() == "") {
        selectLoginBox();
    } else {
        selectLoggedinBox();
    }

    // only used for errors
    authMessage = new QLabel();
    authMessage->setStyleSheet("QLabel { color: red; }");
    qhb->addWidget(authMessage);

    qhb->addStretch();
    w->setLayout(qhb);
    return w;
}

void Window::createLoggedinBox() {
    QWidget *loggedinBox = new QWidget();
    QGridLayout *qgl = new QGridLayout;

    // I force align top because of a top/bottom alignment bug while using QStackedLayout
    // The bug causes the rows to be too high

    QLabel *email = new QLabel(tr("Logged in as: "));
    email->setAlignment(Qt::AlignRight);

    emailLabel = new QLabel();
    emailLabel->setAlignment(Qt::AlignTop);

    QLabel *apiKey = new QLabel(tr("API Key: "));
    apiKey->setAlignment(Qt::AlignRight);

    apiKeyLabel = new QLabel();
    apiKeyLabel->setAlignment(Qt::AlignTop);

    QLabel *type = new QLabel(tr("Account Type: "));
    type->setAlignment(Qt::AlignRight);

    accountTypeLabel = new QLabel();
    accountTypeLabel->setAlignment(Qt::AlignTop);

    QLabel *expiry = new QLabel(tr("Expiry Date: "));
    expiry->setAlignment(Qt::AlignRight);

    expiryLabel = new QLabel();
    expiryLabel->setAlignment(Qt::AlignTop);

    QLabel *disk = new QLabel(tr("Disk Usage: "));
    disk->setAlignment(Qt::AlignRight);

    diskLabel = new QLabel();
    diskLabel->setAlignment(Qt::AlignTop);

    qgl->addWidget(email, 0,0);
    qgl->addWidget(emailLabel, 0, 1);
    qgl->addWidget(apiKey, 1, 0);
    qgl->addWidget(apiKeyLabel, 1, 1);
    qgl->addWidget(type, 2, 0);
    qgl->addWidget(accountTypeLabel, 2, 1);
    qgl->addWidget(expiry, 3, 0);
    qgl->addWidget(expiryLabel, 3, 1);
    qgl->addWidget(disk, 4, 0);
    qgl->addWidget(diskLabel, 4, 1);
    qgl->addWidget(myAccount, 5, 0);
    qgl->addWidget(logoutButton, 5, 1);

    loggedinBox->setLayout(qgl);
    accountBox->addWidget(loggedinBox);
}

void Window::createLoginBox() {
    QWidget *loginBox = new QWidget();
    QVBoxLayout *accountLayout = new QVBoxLayout();

    QLabel *info = new QLabel(tr("You must login to use Puush. If you don't already have an account, you can register for an account below."));
        info->setWordWrap(true);

    passwordEdit->setEchoMode(QLineEdit::Password);

    submitButton->setDefault(true);

    QWidget *form = new QWidget();
    QFormLayout *authLayout = new QFormLayout();
    authLayout->addRow(tr("Email:"),    emailEdit);
    authLayout->addRow(tr("Password:"), passwordEdit);

    form->setLayout(authLayout);

    QLabel *forgot = new QLabel("<a href=\"" + s.value(Settings::BASE_URL).toString() + "reset_password\">" + tr("Forgot Password?") + "</a>");
    forgot->setTextFormat(Qt::RichText);
    forgot->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgot->setOpenExternalLinks(true);

    QLabel *registerAccount = new QLabel("<a href=\"" + s.value(Settings::BASE_URL).toString() + "register\">" + tr("Register...") + "</a>");
    registerAccount->setTextFormat(Qt::RichText);
    registerAccount->setTextInteractionFlags(Qt::TextBrowserInteraction);
    registerAccount->setOpenExternalLinks(true);

    accountLayout->addWidget(info);
    accountLayout->addWidget(form);
    accountLayout->addWidget(forgot);
    accountLayout->addWidget(submitButton);
    accountLayout->addWidget(registerAccount);

    loginBox->setLayout(accountLayout);
    accountBox->addWidget(loginBox);
}

void Window::selectLoginBox() {
    accountBox->setCurrentIndex(0);
}

/**
 * Select the logged in box and update all values
 * @brief Window::selectLoggedinBox
 */
void Window::selectLoggedinBox() {
    accountBox->setCurrentIndex(1);

    // "clear" password from memory after signing in
    passwordEdit->setText("");

    emailLabel->setText(s.value(Settings::ACCOUNT_EMAIL).toString());
    apiKeyLabel->setText(s.value(Settings::ACCOUNT_API_KEY).toString());
    accountTypeLabel->setText("Not Implemented");
    expiryLabel->setText("Not Implemented");
    diskLabel->setText("Not Implemented");
}

QWidget *Window::createTabAdvanced(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *screenBox = new QGroupBox("Screen Capture Quality");
    QVBoxLayout *screenLayout = new QVBoxLayout();
        compressionAlways = new QRadioButton("Always use PNG (no lossy compression)");
        compressionSmart  = new QRadioButton("Smart (use JPG unless PNG is smaller)");
        compressionAlways->setChecked(s.radioValueIs(Settings::COMPRESSION_PHILOSOPHY, Settings::PNG_ALWAYS));
        compressionSmart->setChecked(s.radioValueIs(Settings::COMPRESSION_PHILOSOPHY, Settings::IMAGE_TYPE_SMALLER));
        compressionAlways->setEnabled(false);
        compressionSmart->setEnabled(false);
        QFormLayout *qualityLayout = new QFormLayout();
        QWidget *quality = new QWidget();
        qualitySlider = new QSlider(Qt::Horizontal);
        qualitySlider->setRange(0, 100);
        qualitySlider->setTickPosition(QSlider::TicksBelow);
        qualitySlider->setTickInterval(10);
        qualitySlider->setValue(s.value(Settings::IMAGE_QUALITY).toInt());
        qualityLayout->addRow(tr("Quality:"), qualitySlider);
        quality->setLayout(qualityLayout);
        screenLayout->addWidget(compressionAlways);
        screenLayout->addWidget(compressionSmart);
        screenLayout->addWidget(quality);
        screenBox->setLayout(screenLayout);

    QGroupBox *contextBox = new QGroupBox("Context Menu");
    QVBoxLayout *contextLayout = new QVBoxLayout();
        contextShowExplorerContext = new QCheckBox("Show file explorer context menu items");
        contextShowExplorerContext->setChecked(s.value(Settings::ENABLE_EXPLORER_CONTEXT_MENU).toBool());
        contextShowExplorerContext->setEnabled(false);
        contextLayout->addWidget(contextShowExplorerContext);
        contextBox->setLayout(contextLayout);

    QGroupBox *fullscreenBox = new QGroupBox("Fullscreen Capture");
    QVBoxLayout *fullscreenLayout = new QVBoxLayout();
        fullscreenCaptureAll     = new QRadioButton("Capture all screens");
        fullscreenCaptureCursor  = new QRadioButton("Capture screen containing mouse cursor");
        fullscreenCapturePrimary = new QRadioButton("Always capture primary screen");
        fullscreenCaptureAll->setChecked(s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::ALL_DESKTOPS));
        fullscreenCaptureAll->setChecked(s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::CURRENT_DESKTOP));
        fullscreenCaptureAll->setChecked(s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::PRIMARY_DESKTOP));
        fullscreenLayout->addWidget(fullscreenCaptureAll);
        fullscreenLayout->addWidget(fullscreenCaptureCursor);
        fullscreenLayout->addWidget(fullscreenCapturePrimary);
        fullscreenBox->setLayout(fullscreenLayout);

    QGroupBox *dangerousBox = new QGroupBox("Dangerous stuff");
    QVBoxLayout *dangerousLayout = new QVBoxLayout();
        dangerousExperimentalEnable = new QCheckBox("Enable experimental features");
        dangerousExperimentalEnable->setChecked(s.value(Settings::ENABLE_EXPERIMENTAL_FEATURES).toBool());
        dangerousExperimentalEnable->setEnabled(false);
        dangerousNoSelectionRect = new QCheckBox("Don't draw selection rectangle");
        dangerousNoSelectionRect->setChecked(!s.value(Settings::SHOW_SELECTION_RECTANGLE).toBool());
        dangerousNoSelectionRect->setEnabled(false);
        dangerousLayout->addWidget(dangerousExperimentalEnable);
        dangerousLayout->addWidget(dangerousNoSelectionRect);
        dangerousBox->setLayout(dangerousLayout);

    resetAdvancedButton = new QPushButton("Reset Advanced Settings");

    qhb->addWidget(screenBox);
    qhb->addWidget(contextBox);
    qhb->addWidget(fullscreenBox);
    qhb->addWidget(dangerousBox);
    qhb->addStretch();
    qhb->addWidget(resetAdvancedButton);

    w->setLayout(qhb);
    return w;
}

QWidget *Window::createTabHistory(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *history = new QGroupBox("History");
    QVBoxLayout *historyLayout = new QVBoxLayout();
    history->setLayout(historyLayout);

    QLabel *tmp = new QLabel(tr("Not implemented..."));
    historyLayout->addWidget(tmp);

    getHistoryButton = new QPushButton(tr("get history"));

    qhb->addWidget(history);
    qhb->addWidget(getHistoryButton);

    qhb->addStretch();

    w->setLayout(qhb);
    return w;
}

QWidget *Window::createTabAbout(){
    QWidget *w = new QWidget();
    QGridLayout *qgl = new QGridLayout();

    QLabel *image = new QLabel();
    image->setPixmap(QPixmap(":/images/puush-qt.png"));

    QLabel *title = new QLabel(QCoreApplication::applicationName());
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(36);
    font.setBold(true);
    title->setFont(font);

    QLabel *version = new QLabel();
    version->setText(tr("Version %1").arg(QCoreApplication::applicationVersion()));
    version->setAlignment(Qt::AlignCenter);

    QLabel *settingsVersion = new QLabel();
    settingsVersion->setText(tr("Settings file version: ") + s.value(Settings::SETTINGS_VERSION).toString());
    settingsVersion->setAlignment(Qt::AlignCenter);

    QLabel *license = new QLabel(tr("License: BSD 3-Clause"));
    license->setAlignment(Qt::AlignCenter);

    aboutQt = new QPushButton(tr("About Qt"));

    qgl->addWidget(image, 0, 0);
    qgl->addWidget(title, 0, 1);
    qgl->addWidget(version, 1, 1);
    qgl->addWidget(settingsVersion, 2, 1);
    qgl->addWidget(license, 3, 1);
    // qgl->addWidget(<spacer>, 4, 1);
    qgl->addWidget(aboutQt, 5, 1);

    qgl->setColumnStretch(1, 1); // make the column with info take up as much space as possible
    qgl->setRowStretch(4, 1); // add space between the version info and the "about qt" button

    w->setLayout(qgl);
    return w;
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

void Window::doDoubleClickAction() {
    qDebug() << "dblclick";
    if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS)) {
        uploadFile();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS)) {
        openSettings();
    } else if (s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_CAPTURE)) {

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
    s.setValue(Settings::ACCOUNT_API_KEY, "");
    selectLoginBox();
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

    authMessage->setText("");

    s.setValue(Settings::ACCOUNT_API_KEY, req->apikey());

    userData = req->allData();

    delete req;

    selectLoggedinBox();
}

/**
 * Event used when clicking the message window
 * @brief Window::messageClicked
 */
void Window::messageClicked() {
    openUrl(lastUrl);
}

void Window::openUrl(QUrl url) {
    bool response = QDesktopServices::openUrl(url);

    if (!response) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
        trayIcon->showMessage(tr("Error!"), tr("There was an issue opening the URL. Is your default browser set?"), icon);
    }
}

void Window::createActions() {
    myAccountAction = new QAction(tr("&My Account"), this);
    connect(myAccountAction, SIGNAL(triggered()), this, SLOT(openAccount()));

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

void Window::connectSignals(){
    // General
    connect(enablePuushSound, SIGNAL(clicked(bool)), this, SLOT(soundEnabledChanged(bool)));
    connect(enableLocalSave, SIGNAL(clicked(bool)), this, SLOT(enableLocalSaveChanged(bool)));

    connect(localSaveLocation, SIGNAL(editingFinished()), this, SLOT(localSaveLocChanged()));
    connect(selectSavePathButton, SIGNAL(clicked()), this, SLOT(openSavePath()));

    connect(enableLinkToClipboard, SIGNAL(clicked(bool)), this, SLOT(enableLinkToClipboardChanged(bool)));
    connect(enableLinkToBrowser, SIGNAL(clicked(bool)), this, SLOT(enableLinkToBrowserChanged(bool)));

    connect(trayDoubleClickSettings, SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedSettingsChanged(bool)));
    connect(trayDoubleClickCapture,  SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedCaptureChanged(bool)));
    connect(trayDoubleClickUpload,   SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedUploadChanged(bool)));

    connect(resetGeneralButton, SIGNAL(clicked()), this, SLOT(resetGeneralSettings()));

    // Key Bindings

    // Account
    connect(emailEdit, SIGNAL(editingFinished()), this, SLOT(emailChanged()));

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitInfo()));
    connect(logoutButton, SIGNAL(clicked()), this, SLOT(logout()));

    connect(myAccount, SIGNAL(clicked(bool)), this, SLOT(openAccount()));

    // Advanced
    connect(compressionAlways, SIGNAL(clicked(bool)), this, SLOT(compressionAlwaysChanged(bool)));
    connect(compressionSmart,  SIGNAL(clicked(bool)), this, SLOT(compressionSmartChanged(bool)));

    connect(qualitySlider, SIGNAL(valueChanged(int)), this, SLOT(qualityChanged(int)));

    connect(contextShowExplorerContext, SIGNAL(clicked(bool)), this, SLOT(contextShowExplorerChanged(bool)));

    connect(fullscreenCaptureAll,     SIGNAL(clicked(bool)), this, SLOT(fullscreenCaptureAllChanged(bool)));
    connect(fullscreenCaptureCursor,  SIGNAL(clicked(bool)), this, SLOT(fullscreenCaptureCursorChanged(bool)));
    connect(fullscreenCapturePrimary, SIGNAL(clicked(bool)), this, SLOT(fullscreenCapturePrimaryChanged(bool)));

    connect(dangerousExperimentalEnable, SIGNAL(clicked(bool)), this, SLOT(dangerousExperimentalEnableChanged(bool)));
    connect(dangerousNoSelectionRect, SIGNAL(clicked(bool)), this, SLOT(dangerousNoSelectionRectChanged(bool)));

    connect(resetAdvancedButton, SIGNAL(clicked()), this, SLOT(resetAdvancedSettings()));

    // History
    connect(getHistoryButton, SIGNAL(clicked(bool)), this, SLOT(getHistory()));

    // About
    connect(aboutQt, SIGNAL(clicked(bool)), qApp, SLOT(aboutQt()));
}

void Window::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(myAccountAction);
    trayIconMenu->addSeparator();
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

    openSaveDirectoryAction->setEnabled(s.value(Settings::LOCAL_SAVE_ENABLED).toBool());

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

/**
 * Show the account login tab
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
    if (s.value(Settings::ACCOUNT_API_KEY) != "")
        return true;

    tabs->setCurrentIndex(2);
    openSettings();

    return false;
}

void Window::openAccount() {
    if (!isLoggedIn()) return;

    QString key = s.value(Settings::ACCOUNT_API_KEY).toString();
    openUrl(QUrl(s.value(Settings::BASE_URL).toString() + "login/go/?k=" + key));
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
    return "/tmp/" + QDateTime::currentDateTime().toString(s.value(Settings::LOCAL_SAVE_NAME).toString()) + ".png";
}

/**
 * Opens a folder dialog to set the screenshot save path.
 * @brief Window::openSavePath
 */
void Window::openSavePath(){
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Set Screenshot Directory"),
                s.value(Settings::LOCAL_SAVE_PATH).toString(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // "" if canceled
    if (dir != "") {
        localSaveLocation->setText(dir);
        setSavePath(dir);
    }
}

/**
 * Parses the screenshot save path.
 * @brief Window::getSavePath
 * @return
 */
QString Window::getSavePath(){
    QString path = s.value(Settings::LOCAL_SAVE_PATH).toString();

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

/**
 * Opens the screenshot directory if the user has enabled saving local screenshots.
 * @brief Window::openSaveDirectory
 */
void Window::openSaveDirectory() {
    QString path = getSavePath();

    bool response = QDesktopServices::openUrl(QUrl::fromLocalFile(path));

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
    setTrayIcon(":/images/puush-qt-uploading.png");
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

    if(s.value(Settings::LOCAL_SAVE_ENABLED).toBool()){
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

    setTrayIcon(":/images/puush-qt.png");

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

    if (s.value(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD).toBool()) {
        QClipboard *clipboard = QApplication::clipboard();
        lastUrl = QUrl(url);
        clipboard->setText(url);

        trayIcon->showMessage(tr("Success!"), url, icon);
    } else {
        trayIcon->showMessage(tr("Success!"), url + tr("\nThe url was copied to your clipboard!"), icon);
    }
}

/**
 * Set the quality of the screenshots
 * @brief Window::qualityChanged
 * @param val
 */
void Window::qualityChanged(int val){
    s.setValue(Settings::IMAGE_QUALITY, val);
}

/**
 * Event checking when the save enabled button was clicked.
 * @brief Window::saveEnabledChanged
 * @param val
 */
void Window::saveEnabledChanged(int val){
    // 0 = unchecked, 1 = unchecked & disabled?, 2 = checked, 3 = checked & disabled?
    s.setValue(Settings::LOCAL_SAVE_ENABLED, val == 2);
    localSaveLocation->setEnabled(val == 2);
    selectSavePathButton->setEnabled(val == 2);
    openSaveDirectoryAction->setEnabled(val == 2);
    // saveNameEdit->setEnabled(val == 2); // disabled because NYI
}

/**
 * Save the save screenshot directory path to the settings.
 * @brief Window::setSavePath
 * @param path
 */
void Window::setSavePath(QString path){
    if(path.startsWith("~/")){
        path.remove(0, 1); // remove "~"
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + path; // add "/home/user"
        // update input with the new path
        localSaveLocation->setText(path);
    }

    s.setValue(Settings::LOCAL_SAVE_PATH, path);
}

void Window::saveNameChanged(){
    // s.setValue(Settings::LOCAL_SAVE_NAME, saveNameEdit->text()); //disabled because NYI
}

void Window::resetGeneralSettings(){
    s.resetGeneralSettings();
    // FIXME: update the state of all the buttons!!!
}

void Window::resetAdvancedSettings(){
    s.resetAdvancedSettings();
    // FIXME: update the state of all the buttons!!!
}

void Window::getHistory(){
    if(s.value(Settings::ACCOUNT_API_KEY).toString() == "")
        return;

    ApiHist *api = new ApiHist(s.value(Settings::ACCOUNT_API_KEY).toString());
    QMetaObject::Connection r = connect(api, SIGNAL(done(ApiHist *)), this, SLOT(getHistoryDone(ApiAuth *)));
    api->start();
}

void Window::getHistoryDone(ApiHist *results){
    results->allData();
    // add rows to a thing with each result. Note that getting a thumbnail probably requires another api request per image
}

void Window::emailChanged(){
    s.setValue(Settings::ACCOUNT_EMAIL, emailEdit->text());
}

void Window::soundEnabledChanged(bool enabled){
    s.setValue(Settings::ON_PUUSH_SOUND, enabled);
}

void Window::enableLocalSaveChanged(bool enabled){
    s.setValue(Settings::LOCAL_SAVE_ENABLED, enabled);
    localSaveLocation->setEnabled(enabled);
    selectSavePathButton->setEnabled(enabled);
}

void Window::localSaveLocChanged(){
    setSavePath(localSaveLocation->text());
}

void Window::enableLinkToClipboardChanged(bool enabled){
    s.setValue(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD, enabled);
}

void Window::enableLinkToBrowserChanged(bool enabled){
    s.setValue(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER, enabled);
}

// a lot of these are radio buttons, in which case the one that is checked
// has the sole responsibility of updating the settings value

void Window::trayDoubleClickedSettingsChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS);
}

void Window::trayDoubleClickedCaptureChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_CAPTURE);
}

void Window::trayDoubleClickedUploadChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS);
}

void Window::compressionAlwaysChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::COMPRESSION_PHILOSOPHY, Settings::JPG_ALWAYS);
}

void Window::compressionSmartChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::COMPRESSION_PHILOSOPHY, Settings::IMAGE_TYPE_SMALLER);
}

void Window::contextShowExplorerChanged(bool enabled){
    // FIXME this needs to be done...
    s.setValue(Settings::ENABLE_EXPLORER_CONTEXT_MENU, enabled);
}

void Window::fullscreenCaptureAllChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::FULLSCREEN_CAPTURE_MODE, Settings::ALL_DESKTOPS);
}

void Window::fullscreenCaptureCursorChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::FULLSCREEN_CAPTURE_MODE, Settings::CURRENT_DESKTOP);
}

void Window::fullscreenCapturePrimaryChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::FULLSCREEN_CAPTURE_MODE, Settings::PRIMARY_DESKTOP);
}

void Window::dangerousExperimentalEnableChanged(bool enabled){
    s.setValue(Settings::ENABLE_EXPERIMENTAL_FEATURES, enabled);
}

void Window::dangerousNoSelectionRectChanged(bool enabled){
    s.setValue(Settings::SHOW_SELECTION_RECTANGLE, !enabled);
}
