#include "mypointwidget.h"

MyPointWidget::MyPointWidget(int index, const Point& point, QWidget *parent) : QWidget(parent)
{
    m_index = index;
    m_point = point;

    m_layout = new QHBoxLayout(this);

    QString x_str = QString("%1").arg(point.x);
    QString y_str = QString("%1").arg(point.y);
    m_x_input = new QLineEdit(x_str, this);
    m_y_input = new QLineEdit(y_str, this);
    m_remove_button = new QPushButton("-", this);

    QString s = QString("%1.").arg(index + 1);
    QLabel *index_label = new QLabel(s, this);
    QLabel *left_bracket = new QLabel("(", this);
    QLabel *semicolumn = new QLabel(";", this);
    QLabel *right_bracket = new QLabel(")", this);

    m_layout->addWidget(index_label);
    m_layout->addWidget(left_bracket);
    m_layout->addWidget(m_x_input);
    m_layout->addWidget(semicolumn);
    m_layout->addWidget(m_y_input);
    m_layout->addWidget(right_bracket);
    m_layout->addWidget(m_remove_button);

    connect(m_x_input, &QLineEdit::editingFinished, this, &MyPointWidget::on_x_input_changed);
    connect(m_y_input, &QLineEdit::editingFinished, this, &MyPointWidget::on_y_input_changed);
    connect(m_remove_button, SIGNAL(clicked()), this, SLOT(on_remove_button_clicked()));
}

void MyPointWidget::set_index(int index)
{
    m_index = index;
}

void MyPointWidget::set_point(const Point &point)
{
    m_point = point;
}

void MyPointWidget::set_set(Set set)
{
    m_set = set;
}

int MyPointWidget::index()
{
    return m_index;
}

Set MyPointWidget::set()
{
    return m_set;
}

Point MyPointWidget::point()
{
    return m_point;
}

void MyPointWidget::update_index()
{
    // XXX: AOAOA consider index will always be at pos 0
    QLabel *label = qobject_cast<QLabel*>(m_layout->itemAt(0)->widget());
    QString str = QString("%1.").arg(m_index + 1);
    label->setText(str);
}

bool MyPointWidget::is_input_valid()
{
    return is_valid_x_input && is_valid_y_input;
}

void MyPointWidget::on_x_input_changed()
{
    QString str = m_x_input->text();
    bool is_double = false;
    double value = str.toDouble(&is_double);

    if (is_double) {
        emit x_input_changed(m_index, m_set, value);
        is_valid_x_input = true;
    } else {
        emit invalid_input(m_index);
        is_valid_x_input = false;
    }
}

void MyPointWidget::on_y_input_changed()
{
    QString str = this->y_input->text();
    bool is_double = false;
    double value = str.toDouble(&is_double);

    if (is_double) {
        emit y_input_changed(m_index, m_set, value);
        is_valid_y_input = true;
    } else {
        emit invalid_input(m_index);
        is_valid_y_input = false;
    }
}

void MyPointWidget::on_remove_button_clicked()
{
    emit remove_button_clicked(m_index, m_set);

    delete this;
}
