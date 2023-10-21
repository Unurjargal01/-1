#pragma once

#include <vector.h>

class Intersection {
public:
    Intersection() {
        position_ = Vector();
        normal_ = Vector();
        distance_ = 0;
    }
    Intersection(const Vector& position, const Vector& normal, double distance)
        : position_(position), normal_(normal), distance_(distance){};

    const Vector& GetPosition() const {
        return position_;
    };
    const Vector& GetNormal() const {
        return normal_;
    };
    double GetDistance() const {
        return distance_;
    };

private:
    Vector position_, normal_;
    double distance_;
};
