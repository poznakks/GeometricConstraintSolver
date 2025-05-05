//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef PLANE_H
#define PLANE_H
#include "line.h"
#include "point.h"

class Plane final: public GeometricObject {
public:
    Point center;        // Точка на плоскости (центр)
    Point normal;        // Нормальный вектор

    Plane(const Point& center, const Point& normal);

    // Расстояние от точки до плоскости
    double distanceTo(const Point& p) const;

    // Расстояние между двумя плоскостями
    double distanceTo(const Plane& other) const;

    // Проверка на параллельность с прямой
    bool isParallelTo(const Line& line) const;

    // Проверка, параллельна ли плоскость другой плоскости
    bool isParallelTo(const Plane& other) const;

    // Проверка, принадлежит ли точка плоскости
    bool contains(const Point& p) const;

    // Проверка, пересекает ли прямая плоскость
    bool contains(const Line& line) const;

    friend std::ostream& operator<<(std::ostream& os, const Plane& plane);
};

#endif //PLANE_H
