#define __ALGORITHM_PRIVATE__
#include "algorithm.h"

/* Вариант 24

Заданы два множества точек на плоскости. В каждом множестве найти три
равноудалѐнные друг от друга точки. Через эти точки провести окружности. В
каждую окружность вписать шестиугольник. Найти площадь пересечения двух
получившихся шестиугольников.

*/

/* Алгоритм решения

   1) Убедиться в том, что в каждом множестве есть по крайней мере три точки
   2) Убедиться в том, что в каждом множестве есть тройка равноудалённых
   точек
   3) Найти первую (по порядку) тройку равноудалённых точек из первого
   множества
   4) Найти первую (по порядку) тройку равноудалённых точек из
   второго множества
   5) Найти координаты центра окружности, проходящей через тройку точек из
   первого множества
   6) Найти координаты центра окружности, проходящей через тройку точек из
   второго множества
   7) Для каждой точки на окружностях, найти диаметрально противоположную ей
   8) Отсортировать точки в полученных шестиугольниках
   9) Найти область пересечения двух шестиугольников (которая будет выпуклым
   многоугольником)
   10) Найти какую-нибудь точку внутри полученного многоугольника (берём
   центр масс так как многоугольник выпуклый, и ц. м. точно будет внутри)
   11) Отсортировать точки в многоугольнике (против/по) часовой стрелке
   относительно центра масс
   12) Найти площадь многоугольника, суммируя площади составляющих его
   треугольников
*/

