//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "point.h"

class Ellipse final {
public:
    Point center;         // Центр эллипса
    double semiAxisA;     // Полуось A
    double semiAxisB;     // Полуось B
    Point normal;         // Ориентация (нормальный вектор для плоскости эллипса)

    Ellipse(const Point& center, double semiAxisA, double semiAxisB, const Point& normal);

    // Расстояние от точки до эллипса
    double distanceTo(const Point& p) const;

    friend std::ostream& operator<<(std::ostream& os, const Ellipse& ellipse);
};

#endif //ELLIPSE_H
