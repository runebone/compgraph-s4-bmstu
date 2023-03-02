#include "controller.h"

Controller::Controller(Model *model, View *view, QObject *parent)
{
    m_model = model;
    m_view = view;
    m_history = new History;

    connect(view->m_view, SIGNAL(leftMouseClicked(QMouseEvent*)), this, SLOT());
    connect(view->m_view, SIGNAL(rightMouseClicked(QMouseEvent*)), this, SLOT());
    connect(view->m_view, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT());

    connect(model, SIGNAL(updated()), view, SLOT());
}

Controller::~Controller()
{
    delete m_history;

    // XXX: In this program controller owns model and view
    delete m_model;
    delete m_view;
}
