/*
 * This source file is part of raytracer
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
        return std::numeric_limits<Real>::infinity();
    }

    if (t_sol.x1 < 0) {
        if (t_sol.x2 < 0) {
            return std::numeric_limits<Real>::infinity();
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

/* Plane */
Plane::Plane(Color color, Vec3D position, Vec3D normal) : Object3D(color),
    position(position), normal(normal) { }

Plane::~Plane() { }

Color Plane::getColor() {
    return color;
}

Real Plane::intersect(Ray& ray) {
    return intersectPlane(ray.getOrigin(), ray.getDirection(), position, normal);
}