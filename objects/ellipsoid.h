//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef ELLIPSOID_H
#define ELLIPSOID_H
#include "point.h"

class Ellipsoid final {
public:
    Point center;        // Центр эллипсоида
    double semiAxisA;    // Полуось A
    double semiAxisB;    // Полуось B
    double semiAxisC;    // Полуось C
    Point orientation;   // Ориентация эллипсоида в 3D

    Ellipsoid(const Point& center, double semiAxisA, double semiAxisB, double semiAxisC, const Point& orientation);

    // Расстояние от точки до эллипсоида (приближенно)
    double distanceTo(const Point& p) const;

    friend std::ostream& operator<<(std::ostream& os, const Ellipsoid& ellipsoid);
};

#endif //ELLIPSOID_H
