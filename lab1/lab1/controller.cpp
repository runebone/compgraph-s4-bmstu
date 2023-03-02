#include "controller.h"

Controller::Controller(Model *model, View *view, QObject *parent)
{
    m_model = model;
    m_view = view;
    m_history = new History;

    connect(view->m_view, SIGNAL(leftMouseClicked(QMouseEvent*)), this, SLOT(on_left_mouse_clicked(QMouseEvent*)));
    connect(view->m_view, SIGNAL(rightMouseClicked(QMouseEvent*)), this, SLOT(on_right_mouse_clicked(QMouseEvent*)));
    connect(view->m_view, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(on_key_pressed(QKeyEvent*)));

    connect(model, SIGNAL(updated()), view, SLOT());
}

Controller::~Controller()
{
    delete m_history;

    // XXX: In this program controller owns model and view
    delete m_model;
    delete m_view;
}

void Controller::on_solve_button_clicked()
{
    emit solve_button_clicked();
}

void Controller::on_add_button_1_clicked()
{
    emit add_button_1_clicked();
}

void Controller::on_add_button_2_clicked()
{
    emit add_button_2_clicked();
}

void Controller::on_load_from_file_button_clicked(QString)
{
    // TODO
}

void Controller::on_back_button_clicked()
{
    // TODO
}

void Controller::on_clear_screen_clicked()
{
    emit clear_screen_clicked();
}

void Controller::on_key_pressed(QKeyEvent *event)
{
    emit key_pressed(event);
}

void Controller::on_left_mouse_clicked(QMouseEvent *event)
{
    emit left_mouse_clicked(event);
}

void Controller::on_right_mouse_clicked(QMouseEvent *event)
{
    emit right_mouse_clicked(event);
}
