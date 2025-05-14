//
// Created by Vlad Boguzh on 14.05.2025.
//

#include "array_solver.h"

void ArraySolver::AddObject(const ObjectSharedPtr& object) {
    objects.push_back(object);
}

void ArraySolver::AddConstraint(const ConstraintSharedPtr& constraint) {
    constraints.push_back(constraint);
}

void ArraySolver::ApplyConstraintsFrom(const ObjectSharedPtr& start) {
    for (const auto& constraint : constraints) {
        constraint->apply();
    }
}

std::vector<ObjectSharedPtr> ArraySolver::GetAllObjects() const {
    return objects;
}