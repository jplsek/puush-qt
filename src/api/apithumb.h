#ifndef APITHUMB_H
#define APITHUMB_H

#include "apirequest.h"

#include <QObject>
#include <QString>
#include <QImage>

class ApiThumb : public ApiRequest
{
    Q_OBJECT
public:

    enum Error {
        NoError = 0,
        Failure = 1,
        ImageLoadFailed = 2,
    };

public:
    ApiThumb(const QString &apiurl, const QString &apikey, const QString &id);

    const QString urlext();

    const QString errorStr();

    const QImage thumb();

protected:
    void done();

signals:
    void done(ApiThumb *);

protected:
    void handleResponse();

private:
   QImage image;
};

#endif // APITHUMB_H
