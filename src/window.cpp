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
    tabs->addTab(createTabGeneral(),     tr("General"));
    tabs->addTab(createTabKeyBindings(), tr("Key Bindings")); // this should be handled by a library that interfaces with the desktop environment
    tabs->addTab(createTabAccount(),     tr("Account"));
    tabs->addTab(createTabAdvanced(),    tr("Advanced"));
    tabs->addTab(createTabHistory(),     tr("History"));
    tabs->addTab(createTabAbout(),       tr("About"));
    return tabs;
}

QWidget *Window::createTabGeneral(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *puushGroup = new QGroupBox(tr("On Successful Puush"));
    QVBoxLayout *puushLayout = new QVBoxLayout();

    enablePuushSound = new QCheckBox(tr("Play a sound"));
    puushLayout->addWidget(enablePuushSound);

    enableLocalSave = new QCheckBox(tr("Enable local save"));
    puushLayout->addWidget(enableLocalSave);

    QWidget *saveLocation = new QWidget();
    QHBoxLayout *saveLocationLayout = new QHBoxLayout();
    selectSavePathButton = new QPushButton(tr("..."));
    localSaveLocation = new QLineEdit();
    saveLocationLayout->addWidget(localSaveLocation);
    saveLocationLayout->addWidget(selectSavePathButton);
    saveLocation->setLayout(saveLocationLayout);
    puushLayout->addWidget(saveLocation);

    enableLinkToClipboard = new QCheckBox(tr("Copy link to clipboard"));
    puushLayout->addWidget(enableLinkToClipboard);

    enableLinkToBrowser = new QCheckBox(tr("Open link in browser"));
    puushLayout->addWidget(enableLinkToBrowser);

    puushGroup->setLayout(puushLayout);
    qhb->addWidget(puushGroup);

    QGroupBox *trayGroup = new QGroupBox(tr("Tray Icon Behavior on Double Click"));
    QVBoxLayout *trayLayout = new QVBoxLayout();
    trayDoubleClickSettings       = new QRadioButton(tr("Show settings window"));
    trayDoubleClickUpload         = new QRadioButton(tr("Open screenshot directory"));
    trayDoubleClickUploadFile     = new QRadioButton(tr("Upload file"));
    trayDoubleClickCaptureDesktop = new QRadioButton(tr("Capture desktop"));
    trayDoubleClickCaptureArea    = new QRadioButton(tr("Capture area"));
    trayDoubleClickCaptureWindow  = new QRadioButton(tr("Capture current window"));
    trayDoubleClickAccount        = new QRadioButton(tr("Open account"));
    trayLayout->addWidget(trayDoubleClickSettings);
    trayLayout->addWidget(trayDoubleClickUpload);
    trayLayout->addWidget(trayDoubleClickUploadFile);
    trayLayout->addWidget(trayDoubleClickCaptureDesktop);
    trayLayout->addWidget(trayDoubleClickCaptureArea);
    trayLayout->addWidget(trayDoubleClickCaptureWindow);
    trayLayout->addWidget(trayDoubleClickAccount);
    trayGroup->setLayout(trayLayout);
    qhb->addWidget(trayGroup);

    qhb->addStretch();

    resetGeneralButton = new QPushButton(tr("Reset General Settings"));
    qhb->addWidget(resetGeneralButton);

    w->setLayout(qhb);

    updateTabGeneral();

    return w;
}

void Window::updateTabGeneral() {
    enablePuushSound->setChecked(s.value(Settings::ON_PUUSH_SOUND).toBool());
    enableLocalSave->setChecked(s.value(Settings::LOCAL_SAVE_ENABLED).toBool());
    localSaveLocation->setText(s.value(Settings::LOCAL_SAVE_PATH).toString());
    enableLinkToClipboard->setChecked(s.value(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD).toBool());
    enableLinkToBrowser->setChecked(s.value(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER).toBool());
    trayDoubleClickSettings->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS));
    trayDoubleClickUpload->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS));
    trayDoubleClickUploadFile->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_FILE));
    trayDoubleClickCaptureDesktop->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_DESKTOP));
    trayDoubleClickCaptureArea->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_AREA));
    trayDoubleClickCaptureWindow->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_WINDOW));
    trayDoubleClickAccount->setChecked(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_ACCOUNT));

    enableLocalSaveChanged(s.value(Settings::LOCAL_SAVE_ENABLED).toBool());
}

