#ifndef APIHIST_H
#define APIHIST_H

#include "apirequest.h"
#include <QObject>
#include <QString>

class ApiHist : public ApiRequest
{
    Q_OBJECT
public:
    struct HistData {
        HistData(const QString &s);
        QString id;
        QString date;
        QString url;
        QString filename;
        int views;
        QString unknown;
    };

    enum Error {
        NoError = 0,
        Failure = 1,
        InvalidResponse = 2,
    };

    const static QString errorStrings[];

public:
    ApiHist(const QString &apikey);

    const QString urlext();

    const QString errorStr();

    const QList<HistData> allData();

    static const QString expiryReadableString(int expiry);

protected:
    void done();

signals:
    void done(ApiHist *);

protected:
    void handleResponse();

private:
    QList<HistData> hist;
};

std::ostream &operator<<(std::ostream &, const ApiHist::HistData &);

#endif // APIHIST_H
