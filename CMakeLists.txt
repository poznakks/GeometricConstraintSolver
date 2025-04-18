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
        objects/point.cpp
        objects/point.h
        constraints/constraint.h
        constraints/p2p_horizontal_constraint.cpp
        constraints/p2p_horizontal_constraint.h
        constraints/p2p_vertical_constraint.cpp
        constraints/p2p_vertical_constraint.h
        objects/line.cpp
        objects/line.h
        constraints/parallel_constraint.cpp
        constraints/parallel_constraint.h
        constraints/p2p_distance_constraint.cpp
        constraints/p2p_distance_constraint.h
        objects/plane.cpp
        objects/plane.h
        objects/circle.cpp
        objects/circle.h
        objects/ellipse.cpp
        objects/ellipse.h
        objects/ellipsoid.cpp
        objects/ellipsoid.h
        objects/sphere.cpp
        objects/sphere.h
        objects/solid_body.cpp
        objects/solid_body.h
        constraints/p2l_distance_constraint.cpp
        constraints/p2l_distance_constraint.h
)

# Link wxWidgets libraries
target_link_libraries(GeometricConstraintSolver ${wxWidgets_LIBRARIES})