#include <QtGui>
#include <QProcess>
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include <QWindow>
#include <QDebug>
#include <QRect>
#include <QDesktopWidget>
#include <QRubberBand>

#if defined(Q_OS_WIN)
#include "windows.h"
#endif

#include "screenshot.h"
#include "transparentwindow.h"

Screenshot::Screenshot(QString fileName) {
    fn = fileName;
    quality = s.value(Settings::IMAGE_QUALITY).toString();
}

/**
 * Take a selected area screenshot
 * @brief Screenshot::selectArea
 */
void Screenshot::selectArea() {
    // even though the "transparent window" is "cross platform" some desktop environments don't allow windows to cover their panels, so we will just use scrot
    #if defined(Q_OS_LINUX)

    screenshotProcess = new QProcess();
    screenshotProcess->start("scrot", QStringList() << "-b" << "-q" << quality << "-s" << fn);

    connect(screenshotProcess, SIGNAL(finished(int)), this, SLOT(screenshotDone(int)));

    #else

    tw = new TransparentWindow();
    connect(tw, SIGNAL(finished(QRect, QPoint, QPoint)), this, SLOT(selectionDone(QRect)));

    #endif
}

void Screenshot::selectionDone(QRect selection) {
    delete(tw);
    croppedScreenshot(selection);
}

/**
 * Take a cropped screenshot
 * @brief Screenshot::croppedScreenshot
 * @param selection
 */
void Screenshot::croppedScreenshot(QRect selection) {
    QPixmap screenshot = desktop();
    screenshot = screenshot.copy(selection);

    save(screenshot);
}

/**
 * Take a fullscreen screenshot
 * @brief Screenshot::fullScreen
 */
void Screenshot::fullScreen() {
    // TODO check settings to take a screenshot a screen or the whole desktop

    // wait 1 second before taking a screenshot because of desktop animations, etc
    QTimer::singleShot(1000, this, SLOT(fullScreenAfterTimer()));
}

void Screenshot::fullScreenAfterTimer() {
    // if this is still 0, 0 by the end, we just take a screenshot of the whole desktop
    QPixmap screenshot(0, 0);
    QString message = "";

    if (s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::ALL_DESKTOPS)) {
        screenshot = desktop();
    } else if (s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::PRIMARY_DESKTOP)) {
        screenshot = primaryScreen();
    } else if (s.radioValueIs(Settings::FULLSCREEN_CAPTURE_MODE, Settings::CURRENT_DESKTOP)) {
        // TODO test that this works
        // find out where the mouse is, and base it from that
        QDesktopWidget *desktop = QApplication::desktop();
        QPoint p = desktop->mapFromGlobal(QCursor::pos());

        for (QScreen *s : QGuiApplication::screens()) {
            QRect r = s->geometry();
            if (r.contains(p)) {
                screenshot = screen(s);
                break;
            }
        }

        if (screenshot.width() == 0 && screenshot.height() == 0)
            message = tr("Screen not found within cursor!");
    } else {
        message = tr("Option not recognized!");
    }

    if (screenshot.width() == 0 && screenshot.height() == 0) {
        message = message + tr("\nTaking a screenshot of the whole desktop instead...");
        qDebug() << message;
        screenshot = desktop();
    }

    save(screenshot, message);
}

void Screenshot::save(QPixmap screenshot, QString message) {
    // TODO we can now tell whether to use PNG or JPEG with save(filename, filetype) (our advanced settings).
    // By default, it uses the suffix of the file name.
    if (!screenshot.save(fn)) {
        finished(-1, fn, tr("The image could not be saved to ") + fn + tr("!"));
        return;
    }

    qDebug() << fn;

    finished(0, fn, message);
}

QPixmap Screenshot::desktop() {
    // TODO fix deprecated error
    // We can use our screen() to loop through them, and somehow combine the qpixmaps into 1 (QDesktopWidget::screenCount()).
    // We may also have to take into account the screen locations.
    QDesktopWidget *desktop = QApplication::desktop();
    return QPixmap::grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height());
}

QPixmap Screenshot::primaryScreen() {
    QScreen *screen = QApplication::primaryScreen();
    return screen->grabWindow(0);
}

QPixmap Screenshot::screen(QScreen *screen) {
    if (!screen) {
        finished(-1, fn, tr("Can't find a screen!"));
        return QPixmap(0, 0);
    }

    return screen->grabWindow(0);
}

QPixmap Screenshot::screen(int i) {
    QScreen *s = QGuiApplication::screens()[i];
    return screen(s);
}

/**
 * Take a screenshot of the active window.
 * There is no support to do this with QT, so here we go.
 * ATM we'll be lazy, and use "scrot" for Linux users.
 * @brief Screenshot::activeWindow
 */
void Screenshot::activeWindow() {
    #if defined(Q_OS_WIN)

    HWND hwnd = GetForegroundWindow();
    RECT r;
    GetWindowRect(hwnd, &r);

    QRect qr(r.left, r.top, r.right - r.left, r.bottom - r.top);
    croppedScreenshot(qr);

    #elif defined(Q_OS_LINUX)

    screenshotProcess = new QProcess();
    screenshotProcess->start("scrot", QStringList() << "-b" << "-q" << quality << "-u" << fn);

    connect(screenshotProcess, SIGNAL(finished(int)), this, SLOT(screenshotDone(int)));

    #else

    finished(-1, fn, tr("Your operating system is not supported!"));

    #endif
}

void Screenshot::screenshotDone(int returnCode) {
    QString output = screenshotProcess->readAllStandardOutput();
    QString error = screenshotProcess->readAllStandardError();
    QString all = output + error;
    screenshotProcess->deleteLater();
    finished(returnCode, fn, all);
}
