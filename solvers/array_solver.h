//
// Created by Vlad Boguzh on 14.05.2025.
//

#ifndef ARRAY_SOLVER_H
#define ARRAY_SOLVER_H

#include "solver.h"

class ArraySolver final : public Solver {
public:
    void AddObject(const ObjectSharedPtr& object) override;
    void AddConstraint(const ConstraintSharedPtr& constraint) override;
    void ApplyConstraintsFrom(const ObjectSharedPtr& start) override;

protected:
    std::vector<ObjectSharedPtr> GetAllObjects() const override;

private:
    std::vector<ObjectSharedPtr> objects;
    std::vector<ConstraintSharedPtr> constraints;
};

#endif //ARRAY_SOLVER_H
