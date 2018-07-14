/*
 * This source file is part of PathTracer
 *
 * Copyright 2018 Javier Lancha Vázquez
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "Objects.hpp"

#include "Common.hpp"
#include "Geometry.hpp"
#include "Utils.hpp"
#include "Light.hpp"

#include <algorithm>
#include <limits>

/* Object3D */
Object3D::Object3D(Color color) : color(color) { }

Object3D::~Object3D() { }


/* Sphere */
Sphere::Sphere(Color color, Vec3D center, Real radius) : Object3D(color),
    center(center), radius(radius) { }

Sphere::~Sphere() { }

Color Sphere::getColor() {
    return color;
}

Real Sphere::intersect(Ray& ray) {
    Vec3D dir = ray.getDirection();
    Vec3D oc = ray.getOrigin() - center;
    Real a = dir.dot(dir);
    Real b = 2 * dir.dot(oc);
    Real c = oc.dot(oc) - radius*radius;

    Deg2Solution t_sol;
    solveDeg2(a, b, c, t_sol);
    if (!t_sol.valid) {
        return -std::numeric_limits<Real>::infinity();
    }

    if (t_sol.x1 < 0) {
        if (t_sol.x2 < 0) {
            return -std::numeric_limits<Real>::infinity();
        } else {
            return t_sol.x2;
        }
    } else {
        if (t_sol.x2 < 0) {
            return t_sol.x1;
        } else {
            return std::min(t_sol.x1, t_sol.x2);
        }
    }
}

Vec3D Sphere::getNormal(Vec3D& hitPoint, Vec3D& hitDirection) {
    return hitPoint - center;
}

/* Plane */
Plane::Plane(Color color, Vec3D position, Vec3D normal) : Object3D(color),
    position(position), normal(normal.normalize()) { }

Plane::~Plane() { }

Color Plane::getColor() {
    return color;
}

Real Plane::intersect(Ray& ray) {
    return intersectPlane(ray.getOrigin(), ray.getDirection(), position, normal);
}

Vec3D Plane::getNormal(Vec3D& hitPoint, Vec3D& hitDirection) {
    if (hitDirection.dot(normal) < 0) {
        return normal;
    } else {
        return normal.negative();
    }
}

/* Triangle */
Triangle::Triangle(Color color, Vec3D pA, Vec3D pB, Vec3D pC) : Object3D(color),
    A(pA), B(pB), C(pC)
{
    Vec3D AC = C - A;
    Vec3D AB = B - A;
    normal = AC.cross(AB).normalize();
}

Triangle::~Triangle() { }

Color Triangle::getColor() {
    return color;
}

Real Triangle::intersect(Ray& ray) {
    Vec3D rayOrigin = ray.getOrigin();
    Vec3D rayDirection = ray.getDirection();

    Real tPlane = intersectPlane(rayOrigin, rayDirection, A, normal);
    if (tPlane < 0 || tPlane == std::numeric_limits<Real>::infinity()) {
        // No intersection with containing plane
        return -std::numeric_limits<Real>::infinity();
    }

    // Test intersection inside triangle
    Vec3D Q = ray.point(tPlane);

    // [CA x QA]*n >= 0
    Vec3D CA = C - A;
    Vec3D QA = Q - A;
    Real testA = CA.cross(QA).dot(normal);
    // [AB x QB]*n >= 0
    Vec3D AB = A - B;
    Vec3D QB = Q - B;
    Real testB = AB.cross(QB).dot(normal);
    // [BC x QC]*n >= 0
    Vec3D BC = B - C;
    Vec3D QC = Q - C;
    Real testC = BC.cross(QC).dot(normal);

    if (testA >= 0 && testB >= 0 && testC >= 0) {
        // Inside triangle
        return tPlane;
    } else {
        // Outside triangle
        return -std::numeric_limits<Real>::infinity();
    }
}

Vec3D Triangle::getNormal(Vec3D& hitPoint, Vec3D& hitDirection) {
    if (hitDirection.dot(normal) > 0) {
        return normal;
    } else {
        return normal.negative();
    }
}