//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "line.h"

Line::Line(const Point& point, const Point& direction)
    : point(point), direction(direction) {}

// Прямая через две точки
Line Line::fromTwoPoints(const Point& p1, const Point& p2) {
    const Point direction(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    return {p1, direction};
}

// Расстояние от точки до прямой
double Line::distanceTo(const Point& p) const {
    const Point p0p(p.x - point.x, p.y - point.y, p.z - point.z);

    // Векторное произведение для расчета расстояния
    const Point crossProduct(
        p0p.y * direction.z - p0p.z * direction.y,
        p0p.z * direction.x - p0p.x * direction.z,
        p0p.x * direction.y - p0p.y * direction.x
    );

    const double numerator = std::sqrt(crossProduct.x * crossProduct.x +
                                 crossProduct.y * crossProduct.y +
                                 crossProduct.z * crossProduct.z);
    const double denominator = std::sqrt(direction.x * direction.x +
                                   direction.y * direction.y +
                                   direction.z * direction.z);

    return numerator / denominator;
}

// Расстояние между двумя прямыми
double Line::distanceTo(const Line& other) const {
    const Point n(
        direction.y * other.direction.z - direction.z * other.direction.y,
        direction.z * other.direction.x - direction.x * other.direction.z,
        direction.x * other.direction.y - direction.y * other.direction.x
    );

    // Если векторное произведение равно нулю, прямые параллельны
    if (n.x == 0 && n.y == 0 && n.z == 0) {
        return distanceTo(other.point);
    }

    const Point p0p1(other.point.x - point.x, other.point.y - point.y, other.point.z - point.z);
    return std::abs(p0p1.x * n.x + p0p1.y * n.y + p0p1.z * n.z) / std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
}

// Проверка на параллельность двух прямых
bool Line::isParallelTo(const Line& other) const {
    // Векторное произведение направляющих векторов
    const Point crossProduct(
        direction.y * other.direction.z - direction.z * other.direction.y,
        direction.z * other.direction.x - direction.x * other.direction.z,
        direction.x * other.direction.y - direction.y * other.direction.x
    );

    // Если результат векторного произведения равен нулю, то прямые параллельны
    return crossProduct.x == 0 && crossProduct.y == 0 && crossProduct.z == 0;
}

// Реализация проверки принадлежности точки прямой
bool Line::contains(const Point& p) const {
    const Point p0p(p.x - point.x, p.y - point.y, p.z - point.z);

    const double crossProductMagnitude = std::sqrt(
        (p0p.y * direction.z - p0p.z * direction.y) * (p0p.y * direction.z - p0p.z * direction.y) +
        (p0p.z * direction.x - p0p.x * direction.z) * (p0p.z * direction.x - p0p.x * direction.z) +
        (p0p.x * direction.y - p0p.y * direction.x) * (p0p.x * direction.y - p0p.y * direction.x)
    );

    return crossProductMagnitude == 0;
}

std::ostream& operator<<(std::ostream& os, const Line& line) {
    os << "Line(Point: (" << line.point.x << ", " << line.point.y << ", " << line.point.z << ")"
       << ", Direction: (" << line.direction.x << ", " << line.direction.y << ", " << line.direction.z << "))";
    return os;
}