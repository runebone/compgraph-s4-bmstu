#include "model.h"

#include <fstream>
#include <sstream>

#include "algorithm.h"

Model::Model(QObject *parent) : QObject(parent)
{

}

std::vector<Point> Model::get_first_set_points()
{
    return this->first_set;
}

std::vector<Point> Model::get_second_set_points()
{
    return this->second_set;
}

SolutionData Model::get_solution_data()
{
    return this->solution_data;
}

void Model::set_first_set_points(std::vector<Point> &first_set)
{
    this->first_set = first_set;
}

void Model::set_second_set_points(std::vector<Point> &second_set)
{
    this->second_set = second_set;
}

err_t Model::generate_solution_data()
{
    return Algorithm::solve(this->first_set, this->second_set, this->solution_data);
}

#include <cstdio>
void Model::solve()
{
    err_t rc = this->generate_solution_data();
    std::printf("Solution status: %d | %d\n", this->solution_data.get_status(), rc);

    // FIXME meeeeh
    this->solution_data.set_status(rc);
}

void Model::add_point1()
{
    // ...
    emit this->updated();
}

void Model::add_point2()
{
    // ...
    emit this->updated();
}

void Model::load_from_file(QString filename)
{
    std::ifstream file(filename.toStdString());
    std::string line;

    if (file.is_open())
    {
        Point p;
        int set;

        // Consider file has valid structure
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            ss >> set >> p.x >> p.y;

            if (set == 1) {
                this->first_set.push_back(p);
            } else if (set == 2) {
                this->second_set.push_back(p);
            }
        }
        file.close();
    }

    emit this->updated();
}

void Model::delete_all_points()
{
    // ...
    emit this->updated();
}

void Model::point_edited(int point_index, Set set, double x, double y)
{
}

void Model::x_edited(int point_index, Set set, double x)
{

}

void Model::y_edited(int point_index, Set set, double y)
{

}

void Model::remove_point(int point_index, Set set)
{
    if (set == FIRST) {
        this->first_set.erase(first_set.begin() + point_index);
    } else if (set == SECOND) {
        this->second_set.erase(second_set.begin() + point_index);
    }
}
