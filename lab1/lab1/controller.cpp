#include "controller.h"

Controller::Controller(Model *model, View *view, QObject *parent) : QObject(parent)
{
    this->model = model;
    this->view = view;

    connect(this, SIGNAL(model_solve()), model, SLOT(solve_model()));
}

Controller::~Controller()
{
    delete this->model;
    delete this->view;
}

void Controller::on_solve_button_clicked()
{
    emit model_solve();
    this->view->draw_solution();
}
