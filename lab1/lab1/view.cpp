#include "view.h"

#include <QWidget>

View::View(MyGraphicsView *graphicsView, QScrollArea *leftScrollArea, QScrollArea *rightScrollArea, QLabel *infoLabel, QObject *parent)
{
    m_view = graphicsView;
    m_infoLabel = infoLabel;

    m_scene = new MyGraphicsScene;
    m_view->setScene(m_scene);

    m_leftLayout = new QVBoxLayout(leftScrollArea);
    m_rightLayout = new QVBoxLayout(rightScrollArea);

    // Binding layouts to ScrollAreas
    QWidget *lWidget = new QWidget(leftScrollArea);
    QWidget *rWidget = new QWidget(rightScrollArea);
    lWwidget->setLayout(m_leftLayout);
    rWwidget->setLayout(m_rightLayout);
    leftScrollArea->setWidget(lWidget);
    rightScrollArea->setWidget(rWidget);
}

View::~View()
{
    delete m_scene;
    delete m_leftLayout;
    delete m_rightLayout;
}