#include <chrono>
#include <iostream>
#include <string>
class Timer {
public:
    Timer(std::string str) {
        start_ = std::chrono::high_resolution_clock::now();

        this->str = str;
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();
        std::cout << "Time taken (" << this->str << "): " << duration << " microseconds" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::string str;
};

err_t Algorithm::solve(const std::vector<Point> &first_set_points, const std::vector<Point> &second_set_points, SolutionData &out_solution_data) {
    err_t return_code = OK;

    return_code = solve_check_arguments(first_set_points, second_set_points);

    Timer *t = new Timer("equidistant points");
    Point p1, p2, p3;
    Point pa, pb, pc;
    if (return_code == OK) {
        find_three_equidistant_points(first_set_points, p1, p2, p3);
        find_three_equidistant_points(second_set_points, pa, pb, pc);
    }
    delete t;

    t = new Timer("circle centers");
    Point circle_center1;
    Point circle_center2;
    if (return_code == OK) {
        circle_center1 = get_circle_center(p1, p2, p3);
        circle_center2 = get_circle_center(pa, pb, pc);
    }
    delete t;

    t = new Timer("diametrically opposites");
    Point p1_diametrically_opposite;
    Point p2_diametrically_opposite;
    Point p3_diametrically_opposite;
    Point pa_diametrically_opposite;
    Point pb_diametrically_opposite;
    Point pc_diametrically_opposite;
    if (return_code == OK) {
        p1_diametrically_opposite = find_diametrically_opposite_point_on_circle(circle_center1, p1);
        p2_diametrically_opposite = find_diametrically_opposite_point_on_circle(circle_center1, p2);
        p3_diametrically_opposite = find_diametrically_opposite_point_on_circle(circle_center1, p3);

        pa_diametrically_opposite = find_diametrically_opposite_point_on_circle(circle_center2, pa);
        pb_diametrically_opposite = find_diametrically_opposite_point_on_circle(circle_center2, pb);
        pc_diametrically_opposite = find_diametrically_opposite_point_on_circle(circle_center2, pc);
    }
    delete t;

    t = new Timer("hexagons");
    // Create two vectors, each containing points of hexagon is cw order
    std::vector<Point> hexagon1;
    std::vector<Point> hexagon2;
    if (return_code == OK) {
        if (!are_points_in_clockwise_order(p1, p2, p3)) {
            std::swap(p2, p3);
            std::swap(p2_diametrically_opposite, p3_diametrically_opposite);
        }
        if (!are_points_in_clockwise_order(pa, pb, pc)) {
            std::swap(pb, pc);
            std::swap(pb_diametrically_opposite, pc_diametrically_opposite);
        }

        hexagon1.push_back(p1);
        hexagon1.push_back(p3_diametrically_opposite);
        hexagon1.push_back(p2);
        hexagon1.push_back(p1_diametrically_opposite);
        hexagon1.push_back(p3);
        hexagon1.push_back(p2_diametrically_opposite);

        hexagon2.push_back(pa);
        hexagon2.push_back(pc_diametrically_opposite);
        hexagon2.push_back(pb);
        hexagon2.push_back(pa_diametrically_opposite);
        hexagon2.push_back(pc);
        hexagon2.push_back(pb_diametrically_opposite);
    }
    delete t;

    t = new Timer("intersection polygon");
    std::vector<Point> convex_polygon_of_hexagons_intersection;
    if (return_code == OK) {
        return_code = find_convex_intersection_polygon(hexagon1, hexagon2, convex_polygon_of_hexagons_intersection);
    }
    delete t;

    t = new Timer("mass center");
    // Find some point (mass center) inside of the CONVEX polygon
    Point mass_center;
    if (return_code == OK) {
        mass_center = get_mass_center_of_convex_polygon(convex_polygon_of_hexagons_intersection);
    }
    delete t;

    t = new Timer("sorting polygon vertices");
    // Sort polygon vertices cw around the point found in the previous step
    if (return_code == OK) {
        /* auto lambda_less = [&mass_center](Point &a, Point &b) { return are_points_in_clockwise_order(mass_center, a, b); }; */
        auto lambda_less = [&mass_center](Point &a, Point &b) { return get_angle(mass_center, a) < get_angle(mass_center, b); };
        std::sort(convex_polygon_of_hexagons_intersection.begin(), convex_polygon_of_hexagons_intersection.end(), lambda_less);
    }
    delete t;

    t = new Timer("calculating area");
    // Calculate area of the convex polygon, adding up areas of triangles which it consists of
    double convex_polygon_area = 0.0;
    if (return_code == OK) {
        convex_polygon_area = get_area_of_convex_polygon(convex_polygon_of_hexagons_intersection, mass_center);
    }
    delete t;

    t = new Timer("setting solution data");
    // Setting solution data
    if (return_code == OK
            || return_code == ERR::POLYGONS_DO_NOT_INTERSECT
            || return_code == ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION)
    {
        out_solution_data.set_status(return_code);

        out_solution_data.set_first_circle_center(circle_center1);
        out_solution_data.set_second_circle_center(circle_center2);

        double first_circle_radius = get_distance(hexagon1[0], circle_center1);
        double second_circle_radius = get_distance(hexagon2[0], circle_center2);

        out_solution_data.set_first_circle_radius(first_circle_radius);
        out_solution_data.set_second_circle_radius(second_circle_radius);

        out_solution_data.set_first_hexagon(hexagon1);
        out_solution_data.set_second_hexagon(hexagon2);

        out_solution_data.set_polygon(convex_polygon_of_hexagons_intersection);
        out_solution_data.set_polygon_mass_center(mass_center);

        out_solution_data.set_area(convex_polygon_area);
    }
    delete t;

    return return_code;
}

err_t solve_check_arguments(std::vector<Point> &first_set_points, std::vector<Point> &second_set_points) {
    err_t return_code = OK;

    // Make sure each set contains at least three points
    if (first_set_points.size() < 3) {
        return_code = ERR::NOT_ENOUGH_POINTS_IN_FIRST_SET;
    } else if (second_set_points.size() < 3) {
        return_code = ERR::NOT_ENOUGH_POINTS_IN_SECOND_SET;
    }

    // Make sure there are at least three equidistant points in each set
    if (return_code == OK) {
        Point tmp_point1, tmp_point2, tmp_point3;

        return_code = find_three_equidistant_points(first_set_points, tmp_point1, tmp_point2, tmp_point3);

        if (return_code != OK) {
            return_code = ERR::NO_EQUIDISTANT_POINTS_IN_FIRST_SET;
        }
    }

    if (return_code == OK) {
        Point tmp_point1, tmp_point2, tmp_point3;

        return_code = find_three_equidistant_points(second_set_points, tmp_point1, tmp_point2, tmp_point3);

        if (return_code != OK) {
            return_code = ERR::NO_EQUIDISTANT_POINTS_IN_SECOND_SET;
        }
    }

    return return_code;
}

err_t find_three_equidistant_points(std::vector<Point> &points, Point &out_point1, Point &out_point2, Point &out_point3) {
    err_t return_code = OK;

    bool equidistant_points_found = false;
    int point1_index = 0;
    int point2_index = 0;
    int point3_index = 0;

    for (int i = 0; !equidistant_points_found && i < points.size() - 2; i++) {
        for (int j = i + 1; !equidistant_points_found && j < points.size() - 1; j++) {
            double distance1 = get_distance(points[i], points[j]);
            for (int k = j + 1; !equidistant_points_found && k < points.size(); k++) {
                double distance2 = get_distance(points[i], points[k]);
                double distance3 = get_distance(points[j], points[k]);

                bool condition1 = is_equal(distance1, distance2);
                bool condition2 = is_equal(distance1, distance3);
                bool condition3 = is_equal(distance2, distance3);

                bool three_points_are_equidistant = condition1 && condition2 && condition3;

                if (three_points_are_equidistant && !are_points_on_the_same_line(points[i], points[j], points[k])) {
                    equidistant_points_found = true;

                    point1_index = i;
                    point2_index = j;
                    point3_index = k;
                }
            }
        }
    }

    if (!equidistant_points_found) {
        return_code = ERR::EQUIDISTANT_POINTS_NOT_FOUND;
    } else {
        out_point1 = points[point1_index];
        out_point2 = points[point2_index];
        out_point3 = points[point3_index];
    }

    return return_code;
}

err_t find_convex_intersection_polygon(std::vector<Point> &polygon1, std::vector<Point> &polygon2, std::vector<Point> &out_polygon) {
    err_t return_code = OK;

    std::vector<Point> intersection_polygon;

    for (Point p: polygon1) {
        /* if (is_point_inside_polygon_raycast(p, polygon2)) { */
        if (is_point_inside_polygon(polygon2, p)) {
            intersection_polygon.push_back(p);
        }
    }

    for (Point p: polygon2) {
        /* if (is_point_inside_polygon_raycast(p, polygon1)) { */
        if (is_point_inside_polygon(polygon1, p)) {
            intersection_polygon.push_back(p);
        }
    }

    for (int i = 0; i < polygon1.size(); i++) {
        for (int j = 0; j < polygon2.size(); j++) {
            Point edge_of_polygon1_start = polygon1[i];
            Point edge_of_polygon1_end = polygon1[(i + 1) % polygon1.size()];

            Point edge_of_polygon2_start = polygon2[j];
            Point edge_of_polygon2_end = polygon2[(j + 1) % polygon2.size()];

            err_t status = OK;
            Point intersection_point;
            status = get_line_segments_intersection_point(
                    edge_of_polygon1_start, edge_of_polygon1_end,
                    edge_of_polygon2_start, edge_of_polygon2_end,
                    intersection_point
                    );

            if (status == OK) {
                intersection_polygon.push_back(intersection_point);
            }
        }
    }

    if (intersection_polygon.size() > 1) {
        remove_dublicate_points_from_polygon(intersection_polygon);
    }

    if (intersection_polygon.size() == 0) {
        return_code = ERR::POLYGONS_DO_NOT_INTERSECT;
    } else if (intersection_polygon.size() < 3) {
        return_code = ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION;
    }

    if (return_code == OK) {
        out_polygon = intersection_polygon;
    }

    return return_code;
}

bool is_point_inside_polygon(const std::vector<Point>& polygon, const Point& point) {
    bool is_inside = false;

    // The function loops over all edges of the polygon, and for each edge, it
    // checks whether the point is to the left or to the right of the edge using
    // the cross product. If the point is on the left side of an odd number of
    // edges, then it is inside the polygon.
    for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
        if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
            (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            is_inside = !is_inside;
        }
    }

