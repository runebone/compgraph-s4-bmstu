#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include <vector>
#include "point.h"
#include "error.h"
#include "solution_data.h"

#include "set.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    std::vector<Point> get_first_set_points();
    std::vector<Point> get_second_set_points();
    SolutionData get_solution_data();

    void set_first_set_points(std::vector<Point> &first_set);
    void set_second_set_points(std::vector<Point> &second_set);

private:
    std::vector<Point> first_set;
    std::vector<Point> second_set;

    SolutionData solution_data;

    err_t generate_solution_data();

public slots:
    void solve();
    void add_point1();
    void add_point2();
    // TODO: add_point(x, y)
    void load_from_file(QString filename);

    // meh
    void point_edited(int point_index, Set set, double x, double y);
    void x_edited(int point_index, Set set, double x);
    void y_edited(int point_index, Set set, double y);

    void remove_point(int point_index, Set set);

signals:
    void point_edited_signal(int point_index, Set set, Point p_old, Point p_new);
    void removed_point();
    void updated();
    void restored();

public:
    // Containter of Model data; state of model
    class Memento {
    public:
        Memento(std::vector<Point> fsp, std::vector<Point> ssp) {
            this->first_set = fsp;
            this->second_set = ssp;
        }

        std::vector<Point> first_set;
        std::vector<Point> second_set;
    };

    // const - const member function = promise not to modify object's state
    Memento createMemento() const {
        return Memento(this->first_set, this->second_set);
    }

    void restore(const Memento &memento) {
        this->first_set = memento.first_set;
        this->second_set = memento.second_set;

        emit this->restored();
    }
};

// Caretaker; contains array of mementos (~model states)
class History {
public:
    void push(Model::Memento memento) {
        history.push_back(memento);
    }

    Model::Memento pop() {
        Model::Memento memento = history.back();
        history.pop_back();

        // std::printf("%d\n", history.size());

        return memento;
    }

    unsigned int size() {
        return this->history.size();
    }

    bool take_second = true; // meh

private:
    std::vector<Model::Memento> history;
};


#endif // MODEL_H
