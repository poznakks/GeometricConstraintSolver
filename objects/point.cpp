//
// Created by Vlad Boguzh on 17.09.2024.
//

#include "point.h"
#include <cmath>
#include <iostream>

Point::Point(const double x, const double y, const double z) : x(x), y(y), z(z) {}

double Point::distanceTo(const Point& other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
}

void Point::move(const double dx, const double dy, const double dz) {
    x += dx;
    y += dy;
    z += dz;
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "Point(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
}