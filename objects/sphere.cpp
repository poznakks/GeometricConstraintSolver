//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "sphere.h"

Sphere::Sphere(const Point& center, const double radius) : center(center), radius(radius) {}

// Функция вычисления расстояния от точки до поверхности сферы
double Sphere::distanceTo(const Point& p) const {
    const double centerDistance = center.distanceTo(p);
    return std::fabs(centerDistance - radius);
}

std::ostream& operator<<(std::ostream& os, const Sphere& sphere) {
    os << "Sphere(Center: " << sphere.center << ", Radius: " << sphere.radius << ")";
    return os;
}