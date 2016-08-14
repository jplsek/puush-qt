#include "apihist.h"

#include <iostream>
#include <QByteArray>
#include <QDataStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QString>

const QString ApiHist::errorStrings[] = {
    [ApiHist::Error::NoError] = "No Error",
    [ApiHist::Error::Failure] = "Request Failed",
    [ApiHist::Error::InvalidResponse] = "Invalid response from puush",
};

ApiHist::ApiHist(const QString &apikey):
    ApiRequest(),
    hist()
{
    data.addQueryItem("k", QUrl::toPercentEncoding(apikey));
}

const QString ApiHist::urlext(){
    return "hist";
}

void ApiHist::handleResponse(){
    std::cout << "ApiHist::handleResponse()" << std::endl;

    QList<QByteArray> pieces = response.split(',');
    if(pieces.length() != 6){
        status = InvalidResponse;
        return;
    }

    // for each line
    //  hist.add(HistData(line));

    for(HistData &h : hist)
        std::cout << h;
}

void ApiHist::done(){
    emit done(this);
}

const QString ApiHist::errorStr(){
    if(reply->error() != QNetworkReply::NoError){
        return reply->errorString();
    }
    return errorStrings[status];
}

const QList<ApiHist::HistData> ApiHist::allData(){
    return hist;
}

ApiHist::HistData::HistData(const QString &s){
    QStringList l = s.split(',');
    if(l.length() != 6) return;
    
    id       = l[0];
    date     = l[1];
    url      = l[2];
    filename = l[3];
    views    = l[4].toInt();
    unknown  = l[5];
}

std::ostream &operator<<(std::ostream &s, const ApiHist::HistData &d){
    std::cout << "id: "       << d.id       << std::endl;
    std::cout << "date: "     << d.date     << std::endl;
    std::cout << "url: "      << d.url      << std::endl;
    std::cout << "filename: " << d.filename << std::endl;
    std::cout << "views: "    << d.views    << std::endl;
    std::cout << "unknown: "  << d.unknown  << std::endl;
    return s;
}

