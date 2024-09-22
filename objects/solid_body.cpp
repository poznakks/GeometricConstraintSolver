//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "solid_body.h"

SolidBody::SolidBody(const Point& position, const double angleX, const double angleY, const double angleZ)
    : position(position), angleX(angleX), angleY(angleY), angleZ(angleZ) {}

void SolidBody::move(const double dx, const double dy, const double dz) {
    position.move(dx, dy, dz);
}

void SolidBody::rotate(const double dAngleX, const double dAngleY, const double dAngleZ) {
    angleX += dAngleX;
    angleY += dAngleY;
    angleZ += dAngleZ;
}

double SolidBody::distanceTo(const SolidBody& other) const {
    return position.distanceTo(other.position);
}

std::ostream& operator<<(std::ostream& os, const SolidBody& body) {
    os << "SolidBody(Position: " << body.position << ", Rotation angles: ("
       << body.angleX << ", " << body.angleY << ", " << body.angleZ << "))";
    return os;
}