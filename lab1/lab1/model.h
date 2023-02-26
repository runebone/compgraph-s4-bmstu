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
    void delete_all_points();

    // meh
    void point_edited(int point_index, Set set, double x, double y);
    void x_edited(int point_index, Set set, double x);
    void y_edited(int point_index, Set set, double y);

    void remove_point(int point_index, Set set);

signals:
    void point_edited_signal(int point_index, Set set, double x, double y);
    void updated();

};

#endif // MODEL_H
