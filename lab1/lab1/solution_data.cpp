#include "solution_data.h"

err_t SolutionData::get_status() {
    return this->status;
}

err_t SolutionData::get_first_circle_center(Point &out_circle_center) {
    if (this->status != ERR::SOLUTION_DATA_UNINITIALIZED) {
        out_circle_center = this->circle_center1;
    }

    return this->status;
}

err_t SolutionData::get_second_circle_center(Point &out_circle_center) {
    if (this->status != ERR::SOLUTION_DATA_UNINITIALIZED) {
        out_circle_center = this->circle_center2;
    }

    return this->status;
}

err_t SolutionData::get_first_hexagon(std::vector<Point> &out_hexagon) {
    if (this->status != ERR::SOLUTION_DATA_UNINITIALIZED) {
        out_hexagon = this->hexagon1;
    }

    return this->status;
}

err_t SolutionData::get_second_hexagon(std::vector<Point> &out_hexagon) {
    if (this->status != ERR::SOLUTION_DATA_UNINITIALIZED) {
        out_hexagon = this->hexagon2;
    }

    return this->status;
}

err_t SolutionData::get_polygon(std::vector<Point> &out_polygon) {
    if (this->status == OK) {
        out_polygon = this->polygon;
    }

    return this->status;
}

err_t SolutionData::get_polygon_mass_center(Point &out_polygon_mass_center) {
    if (this->status == OK) {
        out_polygon_mass_center = this->polygon_mass_center;
    }

    return this->status;
}

err_t SolutionData::get_area(double &out_area) {
    if (this->status == OK) {
        out_area = this->area;
    }

    return this->status;
}

void SolutionData::set_status(const err_t &status) {
    this->status = status;
}

void SolutionData::set_first_circle_center(const Point &circle_center) {
    this->circle_center1 = circle_center;
}

void SolutionData::set_second_circle_center(const Point &circle_center) {
    this->circle_center2 = circle_center;
}

void SolutionData::set_first_hexagon(const std::vector<Point> &hexagon) {
    this->hexagon1 = hexagon;
}

void SolutionData::set_second_hexagon(const std::vector<Point> &hexagon) {
    this->hexagon2 = hexagon;
}

void SolutionData::set_polygon(const std::vector<Point> &polygon) {
    this->polygon = polygon;
}

void SolutionData::set_polygon_mass_center(const Point &polygon_mass_center) {
    this->polygon_mass_center = polygon_mass_center;
}

void SolutionData::set_area(const double &area) {
    this->area = area;
}