    return is_inside;
}

Point get_circle_center(Point &point1, Point &point2, Point &point3) {
    /*
    Find circle center from the following system of equations:
      { (x1 - x0)^2 + (y1 - y0)^2 = r^2
      { (x2 - x0)^2 + (y2 - y0)^2 = r^2
      { (x3 - x0)^2 + (y3 - y0)^2 = r^2

    which we can simplify:
      { (x1 - x0)^2 + (y1 - y0)^2 = (x2 - x0)^2 + (y2 - y0)^2
      { (x1 - x0)^2 + (y1 - y0)^2 = (x3 - x0)^2 + (y3 - y0)^2

    and give the result in the following form:
      x0 = (A - B) / (C - D)
      y0 = -C * x0 + A

    where A, B, C, D - some functions of x1, y1, x2, y2, x3, y3.
    */
    double x1 = point1.x;
    double y1 = point1.y;
    double x2 = point2.x;
    double y2 = point2.y;
    double x3 = point3.x;
    double y3 = point3.y;

    // FIXED: zero division if y1 == y2 or y1 == y3
    if (y1 == y2) {
        std::swap(x1, x3);
        std::swap(y1, y3);
    } else if (y1 == y3) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    double A = (x1 * x1 - x3 * x3 + y1 * y1 - y3 * y3) / (2 * (y1 - y3));
    double B = (x1 * x1 - x2 * x2 + y1 * y1 - y2 * y2) / (2 * (y1 - y2));
    double C = (x1 - x3) / (y1 - y3);
    double D = (x1 - x2) / (y1 - y2);

    double xc = (A - B) / (C - D);
    double yc = -C * xc + A;

    Point circle_center = { .x = xc, .y = yc };

    return circle_center;
}

