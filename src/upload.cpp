#include <QtGui>
#include <QSettings>
#include <curl/curl.h>

#include "upload.h"

QString url = "https://puush.me/api/";

Upload::Upload(QString fileName) {
    QSettings s;

    QString key = s.value("key").toString();

    // This uses curl instead of QT because of issues getting uploading files to work.
    // I should actually use libcurl...
    uploadProcess = new QProcess();

    connect(uploadProcess, SIGNAL(started()), this, SLOT(uploadStarted()));
    connect(uploadProcess, SIGNAL(finished(int)), this, SLOT(uploadDone(int)));

    uploadProcess->start(
                "curl", QStringList() <<
                url + "up" << "-Ss" << "-F" << "k=" + key << "-F" << "z=poop" << "-F" << "f=@" + fileName);
}

void Upload::uploadStarted() {
    started();
}

void Upload::uploadDone(int returnCode) {
    finished(returnCode, uploadProcess->readAll());
    uploadProcess->deleteLater();
}
