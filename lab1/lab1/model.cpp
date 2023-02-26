#include "model.h"

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
    this->generate_solution_data();
    std::printf("Solution status: %d\n", this->solution_data.get_status());
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

void Model::load_from_file()
{
    // ...
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
