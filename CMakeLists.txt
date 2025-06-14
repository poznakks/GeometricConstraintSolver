cmake_minimum_required(VERSION 3.29)
project(GeometricConstraintSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск WxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

add_executable(GeometricConstraintSolver main.cpp
        objects/plane.cpp
        objects/plane.h
        objects/point.cpp
        objects/point.h
        constraints/constraint.h
        constraints/p2p_horizontal_constraint.cpp
        constraints/p2p_horizontal_constraint.h
        constraints/p2p_vertical_constraint.cpp
        constraints/p2p_vertical_constraint.h
        objects/line.cpp
        objects/line.h
        constraints/l2l_parallel_constraint.cpp
        constraints/l2l_parallel_constraint.h
        constraints/p2p_distance_constraint.cpp
        constraints/p2p_distance_constraint.h
        objects/circle.cpp
        objects/circle.h
        constraints/p2l_distance_constraint.cpp
        constraints/p2l_distance_constraint.h
        types.h
        constraints/l2l_distance_constraint.cpp
        constraints/l2l_distance_constraint.h
        constraints/l2l_perpendicular_constraint.cpp
        constraints/l2l_perpendicular_constraint.h
        objects/geometric_object.h
        solvers/solver.h
        solvers/graph_solver.cpp
        solvers/graph_solver.h
        solvers/array_solver.cpp
        solvers/array_solver.h
        constraints/c2l_distance_constraint.cpp
        constraints/c2l_distance_constraint.h
        constraints/c2p_distance_constraint.cpp
        constraints/c2p_distance_constraint.h
)

# Link wxWidgets libraries
target_link_libraries(GeometricConstraintSolver ${wxWidgets_LIBRARIES})