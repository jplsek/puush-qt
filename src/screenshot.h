#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QString>
#include <QProcess>
#include <QSettings>

class Screenshot : public QObject {
    Q_OBJECT

public:
    Screenshot(QString);
    void selectArea();
    void fullScreen();
    void activeWindow();

signals:
    int finished(int, QString, QString);

private slots:
    void screenshotDone(int);

private:
    QSettings s;

    QString fn;
    QString quality;

    QProcess *screenshotProcess;
};

#endif
