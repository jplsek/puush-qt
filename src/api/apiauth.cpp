#include "apiauth.h"

#include <iostream>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <ctime>

static std::map<qint32, QString> errorStrings = {
    {ApiAuth::Error::NoError, "No Error"},
    {ApiAuth::Error::InvaildCredentials, "Invalid Username or Password"},
    {ApiAuth::Error::InvalidResponse, "Invalid response from puush"}
};

ApiAuth::ApiAuth(const QString &apiurl, const QString &email, const QString &password):
    ApiRequest()
{
    qDebug() << "ApiAuth::ApiAuth(e, p)";

    url = apiurl;

    data.addQueryItem("e", QUrl::toPercentEncoding(email));
    data.addQueryItem("p", QUrl::toPercentEncoding(password));

    auth.premium   = 0;
    auth.apikey    = "";
    auth.expiry    = 0;
    auth.diskUsage = 0;

    qDebug() << "ApiAuth::ApiAuth() status == " << status;
}

ApiAuth::ApiAuth(const QString &apiurl, const QString &apikey):
    ApiRequest()
{
    url = apiurl;
    data.addQueryItem("k", QUrl::toPercentEncoding(apikey));

    auth.premium   = 0;
    auth.apikey    = "";
    auth.expiry    = 0;
    auth.diskUsage = 0;
}

const QString ApiAuth::urlext()
{
    return "auth";
}

void ApiAuth::handleResponse()
{
    qDebug() << "ApiAuth::handleResponse()";

    QList<QByteArray> pieces = response.split(',');
    if (pieces.length() != 4) {
        status = InvalidResponse;
        return;
    }

    auth.premium   = pieces[0].toInt();
    auth.apikey    = pieces[1];
    auth.diskUsage = pieces[3].toInt();

    if (pieces[2].isEmpty()) {
        auth.expiry = 0;
    } else {
        auth.expiry = pieces[2].toInt();
    }

    std::cout << auth;
}

void ApiAuth::done()
{
    emit done(this);
}

const QString ApiAuth::errorStr()
{
    if (reply->error() != QNetworkReply::NoError)
        return reply->errorString();
    return errorStrings[status];
}

const ApiAuth::AuthData ApiAuth::allData()
{
    return auth;
}

bool ApiAuth::hasPremium()
{
    return auth.premium;
}

const QString ApiAuth::apikey()
{
    return auth.apikey;
}

int ApiAuth::expiry()
{
    return auth.expiry;
}

const QString ApiAuth::expiryString()
{
    return ApiAuth::expiryReadableString(auth.expiry);
}

const QString ApiAuth::expiryReadableString(int expiry)
{
    if (expiry <= 0) {
        return "Never";
    } else if (expiry > 1471070929L) {
        // since this is a pretty big number, assume that the unit is
        // seconds since the epoch. We'll just convert it to days.
        long diff = expiry - time(nullptr);
        diff = diff / (60 * 60 * 24); // seconds per day
        return QString::number(diff) + " Days";
    } else {
        // Welp, its maybe days...
        return QString(expiry) + " Days";
    }
}

int ApiAuth::diskUsage()
{
    return auth.diskUsage;
}


std::ostream &operator<<(std::ostream &s, ApiAuth::AuthData &d)
{
    s << "premium:   " << d.premium   << std::endl;
    s << "apikey:    " << d.apikey.toStdString() << std::endl;
    s << "expiry:    " << d.expiry    << std::endl;
    s << "diskUsage: " << d.diskUsage << std::endl;
    return s;
}
