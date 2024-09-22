//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "point.h"

class Sphere final {
public:
    Point center;              // Центр сферы
    const double radius;       // Радиус сферы

    Sphere(const Point& center, double radius);
    double distanceTo(const Point& p) const; // Функция вычисления расстояния
    friend std::ostream& operator<<(std::ostream& os, const Sphere& sphere);
};

#endif //SPHERE_H
