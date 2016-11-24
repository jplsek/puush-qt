#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QString>
#include <QProcess>
#include <QRect>
#include <QPoint>
#include <QPixmap>
#include <QScreen>
#include <QSettings>

#include "transparentwindow.h"
#include "settings.h"

class Screenshot : public QObject {
    Q_OBJECT

public:
    Screenshot(QString);
    void selectArea();
    void fullScreen();
    void activeWindow();
    void croppedScreenshot(QRect);

signals:
    int finished(int, QString, QString);

private slots:
    void screenshotDone(int);
    void selectionDone(QRect);
    void fullScreenAfterTimer();

private:
    Settings s;

    QString fn;
    QString quality;

    QProcess *screenshotProcess;

    TransparentWindow *tw;

    QPixmap desktop();
    QPixmap primaryScreen();
    QPixmap screen(int);
    QPixmap screen(QScreen*);
    void save(QPixmap, QString = "");
};

#endif
