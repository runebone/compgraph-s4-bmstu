#ifndef MYPOINTWIDGET_H
#define MYPOINTWIDGET_H

#include "point.h"
#include "set.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class MyPointWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyPointWidget(int index, const Point &point, QWidget *parent = nullptr);

    int index;
    Point point;
    Set set = FIRST;

    QHBoxLayout *layout;
    QLineEdit *x_input;
    QLineEdit *y_input;
    QPushButton *remove_btn;

    void update_index();

public slots:
    void on_x_input_changed();
    void on_y_input_changed();
    void on_remove_button_clicked();

signals:
    void x_input_changed(int point_index, Set set, double value);
    void y_input_changed(int point_index, Set set, double value);
    void remove_button_clicked(int point_index, Set set);
    void invalid_input(int point_index);
};

#endif // MYPOINTWIDGET_H
