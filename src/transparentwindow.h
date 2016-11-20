#ifndef TRANSPARENTWINDOW_H
#define TRANSPARENTWINDOW_H

#include <QDialog>
#include <QRubberBand>

class TransparentWindow : public QDialog {
    Q_OBJECT

public:
    explicit TransparentWindow(QWidget *parent = 0);

signals:
    int finished(QRect, QPoint, QPoint);

private:
    QRubberBand *rubberBand;
    QPoint origin;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
};

#endif // TRANSPARENTWINDOW_H
