#include "apirequest.h"

#include <iostream>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QDataStream>

ApiRequest::ApiRequest():
    data(),
    qnr(nullptr),
    reply(nullptr),
    status(0),
    response()
{
    std::cout << "ApiRequest::ApiRequest() status == " << status << std::endl;
}

ApiRequest::~ApiRequest(){
    if(reply)
        delete reply;
    if(qnr)
        delete qnr;
}

void ApiRequest::start(){
    std::cout << "ApiRequest::start()" << std::endl;

    QNetworkAccessManager *nm = new QNetworkAccessManager();

    qnr = new QNetworkRequest(QUrl(PUUSH_API_URL_BASE + urlext()));
    qnr->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    reply = nm->post(*qnr, data.toString().toUtf8());
    QMetaObject::Connection c = connect(reply, SIGNAL(finished()), this, SLOT(readResponse()));
}

void ApiRequest::readResponse(){
    response = reply->readAll();

    std::cout << "R: " << response.toStdString() << std::endl;

    // this can only happen on a failed request.
    if(response.length() == 0){
        status = 1;
        done();
        return;
    }

    std::cout << "response == " << response.toStdString() << std::endl; // DEBUG

    if(response.length() < 4){ // assume we've got a number back.
        status = response.toInt();
    } else {
        status = 0;
    }

    std::cout << "status == " << status << std::endl; // DEBUG

    // anything that gets parsed to a 0 is the result of a successful command. (excluding an empty case that is handled above)
    if(status != 0){
        status = abs(status); // convert from negative to a positive non-zero error index
        std::cout << "status now " << status << std::endl;
        done();
        return;
    }

    handleResponse();
    done();
}


bool ApiRequest::failed(){
    return reply->error() != QNetworkReply::NoError || status != 0;
}
