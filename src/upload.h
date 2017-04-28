#ifndef UPLOAD_H
#define UPLOAD_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

extern QString puushUrl;

class Upload : public QObject {
    Q_OBJECT

public:
    Upload(QString);

signals:
    int finished(QString);
    int started();

private slots:
    void uploadStarted();
    void replyFinished(QNetworkReply*);

private:
};

#endif // UPLOAD_H
