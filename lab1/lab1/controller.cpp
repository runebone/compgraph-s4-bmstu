#include "controller.h"

Controller::Controller(Model *model, View *view, QObject *parent) : QObject(parent)
{
    this->model = model;
    this->view = view;

    connect(this, SIGNAL(model_solve()), model, SLOT(solve()));
    connect(this, SIGNAL(model_add_point1()), model, SLOT(add_point1()));
    connect(this, SIGNAL(model_add_point2()), model, SLOT(add_point2()));
    connect(this, SIGNAL(model_load_from_file(QString)), model, SLOT(load_from_file(QString)));
    connect(this, SIGNAL(model_delete_all()), model, SLOT(delete_all_points()));
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

void Controller::on_add1_button_clicked()
{
    emit model_add_point1();
}

void Controller::on_add2_button_clicked()
{
    emit model_add_point2();
}

void Controller::on_load_from_file_button_clicked(QString filename)
{
    // meh
    if (!filename.isEmpty())
    {
        emit model_load_from_file(filename);
    }
}

void Controller::on_delete_all_button_clicked()
{
    emit model_delete_all();
}
