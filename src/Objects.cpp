/*
 * This source file is part of PathTracer
 *
 * Copyright 2018, 2019 Javier Lancha Vázquez
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

/* IObject3D */
IObject3D::IObject3D(struct Material material) : mMaterial(material) { }

IObject3D::~IObject3D() { }

Color& IObject3D::color() {
    return mMaterial.color;
}

struct Material& IObject3D::material() {
    return mMaterial;
}


/* Plane */
Plane::Plane(struct Material material, Vec3D position_v, Vec3D normal_v)
:   IObject3D(material),
    mPosition_v(position_v), mNormal_v(normal_v.normalize()) { }

Plane::~Plane() { }

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

// TODO: This is a placeholder
struct Enclosure Plane::getEnclosure() {
    const Real nInf = -infinity<Real>();
    const Real inf = infinity<Real>();
    return Enclosure {
        nInf, inf,
        nInf, inf,
        nInf, inf,
    };
}


/* Triangle
 * Surface normal depends on the order of (A, B, C)
*/
Triangle::Triangle(struct Material material, Vec3D A_v, Vec3D B_v, Vec3D C_v)
:   IObject3D(material),
    mA_v(A_v), mB_v(B_v), mC_v(C_v)
{
    Vec3D AC_v = mC_v - mA_v;
    Vec3D AB_v = mB_v - mA_v;
    mNormal_v = AC_v.cross(AB_v).normalize();
}

Triangle::~Triangle() { }

Real Triangle::intersect(Ray& ray) {
    Vec3D rayOrigin_v = ray.getOrigin();
    Vec3D rayDirection_v = ray.getDirection();

    Real tPlane = intersectPlane(rayOrigin_v, rayDirection_v, mA_v, mNormal_v);
    if (tPlane < 0 || tPlane == infinity<Real>()) {
        // No intersection with containing plane
        return -infinity<Real>();
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
        return -infinity<Real>();
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

struct Enclosure Triangle::getEnclosure() {
    const auto x = {mA_v.x, mB_v.x, mC_v.x};
    const auto y = {mA_v.y, mB_v.y, mC_v.y};
    const auto z = {mA_v.z, mB_v.z, mC_v.z};

    const auto [xmin, xmax] = std::minmax_element(x.begin(), x.end());
    const auto [ymin, ymax] = std::minmax_element(y.begin(), y.end());
    const auto [zmin, zmax] = std::minmax_element(z.begin(), z.end());

    return Enclosure {
        *xmin, *xmax,
        *ymin, *ymax,
        *zmin, *zmax
    };
}


/* Sphere */
Sphere::Sphere(struct Material material, Vec3D center_V, Real radius)
:   IObject3D(material),
    mCenter_v(center_V), mRadius(radius) { }

Sphere::~Sphere() { }

Real Sphere::intersect(Ray& ray) {
    Vec3D dir_v = ray.getDirection();
    Vec3D oc_v = ray.getOrigin() - mCenter_v;
    Real a = dir_v.dot(dir_v);
    Real b = 2 * dir_v.dot(oc_v);
    Real c = oc_v.dot(oc_v) - mRadius*mRadius;

    Deg2Solution t_sol;
    solveDeg2(a, b, c, t_sol);
    if (!t_sol.valid) {
        return -infinity<Real>();
    }

    if (t_sol.x1 < 0) {
        if (t_sol.x2 < 0) {
            return -infinity<Real>();
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

struct Enclosure Sphere::getEnclosure() {
    return Enclosure {
        mCenter_v.x - mRadius, mCenter_v.x + mRadius,
        mCenter_v.y - mRadius, mCenter_v.y + mRadius,
        mCenter_v.z - mRadius, mCenter_v.z + mRadius,
    };
}

/* CompositeObject3D */
Real CompositeObject3D::intersect(Ray& ray) {
    return -infinity<Real>();

    Real t_boundary = mBoundary->intersect(ray);
    if (t_boundary < 0 || t_boundary == infinity<Real>()) {
        return -infinity<Real>();
    }
    // Ray intersected the boundary
    Real t;
    intersectObjects(ray, mObjects, t);
    return t;
}

// Protected
IObject3D* CompositeObject3D::intersectedObject(Ray& ray) {
    Real t;
    return intersectObjects(ray, mObjects, t);
}

Vec3D CompositeObject3D::getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    Ray ray(hitPoint_v, hitDirection_v);
    IObject3D* iObject = intersectedObject(ray);
    return iObject->getHitNormal(hitPoint_v, hitDirection_v);
}

Vec3D CompositeObject3D::getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) {
    Ray ray(hitPoint_v, hitDirection_v);
    IObject3D* iObject = intersectedObject(ray);
    return iObject->getHitNormal(hitPoint_v, hitDirection_v);
}

void CompositeObject3D::addObject(IObject3D* object) {
    mObjects.push_back(object);
}

std::vector<IObject3D*>& CompositeObject3D::children() {
    return mObjects;
}

struct Enclosure CompositeObject3D::getEnclosure() {
    return mEnclosure;
}

// TODO: Update enclosure and boundary
void CompositeObject3D::updateBoundary() {

}
