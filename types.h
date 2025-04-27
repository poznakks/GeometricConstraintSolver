//
// Created by Vlad Boguzh on 19.04.2025.
//

#ifndef TYPES_H
#define TYPES_H

#include "constraints/constraint.h"
#include "objects/circle.h"
#include "objects/point.h"
#include "objects/line.h"

using PointSharedPtr = std::shared_ptr<Point>;
using LineSharedPtr  = std::shared_ptr<Line>;
using CircleSharedPtr  = std::shared_ptr<Circle>;
using ConstraintUniquePtr  = std::unique_ptr<Constraint>;

using VectorPointSharedPtr = std::vector<std::shared_ptr<Point>>;
using VectorLineSharedPtr = std::vector<std::shared_ptr<Line>>;
using VectorCircleSharedPtr = std::vector<std::shared_ptr<Circle>>;
using VectorConstraintUniquePtr = std::vector<std::unique_ptr<Constraint>>;

#endif //TYPES_H
