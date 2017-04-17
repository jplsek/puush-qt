#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <QObject>
#include <QKeyEvent>

#include "settings.h"
#include "uglobalhotkeys.h"

class KeyBindings : public QObject
{
    Q_OBJECT
public:
    explicit KeyBindings(QObject *parent = 0);

    Q_INVOKABLE QString getKeySequence(int, int);
    Q_INVOKABLE bool isProperBinding(int, int);

    Q_INVOKABLE QString getUploadFile();
    Q_INVOKABLE QString getCaptureDesktop();
    Q_INVOKABLE QString getCaptureArea();
    Q_INVOKABLE QString getCaptureWindow();
    Q_INVOKABLE QString getUploadClipboard();
    Q_INVOKABLE QString getTogglePuush();

    Q_INVOKABLE void setUploadFile(QString);
    Q_INVOKABLE void setCaptureDesktop(QString);
    Q_INVOKABLE void setCaptureArea(QString);
    Q_INVOKABLE void setCaptureWindow(QString);
    Q_INVOKABLE void setUploadClipboard(QString);
    Q_INVOKABLE void setTogglePuush(QString);

    Q_INVOKABLE QString resetUploadFile();
    Q_INVOKABLE QString resetCaptureDesktop();
    Q_INVOKABLE QString resetCaptureArea();
    Q_INVOKABLE QString resetCaptureWindow();
    Q_INVOKABLE QString resetUploadClipboard();
    Q_INVOKABLE QString resetTogglePuush();

private:
    Settings s;
    QList<int> modifierKeys;
    UGlobalHotkeys *hotkeyManager;

    void registerSequence(Settings::Setting);

signals:

public slots:
};

#endif // KEYBINDINGS_H
