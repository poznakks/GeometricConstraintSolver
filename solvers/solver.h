//
// Created by Vlad Boguzh on 14.05.2025.
//

#ifndef SOLVER_H
#define SOLVER_H

#include "../types.h"

class Solver {
public:
    virtual ~Solver() = default;

    virtual void AddObject(const ObjectSharedPtr& object) = 0;
    virtual void AddConstraint(const ConstraintSharedPtr& constraint) = 0;
    virtual void ApplyConstraintsFrom(const ObjectSharedPtr& start) = 0;

    template <typename T>
    VectorSharedPtr<T> GetObjectsOfType() const {
        VectorSharedPtr<T> result;
        for (const auto& obj : GetAllObjects()) {
            if (auto casted = std::dynamic_pointer_cast<T>(obj)) {
                result.push_back(casted);
            }
        }
        return result;
    }

protected:
    virtual std::vector<ObjectSharedPtr> GetAllObjects() const = 0;
};

#endif //SOLVER_H
