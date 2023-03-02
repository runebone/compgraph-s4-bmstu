#ifndef __SOLUTION_DATA_H__
#define __SOLUTION_DATA_H__

#include <vector>
#include "error.h"
#include "point.h"

class SolutionData {
public:
    err_t get_status();

    err_t get_first_circle_center(Point &out_circle_center);
    err_t get_second_circle_center(Point &out_circle_center);

    err_t get_first_circle_radius(double &out_circle_radius);
    err_t get_second_circle_radius(double &out_circle_radius);

    err_t get_first_hexagon(std::vector<Point> &out_hexagon);
    err_t get_second_hexagon(std::vector<Point> &out_hexagon);

    err_t get_polygon(std::vector<Point> &out_polygon);
    err_t get_polygon_mass_center(Point &out_polygon_mass_center);

    err_t get_area(double &out_area);

    void set_status(const err_t &status);

    void set_first_circle_center(const Point &circle_center);
    void set_second_circle_center(const Point &circle_center);

    void set_first_circle_radius(double circle_radius);
    void set_second_circle_radius(double circle_radius);

    void set_first_hexagon(const std::vector<Point> &hexagon);
    void set_second_hexagon(const std::vector<Point> &hexagon);

    void set_polygon(const std::vector<Point> &polygon);
    void set_polygon_mass_center(const Point &polygon_mass_center);

    void set_area(const double &area);

private:
    err_t status = ERR::SOLUTION_DATA_UNINITIALIZED;

    Point circle_center1;
    Point circle_center2;

    double circle_radius1;
    double circle_radius2;

    std::vector<Point> hexagon1;
    std::vector<Point> hexagon2;

    std::vector<Point> polygon;
    Point polygon_mass_center;

    double area;
};

#endif // __SOLUTION_DATA_H__
