#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent) {}

void MyGraphicsView::mousePressEvent(QMouseEvent *event) {
    emit mouseClicked(event);
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event);
}
