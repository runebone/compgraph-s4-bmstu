#include "mypointwidget.h"

MyPointWidget::MyPointWidget(int index, const Point& point, QWidget *parent) : QWidget(parent)
{
    this->index = index;
    this->point = point;

    this->layout = new QHBoxLayout(this);

    QString x_str = QString("%1").arg(point.x);
    QString y_str = QString("%1").arg(point.y);
    this->x_input = new QLineEdit(x_str, this);
    this->y_input = new QLineEdit(y_str, this);
    this->remove_btn = new QPushButton("-", this);

    QString s = QString("%1.").arg(index + 1);
    QLabel *index_label = new QLabel(s, this);
    QLabel *left_bracket = new QLabel("(", this);
    QLabel *semicolumn = new QLabel(";", this);
    QLabel *right_bracket = new QLabel(")", this);

    this->layout->addWidget(index_label);
    this->layout->addWidget(left_bracket);
    this->layout->addWidget(this->x_input);
    this->layout->addWidget(semicolumn);
    this->layout->addWidget(this->y_input);
    this->layout->addWidget(right_bracket);
    this->layout->addWidget(this->remove_btn);

    connect(this->x_input, &QLineEdit::editingFinished, this, &MyPointWidget::on_x_input_changed);
    connect(this->y_input, &QLineEdit::editingFinished, this, &MyPointWidget::on_y_input_changed);
    connect(this->remove_btn, SIGNAL(clicked()), this, SLOT(on_remove_button_clicked()));
}

void MyPointWidget::on_x_input_changed()
{
    // emit this->x_input_changed(this->index, str);
}

void MyPointWidget::on_y_input_changed()
{
    // emit this->y_input_changed(this->index, str);
}

void MyPointWidget::on_remove_button_clicked()
{
    emit this->remove_button_clicked(this->index);
}
