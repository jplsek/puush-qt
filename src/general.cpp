#include "general.h"

General::General(QObject *parent) : QObject(parent) { }

QString General::removeScheme(QUrl url) {
    return url.toLocalFile();
}
