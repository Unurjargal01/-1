#pragma once

#include <triangle.h>
#include <material.h>
#include <sphere.h>
#include <vector.h>

struct Object {
    const Material* material = nullptr;
    std::string name;
    Triangle polygon;
    Triangle norm_polygon;
    const Vector* GetNormal(size_t index) const {
        return &norm_polygon[index];
    };
};

struct SphereObject {
    const Material* material = nullptr;
    std::string name;
    Sphere sphere;
};
