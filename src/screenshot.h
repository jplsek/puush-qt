#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QString>
#include <QProcess>
#include <QRect>
#include <QPoint>
#include <QPixmap>
#include <QSettings>

#include "transparentwindow.h"

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
    QSettings s;

    QString fn;
    QString quality;

    QProcess *screenshotProcess;

    TransparentWindow *tw;

    QPixmap desktop();
    QPixmap screen(int);
    void save(QPixmap);
};

#endif
