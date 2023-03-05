#include "controller.h"

#include <QDebug>

Controller::Controller(Model *model, View *view, QObject *parent)
{
    m_model = model;
    m_view = view;
    // m_history = new History;
    // m_history->push(m_model->createMemento()); // Create starting point in history

    view->setModel(model);

    connect(view->getView(), SIGNAL(mouseClicked(QMouseEvent*)), this, SLOT(on_mouse_clicked(QMouseEvent*)));
    connect(view->getView(), SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(on_key_pressed(QKeyEvent*)));

    connect(this, SIGNAL(mouse_clicked(QMouseEvent*)), view, SLOT(on_mouse_clicked(QMouseEvent*)));
    connect(this, SIGNAL(key_pressed(QKeyEvent*)), view, SLOT(on_key_pressed(QKeyEvent*)));

    connect(model, SIGNAL(updated(ModelData)), this, SLOT(on_model_updated()));
    connect(model, SIGNAL(updated(ModelData)), view, SLOT(on_model_updated(ModelData)));
    // connect(model, SIGNAL(memento_restored(ModelData)), view, SLOT(on_model_updated(ModelData)));
}

Controller::~Controller()
{
    // delete m_history;

    // XXX: In this program controller owns model and view
    delete m_model;
    delete m_view;
}

void Controller::on_model_updated()
{
    // ModelMemento memento = m_model->createMemento();
    // m_history->push(memento);

    // qDebug() << "Pushed to memento; History size:" << m_history->size();
}

#include <vector>
#include <fstream>
#include <sstream>
void Controller::on_load_from_file_button_clicked(QString filename)
{
    // XXX
    std::ifstream file(filename.toStdString());
    std::string line;

    if (file.is_open())
    {
        char type;
        Pair<double> v;
        Pair<size_t> l;

        std::vector<Pair<double>> vs;
        std::vector<Pair<size_t>> ls;

        double a, b;

        // Consider file has valid structure
        while (std::getline(file, line))
        {
            if (line[0] == '#') {
                continue;
            }

            std::stringstream ss(line);
            ss >> type >> a >> b;

            if (type == 'v') {
                v.a = a;
                v.b = b;
                vs.push_back(v);
            } else if (type == 'l') {
                l.a = static_cast<size_t>(a);
                l.b = static_cast<size_t>(b);
                ls.push_back(l);
            }
        }

        ModelData md;
        md.vertices = vs;
        md.lines = ls;

        m_model->setData(md);
        // qDebug() << vs.size() << ls.size();
        file.close();
    }
}

// void Controller::on_back_button_clicked()
// {
    // if (m_history->size() > 1) {
        // m_history->pop(); // Remove current state from history
        // ModelMemento memento = m_history->back();
        // m_model->restoreMemento(memento);
    // } else {
        // qDebug() << "Reached bottom of history stack!";
    // }
// }

void Controller::on_key_pressed(QKeyEvent *event)
{
    emit key_pressed(event);
}

void Controller::on_mouse_clicked(QMouseEvent *event)
{
    emit mouse_clicked(event);
}
