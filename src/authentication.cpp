#include "authentication.h"

#include <QDebug>

#include "information.h"
#include "api/apiauth.h"

Authentication::Authentication(QObject *parent) : QObject(parent) {  }

void Authentication::submitLogin(QString email, QString password) {
    s.setValue(Settings::ACCOUNT_EMAIL, email);
    ApiAuth *api = new ApiAuth(email, password);
    QMetaObject::Connection r = connect(api, SIGNAL(done(ApiAuth *)), this, SLOT(authDone(ApiAuth *)));
    qDebug() << "submitInfo connect == " << r;
    api->start();
}

void Authentication::authDone(ApiAuth *req) {
    qDebug() << "authDone()";

    if(req->failed()){
        s.setValue(Settings::ACCOUNT_EMAIL, "");
        emit authMessage(req->errorStr());
        return;
    }

    emit authMessage("");

    s.setValue(Settings::ACCOUNT_API_KEY, req->apikey());

    emit authChange(isLoggedIn());

    //userData = req->allData();

    delete req;
}

void Authentication::logout(){
    s.setValue(Settings::ACCOUNT_EMAIL, "");
    s.setValue(Settings::ACCOUNT_API_KEY, "");
    emit authChange(isLoggedIn());
}

bool Authentication::isLoggedIn() {
    return s.value(Settings::ACCOUNT_API_KEY).toString() != "";
}
