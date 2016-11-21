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
    quality = s.value("quality").toString();
}

/**
 * Take a selected area screenshot
 * @brief Screenshot::selectArea
 */
void Screenshot::selectArea() {
    tw = new TransparentWindow();
    connect(tw, SIGNAL(finished(QRect, QPoint, QPoint)), this, SLOT(selectionDone(QRect)));
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
    QPixmap screenshot = desktop();

    if (screenshot.width() == 0 && screenshot.height() == 0)
        return;

    save(screenshot);
}

void Screenshot::save(QPixmap screenshot) {
    // TODO we can now tell whether to use PNG or JPEG with save(filename, filetype) (our advanced settings).
    // By default, it uses the suffix of the file name.
    if (!screenshot.save(fn)) {
        finished(-1, fn, tr("The image could not be saved to ") + fn + tr("!"));
        return;
    }

    qDebug() << fn;

    finished(0, fn, "");
}

QPixmap Screenshot::desktop() {
    // TODO fix deprecated error
    // We can use our screen() to loop through them, and somehow combine the qpixmaps into 1 (QDesktopWidget::screenCount()).
    // We may also have to take into account the screen locations.
    QDesktopWidget *desktop = QApplication::desktop();
    return QPixmap::grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height());
}

QPixmap Screenshot::screen(int i) {
    QScreen *screen = QGuiApplication::screens()[i];

    if (!screen) {
        finished(-1, fn, tr("Can't find a screen!"));
        return QPixmap(0, 0);
    }

    return screen->grabWindow(0);
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

    #else

    screenshotProcess = new QProcess();
    screenshotProcess->start("scrot", QStringList() << "-b" << "-q" << quality << "-u" << fn);

    connect(screenshotProcess, SIGNAL(finished(int)), this, SLOT(screenshotDone(int)));

    #endif
}

void Screenshot::screenshotDone(int returnCode) {
    QString output = screenshotProcess->readAll();
    screenshotProcess->deleteLater();
    finished(returnCode, fn, output);
}
