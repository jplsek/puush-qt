#include "history.h"

#include <QDebug>

#include "api/apihist.h"

History::History(QObject *parent) : QObject(parent) {  }

void History::getHistory(){
    qDebug() << "check login";

    if(s.value(Settings::ACCOUNT_API_KEY).toString() == "")
        return;

    qDebug() << "getting history";

    ApiHist *api = new ApiHist(s.value(Settings::ACCOUNT_API_KEY).toString());
    QMetaObject::Connection r = connect(api, SIGNAL(done(ApiHist *)), this, SLOT(getHistoryDone(ApiHist *)));
    api->start();
}

void History::getHistoryDone(ApiHist *results){
    results->allData();
    // add rows to a thing with each result. Note that getting a thumbnail probably requires another api request per image
}
