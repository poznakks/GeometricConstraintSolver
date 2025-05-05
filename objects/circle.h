//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef CIRCLE_H
#define CIRCLE_H
#include "line.h"
#include "point.h"

class Circle final: public GeometricObject {
public:
    Circle(const Point& center, double radius, const Point& normal = Point());

    bool isPointOnCircle(const Point& point) const;
    bool isLineTangent(const Line& line) const;

    Point center;
    double radius;
    Point normal; // единичный вектор, нормаль к плоскости круга
};

#endif //CIRCLE_H
