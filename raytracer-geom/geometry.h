#pragma once

#include <vector.h>
#include <sphere.h>
#include <intersection.h>
#include <triangle.h>
#include <ray.h>
#include <cmath>
#include <optional>

auto SolveQuad(const double& a, const double& b, const double& c) {
    double disc = b * b - 4 * a * c;
    double x0, x1;
    if (disc == 0) {
        x0 = x1 = -b / (2 * a);
    } else {
        double q = (b > 0) ? -(b + std::sqrt(disc)) / 2 : -(b - std::sqrt(disc)) / 2;
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
    }
    if (x0 > 0) {
        return x0;
    }
    return (x1 > 0) ? x1 : -1;
}
bool InsideCircle(const Vector& v, const Sphere& s) {
    return (Length(s.GetCenter() - v) <= s.GetRadius());
}
std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) {
    Vector oc = ray.GetOrigin() - sphere.GetCenter();
    double b = 2 * DotProduct(ray.GetDirection(), oc);
    double c = DotProduct(oc, oc) - sphere.GetRadius() * sphere.GetRadius();
    double t0 = SolveQuad(1, b, c);
    if (t0 < 0) {
        return std::nullopt;
    }
    Vector pos = ray.GetOrigin() + t0 * ray.GetDirection();
    // Check inside or outside;
    Vector norm = pos - sphere.GetCenter();
    if (InsideCircle(ray.GetOrigin(), sphere)) {
        norm *= -1;
    }
    norm.Normalize();
    return Intersection{pos, norm, std::abs(t0)};
};
std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) {
    const double eps = 0.000001;
    Vector v0 = triangle[0], v1 = triangle[1], v2 = triangle[2];
    Vector edge1, edge2, h, s, q;
    double a, f, u, v;
    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = CrossProduct(ray.GetDirection(), edge2);
    a = DotProduct(edge1, h);

    if (a > -eps && a < eps) {
        return std::nullopt;
    }
    f = 1.0 / a;
    s = ray.GetOrigin() - v0;
    u = f * DotProduct(s, h);

    if (u < 0.0 || u > 1.0) {
        return std::nullopt;
    }
    q = CrossProduct(s, edge1);
    v = f * DotProduct(ray.GetDirection(), q);

    if (v < 0.0 || u + v > 1.0) {
        return std::nullopt;
    }

    double t = f * DotProduct(edge2, q);

    Vector norm = CrossProduct(edge1, edge2);
    norm.Normalize();
    if (DotProduct(norm, ray.GetDirection()) > 0) {
        norm *= -1;
    }
    if (t > eps) {
        // here possibility is that normal might have wrong direction;
        return Intersection{ray.GetOrigin() + ray.GetDirection() * t, norm, t};
    }
    return std::nullopt;
};
Vector Reflect(const Vector& ray, const Vector& normal) {
    Vector ans = 2 * (DotProduct(normal, ray)) * normal - ray;
    return -1 * ans;
};
std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) {
    // eta > 1;
    double c = -DotProduct(normal, ray);
    double int_ref = 1 - eta * eta * (1 - c * c);
    if (int_ref < 0) {
        return std::nullopt;
    }
    Vector ans = eta * ray + (eta * c - std::sqrt(int_ref)) * normal;
    return ans;
};

Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) {
    Vector v0 = triangle[0], v1 = triangle[1], v2 = triangle[2];
    double area = Length(CrossProduct(v1 - v0, v2 - v0));
    double a = Length(CrossProduct(v1 - v2, v1 - point)) / area;
    double b = Length(CrossProduct(v0 - v2, v2 - point)) / area;
    double c = Length(CrossProduct(v1 - v0, v0 - point)) / area;

    if (a >= 0 && b >= 0 && 1 >= a + b) {
        return Vector{a, b, c};
    }
    return Vector();
}