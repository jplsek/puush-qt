#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QUrlQuery>

class QNetworkRequest;
class QNetworkReply;
class QString;

class ApiRequest :public QObject {
    Q_OBJECT

protected:
    ApiRequest();

public:
    ~ApiRequest();

    const QString PUUSH_API_URL_BASE = "https://puush.me/api/";

    // returns the api extension specific to the api call
    virtual const QString urlext() = 0;

    // Start http request. Binds response signal to requestFinished
    void start();

    // Has there been an error?
    bool failed();
    // What error?
    virtual const QString errorStr() = 0;

    // called when the final signal should be thrown.
    // needs to be virtual beacuse the childred types need to emit their own types.
    virtual void done() = 0;

protected slots:
    // called to handle error detection of completed http request.
    void readResponse();

protected:
    // gets called if there was no error.
    // parses response into proper fields.
    virtual void handleResponse() = 0;

protected:
    QUrlQuery data;

    QNetworkRequest *qnr;
    QNetworkReply *reply;

    qint32 status;
    QByteArray response;
};

#endif // APIREQUEST_H
