#include "variables.h"


variables::variables(/* args */)
{
}

char *variables::printColors(Color theColor){
    switch (theColor) {
        case Color::pink:
            return "pink";
        case Color::red:
            return "red";
        case Color::green:
            return "green";
        case Color::blue:
            return "blue";
        case Color::yellow:
            return "yellow";
        case Color::white:
            return "white";
        case Color::black:
            return "black";
        case Color::orange:
            return "orange";
        case Color::purple:
            return "purple";
        case Color::cyan:
            return "cyan";
        default:
            return "unknown color";
    }
}