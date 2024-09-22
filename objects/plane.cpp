//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "plane.h"

Plane::Plane(const Point& center, const Point& normal)
    : center(center), normal(normal) {}

// Расстояние от точки до плоскости
double Plane::distanceTo(const Point& p) const {
    const Point p0p(p.x - center.x, p.y - center.y, p.z - center.z);
    const double dotProduct = p0p.x * normal.x + p0p.y * normal.y + p0p.z * normal.z;
    const double norm = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    return std::abs(dotProduct) / norm;
}

// Расстояние между двумя плоскостями
double Plane::distanceTo(const Plane& other) const {
    // Если плоскости не параллельны, расстояние равно 0
    if (!isParallelTo(other)) {
        return 0.0;
    }

    // Плоскости параллельны, вычисляем расстояние между ними
    return distanceTo(other.center);
}

// Реализация проверки параллельности прямой и плоскости
bool Plane::isParallelTo(const Line& line) const {
    const Point crossProduct(
        normal.y * line.direction.z - normal.z * line.direction.y,
        normal.z * line.direction.x - normal.x * line.direction.z,
        normal.x * line.direction.y - normal.y * line.direction.x
    );
    return crossProduct.x == 0 && crossProduct.y == 0 && crossProduct.z == 0;
}

// Проверка на параллельность двух плоскостей
bool Plane::isParallelTo(const Plane& other) const {
    const Point crossProduct(
        normal.y * other.normal.z - normal.z * other.normal.y,
        normal.z * other.normal.x - normal.x * other.normal.z,
        normal.x * other.normal.y - normal.y * other.normal.x
    );
    return crossProduct.x == 0 && crossProduct.y == 0 && crossProduct.z == 0;
}

// Реализация проверки принадлежности точки плоскости
bool Plane::contains(const Point& p) const {
    return std::abs(distanceTo(p)) < 1e-9; // Используем небольшой допуск для проверки
}

// Реализация проверки пересечения прямой и плоскости
bool Plane::contains(const Line& line) const {
    return isParallelTo(line) && std::abs(distanceTo(line.point)) < 1e-9; // Прямая должна пересекать плоскость
}

std::ostream& operator<<(std::ostream& os, const Plane& plane) {
    os << "Plane(Center: " << plane.center << ", Normal: " << plane.normal << ")";
    return os;
}