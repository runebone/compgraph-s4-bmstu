#ifndef MYPOINTWIDGET_H
#define MYPOINTWIDGET_H

#include "point.h"
#include "set_enum.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class MyPointWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyPointWidget(size_t index, const Point &point, QWidget *parent = nullptr);

    void set_index(size_t index);
    void set_point(const Point &point);
    void set_set(Set set);

    size_t index();
    Set set();
    Point point();

    void update_index();
    bool is_input_valid();

private:
    size_t m_index;
    Point m_point;
    Set m_set = FIRST;

    QHBoxLayout *m_layout;
    QLineEdit *m_x_input;
    QLineEdit *m_y_input;
    QPushButton *m_remove_button;

    bool is_valid_x_input = true;
    bool is_valid_y_input = true;

public slots:
    void on_x_input_changed();
    void on_y_input_changed();
    void on_remove_button_clicked();

signals:
    void x_input_changed(size_t point_index, double value, Set set);
    void y_input_changed(size_t point_index, double value, Set set);
    void remove_button_clicked(size_t point_index, Set set);

    void invalid_input(size_t point_index);
};

#endif // MYPOINTWIDGET_H
