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
    void getHistory();
    void setDirty();

private:
    Settings s;
    // used for keeping a "cache" instead of constantly fetching the history
    bool dirty;
    QList<ApiHist::HistData> historyList;

signals:
    void historyDone(QList<ApiHist::HistData>);

public slots:

private slots:
    void getHistoryDone(ApiHist *);
};

#endif // HISTORY_H
