#include "apithumb.h"

#include <iostream>

#include <QByteArray>
#include <QNetworkRequest>
#include <QNetworkReply>

const QString ApiThumb::errorStrings[] = {
    [ApiThumb::Error::NoError] = "No Error",
    [ApiThumb::Error::Failure] = "Request Failed",
    [ApiThumb::Error::ImageLoadFailed] = "Failed to read image from returned data",
};

ApiThumb::ApiThumb(const QString &apiurl, const QString &apikey, const QString &id):
    ApiRequest(),
    image()
{
    url = apiurl;
    data.addQueryItem("k", QUrl::toPercentEncoding(apikey));
    data.addQueryItem("i", QUrl::toPercentEncoding(id));
}

const QString ApiThumb::urlext()
{
    return "thumb";
}

void ApiThumb::handleResponse()
{
    qDebug() << "ApiThumb::handleResponse()";
    if (!image.loadFromData(response, "png")) {
        status = ImageLoadFailed;
    }
}

void ApiThumb::done()
{
    emit done(this);
}

const QString ApiThumb::errorStr()
{
    if (reply->error() != QNetworkReply::NoError) {
        return reply->errorString();
    }
    return errorStrings[status];
}

const QImage ApiThumb::thumb()
{
    return image;
}

