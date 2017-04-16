#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>

#include "settings.h"
#include "api/apihist.h"

class History : public QObject
{
    Q_OBJECT
public:
    explicit History(QObject *parent = 0);

    Q_INVOKABLE void getHistory();

private:
    Settings s;

signals:

public slots:

private slots:
    void getHistoryDone(ApiHist *);
};

#endif // HISTORY_H
