#include "controller.h"

#include <QDebug>

Controller::Controller(Model *model, View *view, QObject *parent)
{
    m_model = model;
    m_view = view;
    m_history = new History;
    m_history->push(m_model->createMemento()); // Create starting point in history

    view->setModel(model);

    connect(view->getView(), SIGNAL(leftMouseClicked(QMouseEvent*)), this, SLOT(on_left_mouse_clicked(QMouseEvent*)));
    connect(view->getView(), SIGNAL(rightMouseClicked(QMouseEvent*)), this, SLOT(on_right_mouse_clicked(QMouseEvent*)));
    connect(view->getView(), SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(on_key_pressed(QKeyEvent*)));

    connect(model, SIGNAL(dbg_updated()), this, SLOT(on_model_updated()));
    connect(model, SIGNAL(updated(ModelData)), this, SLOT(on_model_updated()));
    connect(model, SIGNAL(updated(ModelData)), view, SLOT(on_model_updated(ModelData)));
    connect(model, SIGNAL(memento_restored(ModelData)), view, SLOT(on_model_updated(ModelData)));

    connect(this, SIGNAL(clear_screen_clicked()), view, SLOT(on_clear_screen_clicked()));
}

Controller::~Controller()
{
    delete m_history;

    // XXX: In this program controller owns model and view
    delete m_model;
    delete m_view;
}

void Controller::on_model_updated()
{
    ModelMemento memento = m_model->createMemento();
    m_history->push(memento);

    qDebug() << "Pushed to memento; History size:" << m_history->size();
}

void Controller::on_solve_button_clicked()
{
    err_t status = m_model->solve();

    // XXX
    if (isNotError(status)) {
        emit solve_button_clicked();
    } else {
        qDebug() << QString().fromStdString(getErrorDescription(status));
    }
}

void Controller::on_add_button_1_clicked()
{
    Point p = { 0.0, 0.0 };
    m_model->add_point(p, FIRST);
}

void Controller::on_add_button_2_clicked()
{
    Point p = { 0.0, 0.0 };
    m_model->add_point(p, SECOND);
}

#include <fstream>
#include <sstream>
void Controller::on_load_from_file_button_clicked(QString filename)
{
    // XXX
    std::ifstream file(filename.toStdString());
    std::string line;

    std::vector<Point> first_set_points;
    std::vector<Point> second_set_points;

    if (file.is_open())
    {
        Point p;
        int set;

        // Consider file has valid structure
        while (std::getline(file, line))
        {
            if (line[0] == '#') {
                continue;
            }

            std::stringstream ss(line);
            ss >> set >> p.x >> p.y;

            if (set == 1) {
                first_set_points.push_back(p);
            } else if (set == 2) {
                second_set_points.push_back(p);
            }
        }
        file.close();
    }

    m_model->replace_points(first_set_points, FIRST);
    m_model->replace_points(second_set_points, SECOND);
}

void Controller::on_back_button_clicked()
{
    if (m_history->size() > 1) {
        m_history->pop(); // Remove current state from history
        ModelMemento memento = m_history->back();
        m_model->restoreMemento(memento);
    } else {
        qDebug() << "Reached bottom of history stack!";
    }
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
