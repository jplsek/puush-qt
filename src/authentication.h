#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QObject>

#include "settings.h"
#include "api/apiauth.h"

class Authentication : public QObject
{
    Q_OBJECT
public:
    explicit Authentication(QObject *parent = 0);

    Q_INVOKABLE void submitLogin(QString, QString);
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool isLoggedIn();

private:
    Settings s;

signals:
    void authMessage(QString text);
    void authChange(bool loggedIn);

public slots:

private slots:
    void authDone(ApiAuth *);
};

#endif // AUTHENTICATION_H
