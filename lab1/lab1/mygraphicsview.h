#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void leftMouseClicked(QMouseEvent *event);
    void rightMouseClicked(QMouseEvent *event);

    void keyPressed(QKeyEvent *event);
};

#endif // MYGRAPHICSVIEW_H
