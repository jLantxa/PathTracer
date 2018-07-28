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
#include "Vector3D.hpp"
#include "Utils.hpp"
#include "Light.hpp"

#include <algorithm>
#include <limits>

/* Object3D */
Object3D::Object3D(Vec3D color) : color(color) { }

Object3D::~Object3D() { }


/* Sphere */
Sphere::Sphere(Vec3D color, Vec3D center_V, Real radius) : Object3D(color),
    mCenter_v(center_V), radius(radius) { }

Sphere::~Sphere() { }

Vec3D Sphere::getColor() {
    return color;
}

Real Sphere::intersect(Ray& ray) {
    Vec3D dir_v = ray.getDirection();
    Vec3D oc_v = ray.getOrigin() - mCenter_v;
    Real a = dir_v.dot(dir_v);
    Real b = 2 * dir_v.dot(oc_v);
    Real c = oc_v.dot(oc_v) - radius*radius;

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

Vec3D Sphere::getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    Vec3D surfNormal = getSurfaceNormal(hitPoint_v, hitDirection_v);
    if (hitDirection_v.dot(surfNormal) < 0) {
        return surfNormal;
    } else {
        return surfNormal.negative();
    }
}

Vec3D Sphere::getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    return hitPoint_v - mCenter_v;
}

/* Plane */
Plane::Plane(Vec3D color, Vec3D position_v, Vec3D normal_v) : Object3D(color),
    mPosition_v(position_v), mNormal_v(normal_v.normalize()) { }

Plane::~Plane() { }

Vec3D Plane::getColor() {
    return color;
}

Real Plane::intersect(Ray& ray) {
    return intersectPlane(ray.getOrigin(), ray.getDirection(), mPosition_v, mNormal_v);
}

Vec3D Plane::getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    if (hitDirection_v.dot(mNormal_v) < 0) {
        return mNormal_v;
    } else {
        return mNormal_v.negative();
    }
}

Vec3D Plane::getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    return mNormal_v;
}

/* Triangle
 * Surface normal depends on the order of (A, B, C)
*/
Triangle::Triangle(Vec3D color, Vec3D A_v, Vec3D B_v, Vec3D C_v) : Object3D(color),
    mA_v(A_v), mB_v(B_v), mC_v(C_v)
{
    Vec3D AC_v = mC_v - mA_v;
    Vec3D AB_v = mB_v - mA_v;
    mNormal_v = AC_v.cross(AB_v).normalize();
}

Triangle::~Triangle() { }

Vec3D Triangle::getColor() {
    return color;
}

Real Triangle::intersect(Ray& ray) {
    Vec3D rayOrigin_v = ray.getOrigin();
    Vec3D rayDirection_v = ray.getDirection();

    Real tPlane = intersectPlane(rayOrigin_v, rayDirection_v, mA_v, mNormal_v);
    if (tPlane < 0 || tPlane == std::numeric_limits<Real>::infinity()) {
        // No intersection with containing plane
        return -std::numeric_limits<Real>::infinity();
    }

    // Test intersection inside triangle
    Vec3D Q_v = ray.point(tPlane);

    // [CA x QA]*n >= 0
    Vec3D CA_v = mC_v - mA_v;
    Vec3D QA_v = Q_v - mA_v;
    Real testA = CA_v.cross(QA_v).dot(mNormal_v);
    // [AB x QB]*n >= 0
    Vec3D AB_v = mA_v - mB_v;
    Vec3D QB_v = Q_v - mB_v;
    Real testB = AB_v.cross(QB_v).dot(mNormal_v);
    // [BC x QC]*n >= 0
    Vec3D BC_v = mB_v - mC_v;
    Vec3D QC_v = Q_v - mC_v;
    Real testC = BC_v.cross(QC_v).dot(mNormal_v);

    if (testA >= 0 && testB >= 0 && testC >= 0) {
        // Inside triangle
        return tPlane;
    } else {
        // Outside triangle
        return -std::numeric_limits<Real>::infinity();
    }
}

Vec3D Triangle::getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    if (hitDirection_v.dot(mNormal_v) < 0) {
        return mNormal_v;
    } else {
        return mNormal_v.negative();
    }
}

Vec3D Triangle::getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    return mNormal_v;
}