#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <vector>
#include "point.h"
#include "error.h"

// This is the only function that has to be avaliable after including algorithm.h
error_t solve(std::vector<Point> &point_array, Point &out_circle_center, double &out_circle_radius);

// Helper-functions used in algorithm.cpp to implement "solve" function
#ifdef __ALGORITHM_PRIVATE__
#include <limits>
#include <cmath>

error_t solve_check_args(std::vector<Point> &point_array);

Point find_special_point(std::vector<Point> &point_array);
void find_two_other_points_of_the_searched_circle(std::vector<Point> &point_array, Point &special_point, Point &out_point1, Point &out_point2);

Point get_circle_center(Point &point1, Point &point2, Point &point3);
double get_circle_radius(Point &point1, Point &point2, Point &point3);
bool is_valid_triangle(Point &point1, Point &point2, Point &point3);
double get_distance(Point &point1, Point &point2);
double get_angle_rad_between_two_vectors(Point &vec1, Point &vec2);
Point get_point_of_lines_intersection(Point& p1, Point& p2, Point& pA, Point& pB);
Point get_point_of_line_lying_on_distance_r_from_x0y0_point_of_the_same_line(double line_slope_coefficient, double x0, double y0, double r);
#endif // __ALGORITHM_PRIVATE__

#endif // __ALGORITHM_H__
