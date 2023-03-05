#include "view.h"

#include <QGraphicsObject>
#include <QGraphicsItemGroup>

#define FILL_COLOR QColor("#fbb829")

#define POINT_SIZE 16.0
#define LINES_WIDTH 4.0
#define MAX_PEN_WIDTH 100.0

View::View(MyGraphicsView *graphicsView, QObject *parent)
{
    m_view = graphicsView;

    m_scene = new MyGraphicsScene(this);
    m_view->setScene(m_scene);
}

void View::setModel(Model *model)
{
    m_model = model;
}

MyGraphicsView *View::getView() const
{
    return m_view;
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

void handleZoom(QGraphicsItem* item, qreal scale_factor) {
    if (item->type() == QGraphicsItemGroup::Type) {
        QGraphicsItemGroup* groupItem = qgraphicsitem_cast<QGraphicsItemGroup*>(item);

        for (QGraphicsItem *child: groupItem->childItems()) {
            if (child->type() == QGraphicsLineItem::Type) {
                QGraphicsLineItem* line = qgraphicsitem_cast<QGraphicsLineItem*>(child);

                QPen pen = line->pen();
                if (scale_factor < MAX_PEN_WIDTH) {
                    pen.setWidthF(LINES_WIDTH * scale_factor);
                }
                line->setPen(pen);
            }
        }
    }
}


namespace draw {
// void polygon(QGraphicsScene *scene, const std::vector<Point> &vertices, QColor color, bool fill = false) {
    // QPolygonF polygon;

    // for (Point p: vertices) {
        // polygon << QPointF(p.x, -p.y); // XXX: -y
    // }

    // QGraphicsPolygonItem *figure = new QGraphicsPolygonItem(polygon);
    // figure->setPen(QPen(color, LINES_WIDTH));

    // if (fill) {
        // figure->setBrush(FILL_COLOR);
    // }

    // scene->addItem(figure);
// }
}

namespace resize {
void scene(QGraphicsScene *scene, QGraphicsView *view) {
    qreal scale_factor = 1.0 / view->transform().m11();
    for (QGraphicsItem *item : scene->items()) {
        handleZoom(item, scale_factor);
    }
}

void fit(QGraphicsScene *scene, QGraphicsView *view) {
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    resize::scene(scene, view);
}
}

void View::on_model_updated(ModelData md)
{
    QGraphicsItemGroup *groupItem = new QGraphicsItemGroup();

    for (auto line: md.lines) {
        qreal x1 = static_cast<qreal>(md.vertices.at(line.a).a);
        qreal y1 = static_cast<qreal>(md.vertices.at(line.a).b);
        qreal x2 = static_cast<qreal>(md.vertices.at(line.b).a);
        qreal y2 = static_cast<qreal>(md.vertices.at(line.b).b);
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(x1, y1, x2, y2);

        groupItem->addToGroup(lineItem);
    }

    m_scene->addItem(groupItem);

    QRectF r = m_scene->itemsBoundingRect();
    m_view->fitInView(r, Qt::KeepAspectRatio);

    resize::scene(m_scene, m_view);
    resize::fit(m_scene, m_view); // XXX

    qDebug() << "lets goooooooooo";
}

void View::on_mouse_clicked(QMouseEvent *event)
{
    // QPoint viewPos = m_view->mapFromGlobal(QCursor::pos());
    // QPointF scenePos = m_view->mapToScene(viewPos);
}

void View::on_key_pressed(QKeyEvent *event)
{
    // if (event->key() == Qt::Key_Delete) {
        // util::delete_selected_points(m_scene, m_model);
    // } else if (event->key() == Qt::Key_F5 || event->key() == Qt::Key_R) {
        // m_model->dbg_emit_updated();
    // } else if (event->key() == Qt::Key_Equal) {
        // resize::fit(m_scene, m_view);
    // } else if (event->key() == Qt::Key_I) {
        // const double sf = 1.2; // XXX meh hardcode

        // QTransform matrix = m_view->transform();
        // matrix.scale(sf, sf);
        // m_view->setTransform(matrix);

        // resize::scene(m_scene, m_view);
    // } else if (event->key() == Qt::Key_O) {
        // const double sf = 0.8; // XXX

        // QTransform matrix = m_view->transform();
        // matrix.scale(sf, sf);
        // m_view->setTransform(matrix);

        // resize::scene(m_scene, m_view);
    // }
}
