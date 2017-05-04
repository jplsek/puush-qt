#ifndef INFORMATION_H
#define INFORMATION_H

#include <QObject>

#include "settings.h"

class Information : public QObject
{
    Q_OBJECT
public:
    explicit Information(QObject *parent = 0);

    Q_INVOKABLE QString getVersion();
    Q_INVOKABLE int getQuality();
    Q_INVOKABLE void getQtAbout();

    Q_INVOKABLE bool settingIsOnPuushSound();
    Q_INVOKABLE bool settingIsLocalSaveEnabled();
    Q_INVOKABLE bool settingIsOnPuushCopyLinkToClipboard();
    Q_INVOKABLE bool settingIsOnPuushOpenLinkInBrowser();

    Q_INVOKABLE bool settingIsTrayClickActionOpenSettings();
    Q_INVOKABLE bool settingIsTrayClickActionOpenUploads();
    Q_INVOKABLE bool settingIsTrayClickActionUploadFile();
    Q_INVOKABLE bool settingIsTrayClickActionUploadClipboard();
    Q_INVOKABLE bool settingIsTrayClickActionCaptureDesktop();
    Q_INVOKABLE bool settingIsTrayClickActionCaptureArea();
    Q_INVOKABLE bool settingIsTrayClickActionCaptureWindow();
    Q_INVOKABLE bool settingIsTrayClickActionOpenAccount();

    Q_INVOKABLE bool settingIsCompressionPng();
    Q_INVOKABLE bool settingIsCompressionSmaller();

    Q_INVOKABLE bool settingIsFullscreenAllDesktops();
    Q_INVOKABLE bool settingIsFullscreenCurrentDesktop();
    Q_INVOKABLE bool settingIsFullscreenPrimaryDesktop();

    Q_INVOKABLE QString settingGetLocalSavePath();
    Q_INVOKABLE QString settingGetPuushUrl();
    Q_INVOKABLE QString settingGetPuushApiUrl();

    Q_INVOKABLE void settingSetOnPuushSound(bool);
    Q_INVOKABLE void settingSetLocalSaveEnabled(bool);
    Q_INVOKABLE void settingSetLocalSavePath(QString);
    Q_INVOKABLE void settingSetPuushUrl(QString);
    Q_INVOKABLE void settingSetPuushApiUrl(QString);
    Q_INVOKABLE void setQuality(int);
    Q_INVOKABLE void settingSetOnPuushCopyLinkToClipboard(bool);
    Q_INVOKABLE void settingSetOnPuushOpenLinkInBrowser(bool);

    Q_INVOKABLE void settingSetTrayClickActionOpenSettings(bool);
    Q_INVOKABLE void settingSetTrayClickActionOpenUploads(bool);
    Q_INVOKABLE void settingSetTrayClickActionUploadFile(bool);
    Q_INVOKABLE void settingSetTrayClickActionUploadClipboard(bool);
    Q_INVOKABLE void settingSetTrayClickActionCaptureDesktop(bool);
    Q_INVOKABLE void settingSetTrayClickActionCaptureArea(bool);
    Q_INVOKABLE void settingSetTrayClickActionCaptureWindow(bool);
    Q_INVOKABLE void settingSetTrayClickActionOpenAccount(bool);

    Q_INVOKABLE void settingSetCompressionPng(bool);
    Q_INVOKABLE void settingSetCompressionSmaller(bool);

    Q_INVOKABLE void settingSetFullscreenAllDesktops(bool);
    Q_INVOKABLE void settingSetFullscreenCurrentDesktop(bool);
    Q_INVOKABLE void settingSetFullscreenPrimaryDesktop(bool);

    Q_INVOKABLE QString getEmail();
    Q_INVOKABLE QString getApiKey();
    Q_INVOKABLE bool isPremium();
    Q_INVOKABLE int getPremiumExpire();
    Q_INVOKABLE int getDiskUsage();

private:
    Settings s;

signals:

public slots:

};

#endif // INFORMATION_H
