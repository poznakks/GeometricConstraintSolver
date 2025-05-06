//
// Created by Vlad Boguzh on 05.05.2025.
//

#ifndef GEOMETRIC_OBJECT_H
#define GEOMETRIC_OBJECT_H

class GeometricObject {
public:
    virtual ~GeometricObject() = default;
    bool isDirty() const { return dirty; }
    void setDirty(const bool d) { dirty = d; }

protected:
    bool dirty = false;
};

#endif //GEOMETRIC_OBJECT_H
