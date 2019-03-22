#include "apihist.h"

#include <iostream>
#include <QByteArray>
#include <QNetworkRequest>
#include <QNetworkReply>

static std::map<qint32, QString> errorStrings = {
    {ApiHist::Error::NoError, "No Error"},
    {ApiHist::Error::Failure, "Request failed"},
    {ApiHist::Error::InvalidResponse, "Invalid response from puush"}
};

ApiHist::ApiHist(const QString &apiurl, const QString &apikey):
    ApiRequest(),
    hist()
{
    qDebug() << "ApiHist::ApiHist()";

    url = apiurl;

    data.addQueryItem("k", QUrl::toPercentEncoding(apikey));
}

const QString ApiHist::urlext()
{
    return "hist";
}

void ApiHist::handleResponse()
{
    qDebug() << "ApiHist::handleResponse()";

    QList<QByteArray> lines = response.split('\n');

    if (lines.length() < 1)
        return;

    // skip response number
    for (int i = 1; i < lines.length(); i++) {
        QList<QByteArray> pieces = lines.at(i).split(',');

        if (pieces.length() != 6) {
            status = InvalidResponse;
            return;
        }

        hist.append(HistData(lines.at(i)));
    }

    //for(HistData &h : hist)
    //std::cout << h;
}

void ApiHist::done()
{
    qDebug() << "emit apihist done()";
    emit done(this);
}

const QString ApiHist::errorStr()
{
    if (reply->error() != QNetworkReply::NoError) {
        return reply->errorString();
    }
    return errorStrings[status];
}

const QList<ApiHist::HistData> ApiHist::allData()
{
    return hist;
}

ApiHist::HistData::HistData(const QString &s)
{
    QStringList l = s.split(',');
    if (l.length() != 6) return;

    id       = l[0];
    date     = l[1];
    url      = l[2];
    filename = l[3];
    views    = l[4].toInt();
    unknown  = l[5];
}

std::ostream &operator<<(std::ostream &s, const ApiHist::HistData &d)
{
    qDebug() << "id: "       << d.id;
    qDebug() << "date: "     << d.date;
    qDebug() << "url: "      << d.url;
    qDebug() << "filename: " << d.filename;
    qDebug() << "views: "    << d.views;
    qDebug() << "unknown: "  << d.unknown;
    return s;
}

