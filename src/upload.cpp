#include "upload.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>

#include "settings.h"

Upload::Upload(QString fileName) {
    Settings s;

    QString key = s.value(Settings::ACCOUNT_API_KEY).toString();

    // remove file:// if it exists
    fileName = fileName.remove("file://");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    QNetworkRequest request;
    request.setUrl(QUrl(s.value(Settings::API_URL).toString() + "up"));

    QFile *file = new QFile(fileName);
    file->open(QIODevice::ReadOnly);

    QString bound="puushqtboundaryapirequest";
    QByteArray postData(QString("--" + bound + "\r\n").toUtf8());
    postData.append("Content-Disposition: form-data; name=\"k\"\r\n\r\n");
    postData.append(key + "\r\n");
    postData.append("--" + bound + "\r\n");
    postData.append("Content-Disposition: form-data; name=\"z\"\r\n\r\n");
    postData.append("poop\r\n");
    postData.append("--" + bound + "\r\n");
    postData.append("Content-Disposition: form-data; name=\"f\"; filename=\"");
    postData.append(fileName);
    postData.append("\"\r\n");
    postData.append("Content-Type: multipart/form-data\r\n\r\n");
    postData.append(file->readAll());
    postData.append("\r\n");
    postData.append("--" + bound + "--\r\n");

    delete file;

    request.setRawHeader("Content-Type", QString("multipart/form-data; boundary=" + bound).toUtf8());
    request.setRawHeader("Content-Length", QString::number(postData.length()).toUtf8());

    manager->post(request, postData);
}

void Upload::uploadStarted() {
    started();
}

void Upload::replyFinished(QNetworkReply *reply) {
    finished(reply->readAll());
}
