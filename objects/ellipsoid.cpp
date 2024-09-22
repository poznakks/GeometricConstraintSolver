//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "ellipsoid.h"

Ellipsoid::Ellipsoid(const Point& center, double semiAxisA, double semiAxisB, double semiAxisC, const Point& orientation)
    : center(center), semiAxisA(semiAxisA), semiAxisB(semiAxisB), semiAxisC(semiAxisC), orientation(orientation) {}

// Расстояние от точки до эллипсоида (приближенно)
double Ellipsoid::distanceTo(const Point& p) const {
    const Point pCentered(p.x - center.x, p.y - center.y, p.z - center.z);
    const double dist = std::sqrt(
        (pCentered.x * pCentered.x) / (semiAxisA * semiAxisA) +
        (pCentered.y * pCentered.y) / (semiAxisB * semiAxisB) +
        (pCentered.z * pCentered.z) / (semiAxisC * semiAxisC)
    );
    return std::abs(dist - 1.0);
}

std::ostream& operator<<(std::ostream& os, const Ellipsoid& ellipsoid) {
    os << "Ellipsoid(Center: " << ellipsoid.center << ", Semi-Axis A: " << ellipsoid.semiAxisA
       << ", Semi-Axis B: " << ellipsoid.semiAxisB << ", Semi-Axis C: " << ellipsoid.semiAxisC
       << ", Orientation: " << ellipsoid.orientation << ")";
    return os;
}