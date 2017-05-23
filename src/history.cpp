#include "history.h"

#include <QDebug>

#include "api/apihist.h"

History::History(QObject *parent) : QObject(parent)
{
    dirty = true;
}

void History::getHistory()
{
    if (s.value(Settings::ACCOUNT_API_KEY).toString() == "" || !dirty)
        return;

    qDebug() << "getting history";

    ApiHist *api = new ApiHist(s.value(Settings::API_URL).toString(),
                               s.value(Settings::ACCOUNT_API_KEY).toString());
    connect(api, SIGNAL(done(ApiHist *)), this, SLOT(getHistoryDone(ApiHist *)));
    api->start();
}

void History::getHistoryDone(ApiHist *results)
{
    historyList = results->allData();

    // set dirty to false only in a valid response. Keep dirty true when there
    // is an error and such so that the user can keep requesting. Like if there
    // is no internet when the user requests, then it would still be dirty, so
    // the next time they go online, they can grab the latest history.
    // The only odd part is if the user is new and have no screenshots available
    // yet...
    if (historyList.size() != 0)
        dirty = false;

    qDebug() << "emit history historyDone()";
    emit historyDone(historyList);
}

void History::setDirty()
{
    dirty = true;
}
