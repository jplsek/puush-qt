#ifndef APIDEL_H
#define APIDEL_H

#include "apirequest.h"
#include <QObject>
#include <QString>

class ApiDel : public ApiRequest
{
    Q_OBJECT
public:

    enum Error {
        NoError = 0,
        Failure = 1,
        InvalidResponse = 2,
    };

    const static QString errorStrings[];

public:
    ApiDel(const QString &apiurl, const QString &apikey, const QString &id);

    const QString urlext();

    const QString errorStr();

    int allData();

protected:
    void done();

signals:
    void done(ApiDel *);

protected:
    void handleResponse();
};

#endif // APIDEL_H
