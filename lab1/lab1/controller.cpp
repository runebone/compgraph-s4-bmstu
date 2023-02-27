#include "controller.h"

Controller::Controller(Model *model, View *view, QObject *parent) : QObject(parent)
{
    this->model = model;
    this->view = view;

    this->model_caretaker = new History;

    connect(this, SIGNAL(model_solve()), model, SLOT(solve()));
    connect(this, SIGNAL(model_add_point1()), model, SLOT(add_point1()));
    connect(this, SIGNAL(model_add_point2()), model, SLOT(add_point2()));
    connect(this, SIGNAL(model_load_from_file(QString)), model, SLOT(load_from_file(QString)));

    connect(this->model, SIGNAL(updated()), this, SLOT(on_model_updated()));
    connect(this->model, SIGNAL(removed_point()), this, SLOT(on_model_updated()));
    connect(this->model, SIGNAL(point_edited_signal(int,Set,Point,Point)), this, SLOT(on_model_updated()));
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
        this->model_caretaker->push(this->model->createMemento());

        emit model_load_from_file(filename);
    }
}

void Controller::on_back_button_clicked()
{
    // meeeeeeeeeeeeeeeeeeeeeeh
    if (this->model_caretaker->size() > 1 && this->model_caretaker->take_second) {
        this->model_caretaker->pop();
        Model::Memento memento = this->model_caretaker->pop();
        this->model->restore(memento);
        this->model_caretaker->take_second = false;
    } else if (this->model_caretaker->size() > 0 && this->model_caretaker->take_second == false) {
        Model::Memento memento = this->model_caretaker->pop();
        this->model->restore(memento);
    }
}

void Controller::on_clear_screen_clicked()
{
    this->view->clear();
}

void Controller::on_model_updated()
{
    this->model_caretaker->push(this->model->createMemento());
    this->model_caretaker->take_second = true;
}
