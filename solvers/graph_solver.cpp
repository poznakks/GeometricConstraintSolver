//
// Created by Vlad Boguzh on 14.05.2025.
//

#include "graph_solver.h"

void GraphSolver::AddObject(const ObjectSharedPtr& object) {
    graph[object] = {};
}

void GraphSolver::AddConstraint(const ConstraintSharedPtr& constraint) {
    auto a = constraint->getObjectA();
    auto b = constraint->getObjectB();
    graph[a].push_back({b, constraint});
    graph[b].push_back({a, constraint});
    a->setDirty(true);
    b->setDirty(true);
    ApplyConstraintsFrom(a);
}

void GraphSolver::ApplyConstraintsFrom(const ObjectSharedPtr& start) {
    std::unordered_set<ObjectSharedPtr> visited;
    DFS(start, visited);
}

std::vector<ObjectSharedPtr> GraphSolver::GetAllObjects() const {
    std::vector<ObjectSharedPtr> result;
    for (const auto& [obj, _] : graph) {
        result.push_back(obj);
    }
    return result;
}

void GraphSolver::DFS(const ObjectSharedPtr& current, std::unordered_set<ObjectSharedPtr>& visited) const {
    if (!current->isDirty() || visited.contains(current)) return;
    visited.insert(current);
    const auto& edges = graph.at(current);
    for (const auto& edge : edges) {
        edge.constraint->apply();
        DFS(edge.object, visited);
    }
    current->setDirty(false);
}