QWidget *Window::createTabKeyBindings(){
    QWidget *w = new QWidget();
    QVBoxLayout *qhb = new QVBoxLayout();

    QGroupBox *keyBindings = new QGroupBox(tr("Key Bindings"));
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

    QGroupBox *account = new QGroupBox(tr("Account Details"));
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

    QGroupBox *screenBox = new QGroupBox(tr("Screen Capture Quality"));
    QVBoxLayout *screenLayout = new QVBoxLayout();
    compressionAlways = new QRadioButton(tr("Always use PNG (no lossy compression)"));
    compressionSmart  = new QRadioButton(tr("Smart (use JPG unless PNG is smaller)"));
    compressionAlways->setEnabled(false);
    compressionSmart->setEnabled(false);

    QFormLayout *qualityLayout = new QFormLayout();
    QWidget *quality = new QWidget();
    qualitySlider = new QSlider(Qt::Horizontal);
    qualitySlider->setRange(0, 100);
    qualitySlider->setTickPosition(QSlider::TicksBelow);
    qualitySlider->setTickInterval(10);

    qualityLayout->addRow(tr("Quality:"), qualitySlider);
    quality->setLayout(qualityLayout);

    screenLayout->addWidget(compressionAlways);
    screenLayout->addWidget(compressionSmart);
    screenLayout->addWidget(quality);
    screenBox->setLayout(screenLayout);

    QGroupBox *fullscreenBox = new QGroupBox(tr("Fullscreen Capture"));
    QVBoxLayout *fullscreenLayout = new QVBoxLayout();
    fullscreenCaptureAll     = new QRadioButton(tr("Capture all screens"));
    fullscreenCaptureCursor  = new QRadioButton(tr("Capture screen containing mouse cursor"));
    fullscreenCapturePrimary = new QRadioButton(tr("Always capture primary screen"));

    fullscreenLayout->addWidget(fullscreenCaptureAll);
    fullscreenLayout->addWidget(fullscreenCaptureCursor);
    fullscreenLayout->addWidget(fullscreenCapturePrimary);
    fullscreenBox->setLayout(fullscreenLayout);

    resetAdvancedButton = new QPushButton(tr("Reset Advanced Settings"));

    qhb->addWidget(screenBox);
    qhb->addWidget(fullscreenBox);
    qhb->addStretch();
    qhb->addWidget(resetAdvancedButton);

    w->setLayout(qhb);

    updateTabAdvanced();

    return w;
}

void Window::updateTabAdvanced() {
    compressionAlways->setChecked(s.radioValueIs(Settings::COMPRESSION_PHILOSOPHY, Settings::PNG_ALWAYS));
    compressionSmart->setChecked(s.radioValueIs(Settings::COMPRESSION_PHILOSOPHY, Settings::IMAGE_TYPE_SMALLER));
    qualitySlider->setValue(s.value(Settings::IMAGE_QUALITY).toInt());
    fullscreenCaptureAll->setChecked(s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::ALL_DESKTOPS));
    fullscreenCaptureCursor->setChecked(s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::CURRENT_DESKTOP));
    fullscreenCapturePrimary->setChecked(s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::PRIMARY_DESKTOP));
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
    switch (reason) {
    // left click
    case QSystemTrayIcon::Trigger:
        trayIconMenu->popup(QCursor::pos());
        break;
    // double left click
    case QSystemTrayIcon::DoubleClick:
        doDoubleClickAction();
        break;
    default:
        break;
    }
}

