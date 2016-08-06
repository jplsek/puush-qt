#ifndef UPLOAD_H
#define UPLOAD_H

#include <QString>
#include <QObject>
#include <QProcess>

class Upload : public QObject {
    Q_OBJECT

public:
    Upload(QString);

signals:
    int finished(int, QString);
    int started();

private slots:
    void uploadStarted();
    void uploadDone(int);

private:
    QProcess *uploadProcess;
};

#endif // UPLOAD_H
