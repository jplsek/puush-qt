#ifndef GENERAL_H
#define GENERAL_H

#include <QObject>
#include <QUrl>

class General : public QObject
{
    Q_OBJECT
public:
    explicit General(QObject *parent = nullptr);

    Q_INVOKABLE QString removeScheme(QUrl);

signals:

public slots:
};

#endif // GENERAL_H
