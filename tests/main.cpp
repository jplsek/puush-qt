#include <QtTest/QtTest>
#include <QSettings>

#include "settings.h"
#include "general.h"
#include "keybindings.h"

class Test: public QObject {
    Q_OBJECT
private slots:
    void radioSettings();
    void properKeybindings();
    //void fullscreenScreenshot();
    //void screenshotDoneOk(int, QString, QString);
    void removeScheme();
};

void Test::radioSettings() {
    Settings s;
    s.setRadioValue(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS);
    QVERIFY(s.radioValueIs(Settings::TRAY_CLICK_ACTION, Settings::OPEN_UPLOADS));
}

void Test::properKeybindings() {
    KeyBindings *kb = new KeyBindings();
    // just CTRL
    QVERIFY(!kb->isProperBinding(0, Qt::Key_Control));
    // just S
    QVERIFY(!kb->isProperBinding(0, Qt::Key_S));
    // SHIFT+CTRL hopefully this is portable...
    QVERIFY(!kb->isProperBinding(33554432, 16777249));
    // CTRL+S
    QVERIFY(kb->isProperBinding(67108864, Qt::Key_S));
    delete kb;
}

void Test::removeScheme() {
    General *general = new General();
    QVERIFY(QString("/home/foo") == general->removeScheme(QUrl("file:///home/foo")));
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
