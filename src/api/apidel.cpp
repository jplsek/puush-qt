#include "apidel.h"

#include <iostream>
#include <QByteArray>
#include <QDataStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QString>

const QString ApiDel::errorStrings[] = {
    [ApiDel::Error::NoError] = "No Error",
    [ApiDel::Error::Failure] = "Request failed",
    [ApiDel::Error::InvalidResponse] = "Invalid response from puush",
};

ApiDel::ApiDel(const QString &apikey, const QString &id):
    ApiRequest()
{
    data.addQueryItem("k", QUrl::toPercentEncoding(apikey));
    data.addQueryItem("i", QUrl::toPercentEncoding(id));
}

const QString ApiDel::urlext(){
    return "del";
}

void ApiDel::handleResponse(){
    std::cout << "ApiDel::handleResponse()" << std::endl;
    // nothing to actually handle. the response code is enough.
}

void ApiDel::done(){
    emit done(this);
}

const QString ApiDel::errorStr(){
    if(reply->error() != QNetworkReply::NoError){
        return reply->errorString();
    }
    return errorStrings[status];
}

