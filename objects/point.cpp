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
    setDirty(true);
}

double Point::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Point Point::normalized() const {
    double len = length();
    if (len < 1e-8) return Point(0, 0, 0); // предотвращаем деление на ноль
    return Point(x / len, y / len, z / len);
}

double Point::dot(const Point& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y, z + other.z);
}

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y, z - other.z);
}

Point Point::operator*(double scalar) const {
    return Point(x * scalar, y * scalar, z * scalar);
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "Point(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
}