void Window::doDoubleClickAction() {
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
        std::cout << "Option not recognized" << std::endl;
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
    if (lastUrl == QUrl(""))
        return;

    openUrl(lastUrl);

    // reset it so if an error popup comes up later, the url wont open
    lastUrl = QUrl("");
}

void Window::openUrl(QUrl url) {
    bool response = QDesktopServices::openUrl(url);

    if (!response) {
        trayIcon->showMessage(tr("Error!"), tr("There was an issue opening the URL. Is your default browser set?"), QSystemTrayIcon::MessageIcon());
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
    connect(activeAction, SIGNAL(triggered()), this, SLOT(activeWindowScreenshotTimed()));

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
    connect(enableLocalSave,  SIGNAL(clicked(bool)), this, SLOT(enableLocalSaveChanged(bool)));

    connect(localSaveLocation,    SIGNAL(editingFinished()), this, SLOT(localSaveLocChanged()));
    connect(selectSavePathButton, SIGNAL(clicked()), this, SLOT(setSaveDirectory()));

    connect(enableLinkToClipboard, SIGNAL(clicked(bool)), this, SLOT(enableLinkToClipboardChanged(bool)));
    connect(enableLinkToBrowser,   SIGNAL(clicked(bool)), this, SLOT(enableLinkToBrowserChanged(bool)));

    connect(trayDoubleClickSettings,       SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedSettingsChanged(bool)));
    connect(trayDoubleClickUpload,         SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedUploadChanged(bool)));
    connect(trayDoubleClickUploadFile,     SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedUploadFileChanged(bool)));
    connect(trayDoubleClickCaptureDesktop, SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedCaptureDesktopChanged(bool)));
    connect(trayDoubleClickCaptureArea,    SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedCaptureAreaChanged(bool)));
    connect(trayDoubleClickCaptureWindow,  SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedCaptureWindowChanged(bool)));
    connect(trayDoubleClickAccount,        SIGNAL(clicked(bool)), this, SLOT(trayDoubleClickedAccountChanged(bool)));

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

    connect(fullscreenCaptureAll,     SIGNAL(clicked(bool)), this, SLOT(fullscreenCaptureAllChanged(bool)));
    connect(fullscreenCaptureCursor,  SIGNAL(clicked(bool)), this, SLOT(fullscreenCaptureCursorChanged(bool)));
    connect(fullscreenCapturePrimary, SIGNAL(clicked(bool)), this, SLOT(fullscreenCapturePrimaryChanged(bool)));

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
 * Gets the file name to use for screenshots
 * @brief Window::getFileName
 * @return
 */
QString Window::getFileName() {
    return QDateTime::currentDateTime().toString(s.value(Settings::LOCAL_SAVE_NAME).toString()) + ".png";
}

/**
 * Generates the temporary screenshot file names.
 * @brief Window::getTempPath
 * @return
 */
QString Window::getTempPath() {
    return QDir::tempPath() + QDir::separator() + getFileName();
}

/**
 * Opens a folder dialog to set the screenshot save path.
 * @brief Window::setSaveDirectory
 */
void Window::setSaveDirectory(){
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Set Screenshot Directory"),
                s.value(Settings::LOCAL_SAVE_PATH).toString(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // "" if canceled
    if (dir != "") {
        localSaveLocation->setText(dir);
        setSaveDirectory(dir);
    }
}

/**
 * Parses the screenshot save path.
 * @brief Window::getSaveDirectory
 * @return
 */
QString Window::getSaveDirectory(){
    QString path = s.value(Settings::LOCAL_SAVE_PATH).toString();

    if(path.startsWith("~/")){
        path.remove(0, 1); // remove "~"
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + path; // add "/home/user"
    }

    return path;
}

/**
 * Generates the screenshot name based on the current settings.
 * @brief Window::getSavePath
 * @return
 */
QString Window::getSavePath() {
    return getSaveDirectory() + "/" + getFileName();
}

/**
 * Puush capture desktop screenshot.
 * @brief Window::fullScreenScreenshot
 */
void Window::fullScreenScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getTempPath();
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

    QString fileName = getTempPath();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->selectArea();
}

