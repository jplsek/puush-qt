#include <QMainWindow>
#include <QRubberBand>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "transparentwindow.h"

TransparentWindow::TransparentWindow(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setParent(0); // Create TopLevel-Widget

    // TODO can't get this to work (it becomes not a window)
    //setAttribute(Qt::WA_NoSystemBackground, true);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    // Doing this for now... (the opacity also effects the rubber band)
    setProperty("windowOpacity", 0.5);

    // set size of window based on all screens
    int width = 0;
    int height = 0;

    for (int i = 0; i < QApplication::desktop()->screenCount(); ++i) {
        width += QApplication::desktop()->screenGeometry(i).width();
        height += QApplication::desktop()->screenGeometry(i).height();
    }

    setGeometry(QRect(0, 0, width, height));

    setCursor(Qt::CrossCursor);

    show();

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}


void TransparentWindow::mousePressEvent(QMouseEvent *event) {
    origin = event->pos();

    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void TransparentWindow::mouseMoveEvent(QMouseEvent *event) {
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void TransparentWindow::mouseReleaseEvent(QMouseEvent *event) {
    rubberBand->hide();
    this->hide();
    finished(rubberBand->geometry(), origin, event->pos());
}
