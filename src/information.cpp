#include "information.h"
#include <QCoreApplication>
#include <QApplication>
#include <QDebug>

Information::Information(QObject *parent) : QObject(parent) { }

QString Information::getVersion() {
    return QCoreApplication::applicationVersion();
}

int Information::getQuality() {
    return s.value(Settings::IMAGE_QUALITY).toInt();
}

void Information::getQtAbout() {
    QApplication::aboutQt();
}

bool Information::settingIsOnPuushSound() {
    return s.value(Settings::ON_PUUSH_SOUND).toBool();
}

bool Information::settingIsLocalSaveEnabled() {
    return s.value(Settings::LOCAL_SAVE_ENABLED).toBool();
}

QString Information::settingGetLocalSavePath() {
    return s.value(Settings::LOCAL_SAVE_PATH).toString();
}

QString Information::settingGetPuushUrl() {
    return s.value(Settings::BASE_URL).toString();
}

QString Information::settingGetPuushApiUrl() {
    return s.value(Settings::API_URL).toString();
}

bool Information::settingIsOnPuushCopyLinkToClipboard() {
    return s.value(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD).toBool();
}

bool Information::settingIsOnPuushOpenLinkInBrowser() {
    return s.value(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER).toBool();
}

bool Information::settingIsTrayClickActionOpenSettings() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS);
}

bool Information::settingIsTrayClickActionOpenUploads() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS);
}

bool Information::settingIsTrayClickActionUploadFile() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_FILE);
}

bool Information::settingIsTrayClickActionUploadClipboard() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_CLIPBOARD);
}

bool Information::settingIsTrayClickActionCaptureDesktop() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_DESKTOP);
}

bool Information::settingIsTrayClickActionCaptureArea() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_AREA);
}

bool Information::settingIsTrayClickActionCaptureWindow() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_WINDOW);
}

bool Information::settingIsTrayClickActionOpenAccount() {
    return s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_ACCOUNT);
}

bool Information::settingIsCompressionPng() {
    return s.radioValueIs(Settings::COMPRESSION_PHILOSOPHY, Settings::PNG_ALWAYS);
}

bool Information::settingIsCompressionSmaller() {
    return s.radioValueIs(Settings::COMPRESSION_PHILOSOPHY, Settings::IMAGE_TYPE_SMALLER);
}

bool Information::settingIsFullscreenAllDesktops() {
    return s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::ALL_DESKTOPS);
}

bool Information::settingIsFullscreenCurrentDesktop() {
    return s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::CURRENT_DESKTOP);
}

bool Information::settingIsFullscreenPrimaryDesktop() {
    return s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::PRIMARY_DESKTOP);
}

void Information::settingSetOnPuushSound(bool enabled) {
    s.setValue(Settings::ON_PUUSH_SOUND, enabled);
}

void Information::settingSetLocalSaveEnabled(bool enabled) {
    s.setValue(Settings::LOCAL_SAVE_ENABLED, enabled);
}

void Information::settingSetLocalSavePath(QString path) {
    s.setValue(Settings::LOCAL_SAVE_PATH, path);
}

void Information::settingSetPuushUrl(QString url) {
    s.setValue(Settings::BASE_URL, url);
}

void Information::settingSetPuushApiUrl(QString url) {
    s.setValue(Settings::API_URL, url);
}

void Information::setQuality(int quality) {
    s.setValue(Settings::IMAGE_QUALITY, quality);
}

void Information::settingSetOnPuushCopyLinkToClipboard(bool enabled) {
    s.setValue(Settings::ON_PUUSH_COPY_LINK_TO_CLIPBOARD, enabled);
}

void Information::settingSetOnPuushOpenLinkInBrowser(bool enabled) {
    s.setValue(Settings::ON_PUUSH_OPEN_LINK_IN_BROWSER, enabled);
}

void Information::settingSetTrayClickActionOpenSettings(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_SETTINGS);
}

void Information::settingSetTrayClickActionOpenUploads(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS);
}

void Information::settingSetTrayClickActionUploadFile(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_FILE);
}

void Information::settingSetTrayClickActionUploadClipboard(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::UPLOAD_CLIPBOARD);
}

void Information::settingSetTrayClickActionCaptureDesktop(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_DESKTOP);
}

void Information::settingSetTrayClickActionCaptureArea(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_AREA);
}

void Information::settingSetTrayClickActionCaptureWindow(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::CAPTURE_WINDOW);
}

void Information::settingSetTrayClickActionOpenAccount(bool enabled) {
    if (enabled) s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_ACCOUNT);
}

void Information::settingSetCompressionPng(bool enabled) {
    if (enabled) s.setRadioValue(Settings::COMPRESSION_PHILOSOPHY, Settings::PNG_ALWAYS);
}

void Information::settingSetCompressionSmaller(bool enabled) {
    if (enabled) s.setRadioValue(Settings::COMPRESSION_PHILOSOPHY, Settings::IMAGE_TYPE_SMALLER);
}

void Information::settingSetFullscreenAllDesktops(bool enabled) {
    if (enabled) s.setRadioValue(Settings::FULLSCREEN_CAPTURE_MODE, Settings::ALL_DESKTOPS);
}

void Information::settingSetFullscreenCurrentDesktop(bool enabled) {
    if (enabled) s.setRadioValue(Settings::FULLSCREEN_CAPTURE_MODE, Settings::CURRENT_DESKTOP);
}

void Information::settingSetFullscreenPrimaryDesktop(bool enabled) {
    if (enabled) s.setRadioValue(Settings::FULLSCREEN_CAPTURE_MODE, Settings::PRIMARY_DESKTOP);
}

QString Information::getEmail() {
    return s.value(Settings::ACCOUNT_EMAIL).toString();
}

QString Information::getApiKey() {
    return s.value(Settings::ACCOUNT_API_KEY).toString();
}

bool Information::isPremium() {
    return s.value(Settings::ACCOUNT_PREMIUM).toBool();
}

int Information::getPremiumExpire() {
    return s.value(Settings::ACCOUNT_PREMIUM_EXPIRY).toInt();
}

int Information::getDiskUsage() {
    return s.value(Settings::ACCOUNT_DISK_USAGE).toInt();
}