Point get_mass_center_of_convex_polygon(std::vector<Point> convex_polygon) {
    double mc_x = 0.0;
    double mc_y = 0.0;

    for (int i = 0; i < convex_polygon.size(); i++) {
        mc_x += convex_polygon[i].x;
        mc_y += convex_polygon[i].y;
    }

    mc_x /= convex_polygon.size();
    mc_y /= convex_polygon.size();

    Point mass_center = { .x = mc_x, .y = mc_y };

    return mass_center;
}

Point find_diametrically_opposite_point_on_circle(Point &circle_center, Point &point_on_circle) {
    Point radius_vector = {
        .x = point_on_circle.x - circle_center.x,
        .y = point_on_circle.y - circle_center.y
    };
    Point diametrically_opposite_point_on_circle = {
        .x = circle_center.x - radius_vector.x,
        .y = circle_center.y - radius_vector.y
    };

    return diametrically_opposite_point_on_circle;
}

err_t get_line_segments_intersection_point(Point &l1p1, Point &l1p2, Point &l2p1, Point &l2p2, Point &out_intersection_point) {
    err_t return_code = OK;

    double x11 = l1p1.x;
    double x12 = l1p2.x;
    double x21 = l2p1.x;
    double x22 = l2p2.x;

    double y11 = l1p1.y;
    double y12 = l1p2.y;
    double y21 = l2p1.y;
    double y22 = l2p2.y;

    // Solve the following system of linear equations:
    // { x * (y12 - y11) - y * (x12 - x11) = x11 * (y12 - y11) - y11 * (x12 - x11)
    // { x * (y22 - y21) - y * (x22 - x21) = x21 * (y22 - y21) - y21 * (x22 - x21)

    double dx1 = x12 - x11;
    double dy1 = y12 - y11;
    double dx2 = x22 - x21;
    double dy2 = y22 - y21;

    double b1 = x11 * (y12 - y11) - y11 * (x12 - x11);
    double b2 = x21 * (y22 - y21) - y21 * (x22 - x21);

    double det = dx1 * dy2 - dx2 * dy1;
    double detX = dx1 * b2 - dx2 * b1;
    // double detY = b1 * dy2 - b2 * dy1; // TODO check why not this
    double detY = dy1 * b2 - dy2 * b1;

    if (is_equal(det, 0.0)) {
        if (is_equal(detX, 0.0) && is_equal(detY, 0.0)) {
            return_code = ERR::LINES_ARE_THE_SAME;
        } else {
            return_code = ERR::LINES_ARE_PARALLEL;
        }
    } else {
        double x = detX / det;
        double y = detY / det;

        double min_x1 = std::min(x11, x12);
        double max_x1 = std::max(x11, x12);
        double min_y1 = std::min(y11, y12);
        double max_y1 = std::max(y11, y12);

        double min_x2 = std::min(x21, x22);
        double max_x2 = std::max(x21, x22);
        double min_y2 = std::min(y21, y22);
        double max_y2 = std::max(y21, y22);

        bool intersection_is_on_line1 = (min_x1 < x || is_equal(min_x1, x))
            && (max_x1 > x || is_equal(max_x1, x))
            && (min_y1 < y || is_equal(min_y1, y))
            && (max_y1 > y || is_equal(max_y1, y));

        bool intersection_is_on_line2 = (min_x2 < x || is_equal(min_x2, x))
            && (max_x2 > x || is_equal(max_x2, x))
            && (min_y2 < y || is_equal(min_y2, y))
            && (max_y2 > y || is_equal(max_y2, y));

        if (intersection_is_on_line1 && intersection_is_on_line2) {
            out_intersection_point = { .x = x, .y = y };
        } else {
            return_code = ERR::LINE_SEGMENTS_DO_NOT_INTERSECT;
        }
    }

    return return_code;
}

