#include "constraints/distance_constraint.h"
#include "constraints/horizontal_constraint.h"
#include "constraints/parallel_constraint.h"
#include "constraints/vertical_constraint.h"
#include "objects/line.h"
#include "objects/point.h"

using namespace std;

void checkHorizontalConstraint() {
    Point p1(2, 5);
    Point p2(4, 5);
    HorizontalConstraint hc(p1, p2);
    cout << "Before moving p1" << endl;
    cout << p1 << " " << p2 << endl;
    p1.move(-3, 2);
    hc.apply();
    cout << "After moving p1" << endl;
    cout << p1 << " " << p2 << endl;
}

void checkVerticalConstraint() {
    Point p1(2, 5);
    Point p2(4, 5);
    VerticalConstraint vc(p1, p2);
    cout << "Before moving p1" << endl;
    cout << p1 << " " << p2 << endl;
    p1.move(-3, 2);
    vc.apply();
    cout << "After moving p1" << endl;
    cout << p1 << " " << p2 << endl;
}

void checkDistanceConstraint() {
    Point p1;
    Point p2(5, 5);
    DistanceConstraint dc(p1, p2, 5);
    dc.apply();
    cout << "Before moving p1" << endl;
    cout << p1 << " " << p2 << endl;
    p1.move(-1, 0);
    dc.apply();
    cout << "After moving p1" << endl;
    cout << p1 << " " << p2 << endl;
}

void checkParallelConstraint() {
    Point p1(1, 1);
    Point p2(2, 2); // Направляющий вектор для первой прямой
    Line l1(p1, p2);

    Point p3(3, 3);
    Point p4(4, 5); // Направляющий вектор для второй прямой
    Line l2(p3, p4);

    ParallelConstraint pc(l1, l2);

    cout << "Before applying parallel constraint" << endl;
    cout << l1 << endl;
    cout << l2 << endl;

    pc.apply();

    cout << "After applying parallel constraint" << endl;
    cout << l1 << endl;
    cout << l2 << endl;
}

int main() {
    checkHorizontalConstraint();
    // checkVerticalConstraint();
    // checkDistanceConstraint();
    // checkParallelConstraint();
    return 0;
}
