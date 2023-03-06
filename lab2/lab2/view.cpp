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

    m_transform = QTransform();
    m_transform.setMatrix(1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0);
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
void model(QGraphicsScene *scene, ModelData md) {
    QGraphicsItemGroup *groupItem = new QGraphicsItemGroup();

    for (auto line: md.lines) {
        qreal x1 = static_cast<qreal>(md.vertices.at(line.a).a);
        qreal y1 = static_cast<qreal>(md.vertices.at(line.a).b);
        qreal x2 = static_cast<qreal>(md.vertices.at(line.b).a);
        qreal y2 = static_cast<qreal>(md.vertices.at(line.b).b);
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(x1, y1, x2, y2);

        groupItem->addToGroup(lineItem);
    }

    scene->addItem(groupItem);
}
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

namespace transform {
void apply(QGraphicsScene *scene, QTransform transform) {
    for (QGraphicsItem *item : scene->items()) {
        QTransform t_current = item->transform();
        QTransform t_new = transform * t_current;
        item->setTransform(t_new);
    }
}

QTransform get_translation(qreal dx, qreal dy) {
    QTransform matrix;
    matrix.setMatrix(
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                dx, dy, 1.0
                );
    return matrix;
}

QTransform get_scale(qreal sx, qreal sy) {
    QTransform matrix;
    matrix.setMatrix(
                sx, 0.0, 0.0,
                0.0, sy, 0.0,
                0.0, 0.0, 1.0
                );
    return matrix;
}

QTransform get_rotation(qreal degrees) {
    QTransform matrix;
    qreal theta = degrees * M_PI / 180;
    matrix.setMatrix(
                cos(theta), -sin(theta), 0.0,
                sin(theta), cos(theta), 0.0,
                0.0, 0.0, 1.0
                );
    return matrix;
}
}

void View::on_model_updated(ModelData md)
{
    draw::model(m_scene, md);

    QRectF r = m_scene->itemsBoundingRect();
    m_view->fitInView(r, Qt::KeepAspectRatio);

    resize::scene(m_scene, m_view);
    resize::fit(m_scene, m_view); // XXX

    qDebug() << "model updated";
}

void View::on_mouse_clicked(QMouseEvent *event)
{
    // QPoint viewPos = m_view->mapFromGlobal(QCursor::pos());
    // QPointF scenePos = m_view->mapToScene(viewPos);
}

#define TRANSLATION_SPEED 1.0
#define ANGLE 5.0
#define SCALE 0.2
void View::on_key_pressed(QKeyEvent *event)
{
    if (event->key() == Qt::Key_I) {
        qreal sf = 1.0 + SCALE;
        QTransform matrix = transform::get_scale(sf, sf);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_O) {
        qreal sf = 1.0 - SCALE;
        QTransform matrix = transform::get_scale(sf, sf);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_H) {
        qreal dx = TRANSLATION_SPEED;
        qreal dy = 0.0;
        QTransform matrix = transform::get_translation(dx, dy);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_J) {
        qreal dx = 0.0;
        qreal dy = TRANSLATION_SPEED;
        QTransform matrix = transform::get_translation(dx, dy);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_K) {
        qreal dx = 0.0;
        qreal dy = -TRANSLATION_SPEED;
        QTransform matrix = transform::get_translation(dx, dy);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_L) {
        qreal dx = -TRANSLATION_SPEED;
        qreal dy = 0.0;
        QTransform matrix = transform::get_translation(dx, dy);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_Q) {
        QTransform matrix = transform::get_rotation(ANGLE);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_E) {
        QTransform matrix = transform::get_rotation(-ANGLE);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_A) {
        qreal sf = 1.0 + SCALE;
        QTransform matrix = transform::get_scale(sf, 1.0);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_D) {
        qreal sf = 1.0 - SCALE;
        QTransform matrix = transform::get_scale(sf, 1.0);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_W) {
        qreal sf = 1.0 - SCALE;
        QTransform matrix = transform::get_scale(1.0, sf);
        transform::apply(m_scene, matrix);
    } else if (event->key() == Qt::Key_S) {
        qreal sf = 1.0 + SCALE;
        QTransform matrix = transform::get_scale(1.0, sf);
        transform::apply(m_scene, matrix);
    }
}

// TODO: QPointF center = view.mapToScene(view.viewport()->rect().center());
