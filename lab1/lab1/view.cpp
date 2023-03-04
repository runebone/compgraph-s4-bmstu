#include "view.h"

#include <QWidget>
#include <QGraphicsObject>

View::View(MyGraphicsView *graphicsView, QScrollArea *leftScrollArea, QScrollArea *rightScrollArea, QLabel *infoLabel, QObject *parent)
{
    m_view = graphicsView;
    m_infoLabel = infoLabel;

    m_scene = new MyGraphicsScene(this);
    m_view->setScene(m_scene);

    m_leftLayout = new QVBoxLayout(leftScrollArea);
    m_rightLayout = new QVBoxLayout(rightScrollArea);

    // Binding layouts to ScrollAreas
    QWidget *lWidget = new QWidget(leftScrollArea);
    QWidget *rWidget = new QWidget(rightScrollArea);
    lWidget->setLayout(m_leftLayout);
    rWidget->setLayout(m_rightLayout);
    leftScrollArea->setWidget(lWidget);
    rightScrollArea->setWidget(rWidget);
}

void View::setModel(Model *model)
{
    m_model = model;
}

MyGraphicsView *View::getView() const
{
    return m_view;
}

void clearLayout(QLayout *layout) {
    QLayoutItem* item;
    QWidget* widget;

    while ((item = layout->takeAt(0)) != nullptr) {
        widget = item->widget();
        if (widget != nullptr) {
            delete widget;
        }
        delete item;
    }
}

void View::addMyPointWidgetToLayout(QLayout *layout, MyPointWidget *mpw) {
    layout->addWidget(mpw);

    connect(mpw, SIGNAL(x_input_changed(size_t,double,Set)), this, SLOT(on_point_x_changed(size_t,double,Set)));
    connect(mpw, SIGNAL(y_input_changed(size_t,double,Set)), this, SLOT(on_point_y_changed(size_t,double,Set)));
    connect(mpw, SIGNAL(remove_button_clicked(size_t,Set)), this, SLOT(on_point_remove_clicked(size_t,Set)));

    if (mpw->set() == FIRST) {
        connect(mpw, SIGNAL(invalid_input(size_t)), this, SLOT(on_invalid_input_left(size_t)));
    } else if (mpw->set() == SECOND) {
        connect(mpw, SIGNAL(invalid_input(size_t)), this, SLOT(on_invalid_input_right(size_t)));
    }
}

void View::addPointsToLayout(QLayout *layout, std::vector<Point> &points, Set set) {
    int i = 0;
    for (Point p: points) {
        MyPointWidget *mpw = new MyPointWidget(i, p);
        mpw->set_set(set);
        addMyPointWidgetToLayout(layout, mpw);
        i++;
    }
}

void clearScene(QGraphicsScene *scene) {
    QList<QGraphicsItem*> items = scene->items();

    for (auto item : items) {
        QGraphicsObject* object = dynamic_cast<QGraphicsObject*>(item);
        if (object) {
            delete object;
        }
    }

    scene->clear();
}

void View::on_model_updated(ModelData md)
{
    clearLayout(m_leftLayout);
    clearLayout(m_rightLayout);

    addPointsToLayout(m_leftLayout, md.firstSetPoints, FIRST);
    addPointsToLayout(m_rightLayout, md.secondSetPoints, SECOND);

    clearScene(m_scene);
    // TODO
}

void View::on_clear_screen_clicked()
{
    m_model->dbg_emit_updated();
}

void View::on_point_x_changed(size_t index, double value, Set set)
{
    Point p = m_model->get_point(index, set);
    p.x = value;
    m_model->update_point(index, p, set);
}

void View::on_point_y_changed(size_t index, double value, Set set)
{
    Point p = m_model->get_point(index, set);
    p.y = value;
    m_model->update_point(index, p, set);
}

void View::on_point_remove_clicked(size_t index, Set set)
{
    m_model->remove_point(index, set);
}

void View::on_invalid_input_left(size_t index)
{
    // TODO
}

void View::on_invalid_input_right(size_t index)
{
    // TODO
}
