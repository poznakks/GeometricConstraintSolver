//
// Created by Vlad Boguzh on 19.04.2025.
//

#ifndef TYPES_H
#define TYPES_H

#include "constraints/constraint.h"
#include "objects/circle.h"
#include "objects/point.h"
#include "objects/line.h"


using ObjectSharedPtr = std::shared_ptr<GeometricObject>;
using PointSharedPtr = std::shared_ptr<Point>;
using LineSharedPtr  = std::shared_ptr<Line>;
using CircleSharedPtr  = std::shared_ptr<Circle>;
using ConstraintSharedPtr  = std::shared_ptr<Constraint>;

using VectorObjectSharedPtr = std::vector<ObjectSharedPtr>;
using VectorPointSharedPtr = std::vector<std::shared_ptr<Point>>;
using VectorLineSharedPtr = std::vector<std::shared_ptr<Line>>;
using VectorCircleSharedPtr = std::vector<std::shared_ptr<Circle>>;
using VectorConstraintSharedPtr = std::vector<ConstraintSharedPtr>;

#endif //TYPES_H
