#include "authentication.h"

#include <QDebug>

#include "information.h"
#include "api/apiauth.h"
#include "settings.h"

Authentication::Authentication(QObject *parent) : QObject(parent) {  }

void Authentication::submitLogin(QString email, QString password)
{
    s.setValue(Settings::ACCOUNT_EMAIL, email);
    ApiAuth *api = new ApiAuth(s.value(Settings::API_URL).toString(), email, password);
    QMetaObject::Connection r = connect(api, SIGNAL(done(ApiAuth *)), this, SLOT(authDone(ApiAuth *)));
    qDebug() << "submitInfo connect == " << r;
    api->start();
}

void Authentication::authDone(ApiAuth *req)
{
    qDebug() << "authDone()";

    if (req->failed()) {
        // we don't need to reset the email. There could be offline issues
        // which would reset the email, and having the email stored isn't really a problem.
        emit authMessage(req->errorStr());
        return;
    }

    emit authMessage("");

    s.setValue(Settings::ACCOUNT_API_KEY, req->apikey());
    // although the user can change the settings by hand,
    // it really doesn't affect the functionality of the client.
    s.setValue(Settings::ACCOUNT_PREMIUM, req->hasPremium());
    s.setValue(Settings::ACCOUNT_PREMIUM_EXPIRY, req->expiry());
    s.setValue(Settings::ACCOUNT_DISK_USAGE, req->diskUsage());

    emit authChange(isLoggedIn());

    //userData = req->allData();

    delete req;
}

void Authentication::logout()
{
    s.setValue(Settings::ACCOUNT_EMAIL, "");
    s.setValue(Settings::ACCOUNT_API_KEY, "");
    emit authChange(isLoggedIn());
}

bool Authentication::isLoggedIn()
{
    return s.value(Settings::ACCOUNT_API_KEY).toString() != "";
}

void Authentication::update()
{
    if (!isLoggedIn())
        return;

    ApiAuth *api = new ApiAuth(s.value(Settings::API_URL).toString(),
                               s.value(Settings::ACCOUNT_API_KEY).toString());
    QMetaObject::Connection r = connect(api, SIGNAL(done(ApiAuth *)), this, SLOT(authDone(ApiAuth *)));
    api->start();
}
