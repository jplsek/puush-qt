#include "apidel.h"

#include <QNetworkRequest>
#include <QNetworkReply>

const QString ApiDel::errorStrings[] = {
    [ApiDel::Error::NoError] = "No Error",
    [ApiDel::Error::Failure] = "Request failed",
    [ApiDel::Error::InvalidResponse] = "Invalid response from puush",
};

ApiDel::ApiDel(const QString &apiurl, const QString &apikey, const QString &id):
    ApiRequest()
{
    qDebug() << "ApiDel::ApiDel()";

    url = apiurl;

    data.addQueryItem("k", QUrl::toPercentEncoding(apikey));
    data.addQueryItem("i", QUrl::toPercentEncoding(id));
}

const QString ApiDel::urlext()
{
    return "del";
}

void ApiDel::handleResponse()
{
    qDebug() << "ApiDel::handleResponse(): " << response;
    // nothing to actually handle. the response code is enough.
}

int ApiDel::allData()
{
    return response.toInt();
}

void ApiDel::done()
{
    emit done(this);
}

const QString ApiDel::errorStr()
{
    if (reply->error() != QNetworkReply::NoError) {
        return reply->errorString();
    }
    return errorStrings[status];
}

