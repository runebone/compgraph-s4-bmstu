#define __ALGORITHM_PRIVATE__
#include "algorithm.h"

/* Вариант 22

Определить радиус и центр окружности минимального радиуса, проходящей хотя бы
через три различные точки заданного множества точек на плоскости, притом, одна
из точек является такой, что сумма расстояний от неѐ до остальных точек всего
множества минимальна.

*/

error_t solve(std::vector<Point> &point_array, Point &out_circle_center, double &out_circle_radius) {
    error_t return_code = OK;

    return_code = solve_check_args(point_array);

    if (return_code == OK) {
        Point special_point, point1, point2;

        special_point = find_special_point(point_array);

        find_two_other_points_of_the_searched_circle(point_array, special_point, point1, point2);

        out_circle_center = get_circle_center(special_point, point1, point2);
        out_circle_radius = get_circle_radius(special_point, point1, point2);
    }
    
    return return_code;
}

error_t solve_check_args(std::vector<Point> &point_array) {
    error_t return_code = OK;

    // Check number of points
    if (point_array.size() < 3) {
        return_code = ERR_NOT_ENOUGH_POINTS_FOR_CIRCLE;
    }

    // Check if points that form a triangle exist in point array
    if (return_code == OK) {
        bool triangle_exists = false;

        for (int i = 0; !triangle_exists && i < point_array.size() - 2; i++) {
            for (int j = i + 1; !triangle_exists && j < point_array.size() - 1; j++) {
                for (int k = j + 1; !triangle_exists && k < point_array.size(); k++) {
                    if (is_valid_triangle(point_array[i], point_array[j], point_array[k])) {
                        triangle_exists = true;
                    }
                }
            }
        }

        if (!triangle_exists) {
            return_code = ERR_TRIANGLE_DOES_NOT_EXIST;
        }
    }

    return return_code;
}

Point find_special_point(std::vector<Point> &point_array) {
    // Find point such that the sum of distances from it to all other points
    // is minimal.

    Point special_point;

    int special_point_index = 0;
    double min_sum_of_distances = std::numeric_limits<double>::infinity();

    for (int i = 0; i < point_array.size() - 1; i++) {
        double sum_of_distances = 0.0; // from point i to all points j

        for (int j = i + 1; j < point_array.size(); j++) {
            sum_of_distances += get_distance(point_array[i], point_array[j]);
        }

        if (sum_of_distances < min_sum_of_distances) {
            min_sum_of_distances = sum_of_distances;
            special_point_index = i;
        }
    }

    special_point = point_array[special_point_index];

    return special_point;
}

void find_two_other_points_of_the_searched_circle(std::vector<Point> &point_array, Point &special_point, Point &out_point1, Point &out_point2) {
    int point1_index = 0;
    int point2_index = 0;

    double min_radius = std::numeric_limits<double>::infinity();

    for (int i = 0; i < point_array.size() - 1; i++) {
        for (int j = i + 1; j < point_array.size(); j++) {
            double radius = 0.0;

            if (is_valid_triangle(special_point, point_array[i], point_array[j])) {
                radius = get_circle_radius(special_point, point_array[i], point_array[j]);
            }

            if (radius != 0.0 && radius < min_radius) {
                min_radius = radius;
                point1_index = i;
                point2_index = j;
            }
        }
    }

    out_point1 = point_array[point1_index];
    out_point2 = point_array[point2_index];
}

#define R 1.0 // Can be anything greater than 0
Point get_circle_center(Point &point1, Point &point2, Point &point3) {
    Point ab_mid, bc_mid;

    ab_mid.x = (point1.x + point2.x) / 2.0;
    ab_mid.y = (point1.y + point2.y) / 2.0;

    bc_mid.x = (point2.x + point3.x) / 2.0;
    bc_mid.y = (point2.y + point3.y) / 2.0;

    double ab_mid_perp_equation_k = 0.0;
    double ab_mid_perp_equation_x0 = ab_mid.x;
    double ab_mid_perp_equation_y0 = ab_mid.y;

    double bc_mid_perp_equation_k = 0.0;
    double bc_mid_perp_equation_x0 = bc_mid.x;
    double bc_mid_perp_equation_y0 = bc_mid.y;

    bool ab_mid_perp_is_vertical = (point2.x == point1.x);
    bool bc_mid_perp_is_vertical = (point3.x == point2.x);

    if (!ab_mid_perp_is_vertical) {
        ab_mid_perp_equation_k = (point2.y - point1.y) / (point2.x - point1.x);
    }

    if (!bc_mid_perp_is_vertical) {
        bc_mid_perp_equation_k = (point3.y - point2.y) / (point3.x - point2.x);
    }

    Point new_point_on_ab_mid_perp;
    if (!ab_mid_perp_is_vertical) {
        double k = ab_mid_perp_equation_k;
        double x0 = ab_mid_perp_equation_x0;
        double y0 = ab_mid_perp_equation_y0;

        new_point_on_ab_mid_perp = get_point_of_line_lying_on_distance_r_from_x0y0_point_of_the_same_line(k, x0, y0, R);
    } else {
        double x0 = ab_mid_perp_equation_x0;
        double y0 = ab_mid_perp_equation_y0;

        new_point_on_ab_mid_perp = { .x = x0, .y = y0 + R };
    }

    Point new_point_on_bc_mid_perp;
    if (!bc_mid_perp_is_vertical) {
        double k = bc_mid_perp_equation_k;
        double x0 = bc_mid_perp_equation_x0;
        double y0 = bc_mid_perp_equation_y0;

        new_point_on_bc_mid_perp = get_point_of_line_lying_on_distance_r_from_x0y0_point_of_the_same_line(k, x0, y0, R);
    } else {
        double x0 = bc_mid_perp_equation_x0;
        double y0 = bc_mid_perp_equation_y0;

        new_point_on_bc_mid_perp = { .x = x0, .y = y0 + R };
    }

    Point circle_center = get_point_of_lines_intersection(ab_mid, new_point_on_ab_mid_perp, bc_mid, new_point_on_bc_mid_perp);

    return circle_center;
}