void Window::activeWindowScreenshotTimed() {
    if (!isLoggedIn()) return;

    trayIcon->showMessage(tr("Select a window"),
                          tr("Taking a screenshot in ") + QString::number(defaultSelectionTimeout) + tr(" seconds..."),
                          QSystemTrayIcon::MessageIcon());

    numTime = defaultSelectionTimeout;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateActiveMessage()));
    timer->start(1000);
}

/**
 * Initiate the Puush current window screenshot.
 * @brief Window::activeWindowScreenshot
 */
void Window::activeWindowScreenshot() {
    if (!isLoggedIn()) return;

    QString fileName = getTempPath();
    Screenshot *ss = new Screenshot(fileName);
    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDone(int, QString, QString)));
    ss->activeWindow();
}

/**
 * Shows the countdown message leading to the current window screenshot.
 * Once it is done, it will create the Puush current window screenshot.
 * @brief Window::updateActiveMessage
 */
void Window::updateActiveMessage() {
    if (numTime < 1) {
        timer->stop();
        activeWindowScreenshot();
        return;
    }

    --numTime;
}

/**
 * Opens the screenshot directory if the user has enabled saving local screenshots.
 * @brief Window::openSaveDirectory
 */
void Window::openSaveDirectory() {
    QString path = getSaveDirectory();

    bool response = QDesktopServices::openUrl(QUrl::fromLocalFile(path));

    if (!response) {
        trayIcon->showMessage(tr("Error!"), tr("Error opening save directory"), QSystemTrayIcon::MessageIcon());
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
        trayIcon->showMessage(tr("Error!"), output, QSystemTrayIcon::MessageIcon());
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
 * Notify the user when the upload to Puush has been completed.
 * @brief Window::puushDone
 * @param returnCode
 * @param output
 */
void Window::puushDone(int returnCode, QString output) {
    if (returnCode != 0) {
        trayIcon->showMessage(tr("Error!"), output, QSystemTrayIcon::MessageIcon());
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

    if (s.value(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER).toBool()) {
        openUrl(url);
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
 * Save the save screenshot directory path to the settings.
 * @brief Window::setSaveDirectory
 * @param path
 */
void Window::setSaveDirectory(QString path){
    if(path.startsWith("~/")){
        path.remove(0, 1); // remove "~"
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + path; // add "/home/user"
        // update input with the new path
        localSaveLocation->setText(path);
    }

    s.setValue(Settings::LOCAL_SAVE_PATH, path);
}

void Window::resetGeneralSettings(){
    s.resetGeneralSettings();
    updateTabGeneral();
}

void Window::resetAdvancedSettings(){
    s.resetAdvancedSettings();
    updateTabAdvanced();
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
    setSaveDirectory(localSaveLocation->text());
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

void Window::trayDoubleClickedUploadChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS);
}

void Window::trayDoubleClickedUploadFileChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_FILE);
}

void Window::trayDoubleClickedCaptureDesktopChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_DESKTOP);
}

void Window::trayDoubleClickedCaptureAreaChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_AREA);
}

void Window::trayDoubleClickedCaptureWindowChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_WINDOW);
}

void Window::trayDoubleClickedAccountChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_ACCOUNT);
}

void Window::compressionAlwaysChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::COMPRESSION_PHILOSOPHY, Settings::JPG_ALWAYS);
}

void Window::compressionSmartChanged(bool enabled){
    if(enabled)
        s.setRadioValue(Settings::COMPRESSION_PHILOSOPHY, Settings::IMAGE_TYPE_SMALLER);
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
