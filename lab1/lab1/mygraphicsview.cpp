#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent) {}

void MyGraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftMouseClicked(event);
    } else if (event->button() == Qt::RightButton) {
        emit rightMouseClicked(event);
    } else if (event->button() == Qt::MiddleButton) {
        emit middleMouseClicked(event);
    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event);
}
