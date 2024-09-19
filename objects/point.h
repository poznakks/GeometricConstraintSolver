//
// Created by Vlad Boguzh on 17.09.2024.
//

#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
    double x, y, z;
    explicit Point(double x = 0, double y = 0, double z = 0);
    double distanceTo(const Point& other) const;
    void move(double dx, double dy, double dz = 0);
    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

#endif //POINT_H