#include "upload.h"

#include "settings.h"

Upload::Upload(QString fileName) {
    Settings s;

    QString key = s.value(Settings::ACCOUNT_API_KEY).toString();

    // remove file:// if it exists
    fileName = fileName.remove("file://");

    // This uses curl instead of QT because of issues getting uploading files to work.
    // I should actually use libcurl...
    uploadProcess = new QProcess();

    connect(uploadProcess, SIGNAL(started()), this, SLOT(uploadStarted()));
    connect(uploadProcess, SIGNAL(finished(int)), this, SLOT(uploadDone(int)));

    uploadProcess->start("curl",
                         QStringList() << s.value(Settings::API_URL).toString() + "up"
                                       << "-Ss" << "-F"
                                       << "k=" + key << "-F" << "z=poop"
                                       << "-F" << "f=@" + fileName);

}

void Upload::uploadStarted() {
    started();
}

void Upload::uploadDone(int returnCode) {
    finished(returnCode, uploadProcess->readAll());
    uploadProcess->deleteLater();
}
