#define ENABLE_ERROR_FUNCS
#include "error.h"

std::string get_error_description(Error error) {
    std::string description;

    switch (error) {
        case OK:
            description = "No errors.";
            break;

        case Error::NOT_ENOUGH_POINTS_IN_FIRST_SET:
            description = "Error: Not enough points in first set.";
            break;
        case Error::NOT_ENOUGH_POINTS_IN_SECOND_SET:
            description = "Error: Not enough points in second set.";
            break;

        case Error::EQUIDISTANT_POINTS_NOT_FOUND:
            description = "Error: Equidistant points not found.";
            break;
        case Error::NO_EQUIDISTANT_POINTS_IN_FIRST_SET:
            description = "Error: No equidistant points in first set.";
            break;
        case Error::NO_EQUIDISTANT_POINTS_IN_SECOND_SET:
            description = "Error: No equidistant points in second set.";
            break;

        case Error::LINES_ARE_PARALLEL:
            description = "Error: Lines are parallel.";
            break;
        case Error::LINES_ARE_THE_SAME:
            description = "Error: Lines are the same.";
            break;
        case Error::LINE_SEGMENTS_DO_NOT_INTERSECT:
            description = "Error: Line segments do not intersect.";
            break;

        case Error::POLYGONS_DO_NOT_INTERSECT:
            description = "Error: Polygons do not intersect.";
            break;
        case Error::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION:
            description = "Error: Less than three points of polygons intersection.";
            break;
    }

    return description;
}

void print_error(Error error) {
    std::string description = get_error_description(error);
    std::cout << description << std::endl;
}
