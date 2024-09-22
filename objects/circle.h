//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef CIRCLE_H
#define CIRCLE_H
#include "point.h"

class Circle final {
public:
    Point center;     // Центр окружности
    double radius;    // Радиус окружности
    Point normal;     // Нормальный вектор для 3D

    Circle(const Point& center, double radius, const Point& normal);

    // Расстояние от точки до окружности
    double distanceTo(const Point& p) const;

    // Расстояние между двумя окружностями
    double distanceTo(const Circle& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Circle& circle);
};


#endif //CIRCLE_H
