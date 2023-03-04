#define ENABLE_ERROR_FUNCS
#include "error.h"

std::string get_error_description(Error error) {
    std::string description;

    switch (error) {
        case OK:
            description = "Info: No errors.";
            break;
        case Error::SOLUTION_DATA_UNINITIALIZED:
            description = "Info: Solution data uninitialized.";
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
            description = "Info: Lines are parallel.";
            break;
        case Error::LINES_ARE_THE_SAME:
            description = "Info: Lines are the same.";
            break;
        case Error::LINE_SEGMENTS_DO_NOT_INTERSECT:
            description = "Info: Line segments do not intersect.";
            break;

        case Error::POLYGONS_DO_NOT_INTERSECT:
            description = "Info: Polygons do not intersect.";
            break;
        case Error::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION:
            description = "Info: Less than three points of polygons intersection.";
            break;
    }

    return description;
}

void print_error(Error error) {
    std::string description = get_error_description(error);
    std::cout << description << std::endl;
}

bool isNotError(Error error)
{
    switch (error) {
    case OK:
        return true;
    case Error::POLYGONS_DO_NOT_INTERSECT:
        return true;
    case Error::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION:
        return true;
    default:
        return false;
    }
}
