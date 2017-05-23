#include "apirequest.h"

#include <iostream>

#include <QNetworkRequest>
#include <QNetworkReply>

ApiRequest::ApiRequest():
    data(),
    qnr(nullptr),
    reply(nullptr),
    status(0),
    response()
{
    qDebug() << "ApiRequest::ApiRequest() generate status == " << status;
}

ApiRequest::~ApiRequest()
{
    if (reply)
        delete reply;
    if (qnr)
        delete qnr;
}

void ApiRequest::start()
{
    qDebug() << "ApiRequest::start()";

    QNetworkAccessManager *nm = new QNetworkAccessManager();

    qnr = new QNetworkRequest(url + urlext());
    qnr->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    reply = nm->post(*qnr, data.toString().toUtf8());
    connect(reply, SIGNAL(finished()), this, SLOT(readResponse()));
}

void ApiRequest::readResponse()
{
    response = reply->readAll();

    qDebug() << "R: " << response;

    // this can only happen on a failed request.
    if (response.length() == 0) {
        status = 1;
        done();
        return;
    }

    if (response.length() < 4) { // assume we've got a number back.
        status = response.toInt();
    } else {
        status = 0;
    }

    qDebug() << "status == " << status;

    // anything that gets parsed to a 0 is the result of a successful command. (excluding an empty case that is handled above)
    if (status != 0) {
        status = abs(status); // convert from negative to a positive non-zero error index
        qDebug() << "status now " << status;
        done();
        return;
    }

    handleResponse();
    done();
}


bool ApiRequest::failed()
{
    return reply->error() != QNetworkReply::NoError || status != 0;
}
