#ifndef APIAUTH_H
#define APIAUTH_H

#include "apirequest.h"
#include <QObject>
#include <QString>

class ApiAuth : public ApiRequest
{
    Q_OBJECT
public:
    struct AuthData {
        bool premium;
        QString apikey;
        int expiry; // this is of unknown units.
        int diskUsage; // maybe in MB/MiB
    };

    enum Error {
        NoError = 0,
        InvaildCredentials = 1,
        InvalidResponse = 2,
    };

public:
    ApiAuth(const QString &apiurl, const QString &email, const QString &password);
    ApiAuth(const QString &apiurl, const QString &apikey);

    const QString urlext();

    const QString errorStr();

    const AuthData allData();
    bool hasPremium();
    const QString apikey();
    int expiry();
    const QString expiryString();
    int diskUsage();

    static const QString expiryReadableString(int expiry);

protected:
    void done();

signals:
    void done(ApiAuth *);

protected:
    void handleResponse();

private:
    AuthData auth;
};

std::ostream &operator<<(std::ostream &, ApiAuth::AuthData &);

#endif // APIAUTH_H