double get_circle_radius(Point &point1, Point &point2, Point &point3) {
    // Applying formula: AB / sin C = 2R
    Point vec1, vec2;
    vec1.x = point3.x - point1.x;
    vec1.y = point3.y - point1.y;

    vec2.x = point3.x - point2.x;
    vec2.y = point3.y - point2.y;

    double ab_length = get_distance(point1, point2);
    double angle_rad = get_angle_rad_between_two_vectors(vec1, vec2);

    double circle_radius = ab_length / (2.0 * std::sin(angle_rad));

    return circle_radius;
}

bool is_valid_triangle(Point &point1, Point &point2, Point &point3) {
    double ab = get_distance(point1, point2);
    double ac = get_distance(point1, point3);
    double bc = get_distance(point2, point3);

    bool condition1 = (ab + ac) > bc;
    bool condition2 = (ab + bc) > ac;
    bool condition3 = (ac + bc) > ab;

    return (condition1 && condition2 && condition3);
}

double get_distance_2d(Point &point1, Point &point2) {
    double dx = point2.x - point1.x;
    double dy = point2.y - point1.y;

    return std::sqrt(dx * dx + dy * dy);
}

double get_distance(Point &point1, Point &point2) {
    return get_distance_2d(point1, point2);
}

double get_angle_rad_between_two_vectors_2d(Point &vec1, Point &vec2) {
    double dot_product = vec1.x * vec2.x + vec1.y * vec2.y;
    double vec1_length = std::sqrt(vec1.x * vec1.x + vec1.y * vec1.y);
    double vec2_length = std::sqrt(vec2.x * vec2.x + vec2.y * vec2.y);

    double cosine_of_angle_between_vectors = dot_product / (vec1_length * vec2_length);

    return std::acos(cosine_of_angle_between_vectors);
}

double get_angle_rad_between_two_vectors(Point &vec1, Point &vec2) {
    return get_angle_rad_between_two_vectors_2d(vec1, vec2);
}

Point get_point_of_lines_intersection(Point& p1, Point& p2, Point& pA, Point& pB) {
    double x1 = p1.x;
    double x2 = p2.x;
    double y1 = p1.y;
    double y2 = p2.y;

    double xA = pA.x;
    double xB = pB.x;
    double yA = pA.y;
    double yB = pB.y;

    double dx1 = x2 - x1;
    double dy1 = y2 - y1;
    double k1 = dy1 / dx1;
    double b1 = k1 * x1 - y1;

    double dxA = xB - xA;
    double dyA = yB - yA;
    double kA = dyA / dxA;
    double bA = kA * xA - yA;

    double intersect_x = 0;
    double intersect_y = 0;

    // Handle vertical lines cases
    if (dx1 == 0 || dxA == 0)
    {
        if (dx1 == 0)
        {
            intersect_x = x1;
            intersect_y = kA * x1 - bA;
        }
        if (dxA == 0)
        {
            intersect_x = xA;
            intersect_y = k1 * xA - b1;
        }
    }
    else
    {
        double det = kA - k1;
        double detX = bA - b1;
        double detY = k1 * bA - kA * b1;

        intersect_x = detX / det;
        intersect_y = detY / det;
    }

    // TODO: handle errors

    Point intersection_point;
    intersection_point.x = intersect_x;
    intersection_point.y = intersect_y;

    return intersection_point;
}

Point get_point_of_line_lying_on_distance_r_from_x0y0_point_of_the_same_line(double line_slope_coefficient, double x0, double y0, double r) {
    double k = line_slope_coefficient;

    double numerator = -x0 + std::sqrt(k * k * (r * r - x0 * x0) + r * r);
    double denominator = k * k + 1;

    double new_x = numerator / denominator;
    double new_y = k * (new_x - x0) + y0;

    Point point = { .x = new_x, .y = new_y };

    return point;
}
