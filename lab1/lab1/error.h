#ifndef __ERROR_H__
#define __ERROR_H__

enum class Error {
    OK = 0,
    SOLUTION_DATA_UNINITIALIZED,

    NOT_ENOUGH_POINTS_IN_FIRST_SET,
    NOT_ENOUGH_POINTS_IN_SECOND_SET,

    EQUIDISTANT_POINTS_NOT_FOUND,
    NO_EQUIDISTANT_POINTS_IN_FIRST_SET,
    NO_EQUIDISTANT_POINTS_IN_SECOND_SET,

    LINES_ARE_PARALLEL,
    LINES_ARE_THE_SAME,
    LINE_SEGMENTS_DO_NOT_INTERSECT,

    POLYGONS_DO_NOT_INTERSECT,
    LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION,
};

#ifdef ENABLE_ERROR_FUNCS
#include <iostream>
#include <string>
std::string get_error_description(Error error);
void print_error(Error error);
#endif // ENABLE_ERROR_FUNCS

#define OK Error::OK
#define ERR Error

typedef Error err_t;

#endif // __ERROR_H__
