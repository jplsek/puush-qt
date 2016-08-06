#include <QtGui>
#include <QProcess>

#include "screenshot.h"

Screenshot::Screenshot(QString fileName) {
    fn = fileName;
    quality = s.value("quality").toString();
}

void Screenshot::selectArea() {
    screenshotProcess = new QProcess();
    screenshotProcess->start("scrot", QStringList() << "-b" << "-q" << quality << "-s" << fn);

    connect(screenshotProcess, SIGNAL(finished(int)), this, SLOT(screenshotDone(int)));
}

void Screenshot::fullScreen() {
    screenshotProcess = new QProcess();
    screenshotProcess->start("scrot", QStringList() << "-b" << "-q" << quality << fn);

    connect(screenshotProcess, SIGNAL(finished(int)), this, SLOT(screenshotDone(int)));
}

void Screenshot::activeWindow() {
    screenshotProcess = new QProcess();
    screenshotProcess->start("scrot", QStringList() << "-b" << "-q" << quality << "-u" << fn);

    connect(screenshotProcess, SIGNAL(finished(int)), this, SLOT(screenshotDone(int)));
}

void Screenshot::screenshotDone(int returnCode) {
    QString output = screenshotProcess->readAll();
    screenshotProcess->deleteLater();
    finished(returnCode, fn, output);
}
