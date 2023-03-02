#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <vector>
#include "point.h"
#include "error.h"
#include "solution_data.h"

// This is the only function that has to be avaliable after including algorithm.h
namespace Algorithm {
    err_t solve(const std::vector<Point> &first_set_points, const std::vector<Point> &second_set_points, SolutionData &out_solution_data);
}

// Helper-functions used in algorithm.cpp to implement "solve" function
#ifdef __ALGORITHM_PRIVATE__
#include <cmath>
#include <cstdio>
#include <algorithm> // std::sort

err_t solve_check_arguments(std::vector<Point> &first_set_points, std::vector<Point> &second_set_points);
err_t find_three_equidistant_points(std::vector<Point> &points, Point &out_point1, Point &out_point2, Point &out_point3);
err_t find_convex_intersection_polygon(std::vector<Point> &polygon1, std::vector<Point> &polygon2, std::vector<Point> &out_polygon);
bool is_point_inside_polygon(const std::vector<Point>& polygon, const Point& point);
Point get_circle_center(Point &point1, Point &point2, Point &point3);
Point get_mass_center_of_convex_polygon(std::vector<Point> convex_polygon);
Point find_diametrically_opposite_point_on_circle(Point &circle_center, Point &point_on_circle);
err_t get_line_segments_intersection_point(Point &l1p1, Point &l1p2, Point &l2p1, Point &l2p2, Point &out_intersection_point);
void remove_dublicate_points_from_polygon(std::vector<Point> &polygon);
double get_signed_area_parallelogram(Point &point1, Point &point2, Point &point3);
double get_triangle_area(Point &point1, Point &point2, Point &point3);
double get_area_of_convex_polygon(std::vector<Point> &polygon_sorted, Point &point_inside_polygon);
// double get_area_of_polygon(std::vector<Point> &polygon);
bool are_points_in_clockwise_order(Point &point1, Point &point2, Point &point3);
bool are_points_on_the_same_line(Point &point1, Point &point2, Point &point3);
bool is_point_inside_clip_edge(Point &point, Point &clip_edge_start, Point &clip_edge_end);
double get_distance(Point &point1, Point &point2);
double get_angle(Point &point1, Point &point2);
bool is_equal(double x, double y);

#endif // __ALGORITHM_PRIVATE__

#endif // __ALGORITHM_H__
