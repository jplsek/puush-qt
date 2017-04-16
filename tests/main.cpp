#include <QtTest/QtTest>
#include <QSettings>

#include "settings.h"

class Test: public QObject {
    Q_OBJECT
private slots:
    void radioSettings();
    //void fullscreenScreenshot();
    //void screenshotDoneOk(int, QString, QString);
};

void Test::radioSettings() {
    Settings s;
    s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS);
    QVERIFY(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS));
}

//void Test::fullscreenScreenshot() {
//    QString fileName = Window::getFileName();
//    QVERIFY(fileName != "");
//
//    Screenshot *ss = new Screenshot(fileName);
//    connect(ss, SIGNAL(finished(int, QString, QString)), this, SLOT(screenshotDoneOk(int, QString, QString)));
//    ss->fullScreen();
//}

//void Test::screenshotDoneOk(int returnCode, QString fileName, QString output) {
//   QVERIFY(returnCode == 0);
//   QVERIFY(fileName != "");
//}

QTEST_MAIN(Test)
#include "main.moc"
