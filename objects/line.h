//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef LINE_H
#define LINE_H
#include "point.h"

class Line {
public:
    Point point;     // Точка на прямой
    Point direction; // Направляющий вектор

    Line(const Point& point, const Point& direction);

    // Альтернативный способ задания через две точки
    static Line fromTwoPoints(const Point& p1, const Point& p2);

    // Получение расстояния от точки до прямой
    double distanceTo(const Point& p) const;

    // Получение расстояния между двумя прямыми
    double distanceTo(const Line& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Line& line);
};


#endif //LINE_H
