//
// Created by Vlad Boguzh on 14.05.2025.
//

#ifndef GRAPH_SOLVER_H
#define GRAPH_SOLVER_H
#include <unordered_set>

#include "solver.h"

struct ConstraintEdge {
    ObjectSharedPtr object;
    ConstraintSharedPtr constraint;
};
using Graph = std::unordered_map<ObjectSharedPtr, std::vector<ConstraintEdge>>;

class GraphSolver final : public Solver {
public:
    void AddObject(const ObjectSharedPtr& object) override;
    void AddConstraint(const ConstraintSharedPtr& constraint) override;
    void ApplyConstraintsFrom(const ObjectSharedPtr& start) override;

protected:
    std::vector<ObjectSharedPtr> GetAllObjects() const override;

private:
    Graph graph;
    void DFS(const ObjectSharedPtr& current, std::unordered_set<ObjectSharedPtr>& visited) const;
};

#endif //GRAPH_SOLVER_H
