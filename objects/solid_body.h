//
// Created by Vlad Boguzh on 22.09.2024.
//

#ifndef SOLID_BODY_H
#define SOLID_BODY_H
#include "point.h"

class SolidBody final {
public:
    Point position;      // Положение центра
    double angleX, angleY, angleZ; // Углы поворота по осям X, Y, Z

    explicit SolidBody(const Point& position = Point(), double angleX = 0, double angleY = 0, double angleZ = 0);

    // Смещение тела на заданные векторы
    void move(double dx, double dy, double dz);

    // Поворот тела вокруг каждой оси
    void rotate(double dAngleX, double dAngleY, double dAngleZ);

    // Расстояние до другого твердого тела
    double distanceTo(const SolidBody& other) const;

    // Вывод информации о твердом теле
    friend std::ostream& operator<<(std::ostream& os, const SolidBody& body);
};

#endif //SOLID_BODY_H