void remove_dublicate_points_from_polygon(std::vector<Point> &polygon) {
    int restart = 1;

    while (restart) {
        restart = 0;

        for (int i = 0; !restart && i < polygon.size() - 1; i++) {
            for (int j = i + 1; !restart && j < polygon.size(); j++) {
                double xi = polygon[i].x;
                double xj = polygon[j].x;
                double yi = polygon[i].y;
                double yj = polygon[j].y;
                if (is_equal(xi, xj) && is_equal(yi, yj)) {
                    polygon.erase(polygon.begin() + j);
                    restart = 1;
                }
            }
        }
    }
}

double get_signed_area_parallelogram(Point &point1, Point &point2, Point &point3) {
    double x1 = point1.x;
    double y1 = point1.y;
    double x2 = point2.x;
    double y2 = point2.y;
    double x3 = point3.x;
    double y3 = point3.y;

    return (x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1);
}

double get_triangle_area(Point &point1, Point &point2, Point &point3) {
    return std::fabs(get_signed_area_parallelogram(point1, point2, point3)) / 2.0;
}

double get_area_of_convex_polygon(std::vector<Point> &polygon_sorted, Point &point_inside_polygon) {
    double area = 0.0;

    for (int i = 0; i < polygon_sorted.size(); i++) {
        Point p1 = polygon_sorted[i];
        Point p2 = polygon_sorted[(i + 1) % polygon_sorted.size()];

        area += get_triangle_area(point_inside_polygon, p1, p2);
    }

    return area;
}

double get_area_of_polygon(std::vector<Point> &polygon) {
    double area = 0.0;

    for (int i = 0; i < polygon.size(); i++) {
        Point p = i ? polygon[i - 1] : polygon.back();
        Point q = polygon[i];
        area += (p.x - q.x) * (p.y + q.y);
    }

    return std::fabs(area) / 2.0;
}

bool are_points_in_clockwise_order(Point &point1, Point &point2, Point &point3) {
    double x1 = point1.x;
    double y1 = point1.y;
    double x2 = point2.x;
    double y2 = point2.y;
    double x3 = point3.x;
    double y3 = point3.y;

    double double_signed_area = (x2 - x1) * (y3 - y2) - (x3 - x2) * (y3 - y1);

    bool points_are_cw = double_signed_area < 0.0;

    return points_are_cw;
}

bool are_points_on_the_same_line(Point &point1, Point &point2, Point &point3) {
    double distance12 = get_distance(point1, point2);
    double distance13 = get_distance(point1, point3);
    double distance23 = get_distance(point2, point3);

    return is_equal(distance12 + distance23, distance13);
}

bool is_point_inside_clip_edge(Point &point, Point &clip_edge_start, Point &clip_edge_end) {
    bool point_is_inside_clip_edge = are_points_in_clockwise_order(clip_edge_start, clip_edge_end, point)
        || are_points_on_the_same_line(clip_edge_start, clip_edge_end, point);

    return point_is_inside_clip_edge;
}

double get_distance_2d(Point &point1, Point &point2) {
    double dx = point2.x - point1.x;
    double dy = point2.y - point1.y;

    return std::sqrt(dx * dx + dy * dy);
}

double get_distance(Point &point1, Point &point2) {
    return get_distance_2d(point1, point2);
}

double get_angle(Point &point1, Point &point2) {
    return atan2(point2.y - point1.y, point2.x - point1.x);
}

#define EPSILON 1e-06
bool is_equal(double x, double y) {
    return std::fabs(x - y) <= EPSILON;
}

