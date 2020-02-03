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

#ifndef _INCLUDE_PATHTRACER_OBJECTS_H_
#define _INCLUDE_PATHTRACER_OBJECTS_H_

#include "Common.hpp"
#include "Light.hpp"
#include "Vector3D.hpp"

#include <vector>

/* Object library
 * These are the primitives to build a 3D scene. Any mesh can be constructed
 * simply by means of Triangle objects. Some more complex objects are also
 * provided.
 *
 * Available objects:
 *  - Plane
 *  - Triangle
 *  - Sphere
*/

struct Material {
    Color color;
    Color emission;
};

struct Enclosure {
    Real x_min, x_max;
    Real y_min, y_max;
    Real z_min, z_max;
};

/**
 * 3D Object base
 */
class IObject3D {
    public:
        IObject3D(struct Material material);
        virtual ~IObject3D();

        /** Returns the intersection point of the ray and the object
         * as the distance from the ray's origin in the direction of the ray.
        */
        virtual Real intersect(Ray& ray) = 0;

        /** Returns a vector normal to the object's surface in a given point
         * and hit direction.
        */
        virtual Vec3D getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) = 0;

        /** Returns a vector normal to the object's surface in a given point
         * facing out of the object
        */
        virtual Vec3D getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v) = 0;

        virtual struct Enclosure getEnclosure() = 0;

        struct Material& material();
        Color& color();

    protected:
        struct Material mMaterial;
};

/** A container of objects and light sources */
/// \todo Delete objects
struct Scene {
    std::vector<IObject3D*> objects;
    Color backgroundColor;
};

/// \todo Define a Cube object
/// \todo Define a Square object

/** A plane object derived from the IObject3D base */
class Plane : public IObject3D {
    public:
        Plane(struct Material material, Vec3D position_v, Vec3D normal_v);
        virtual ~Plane();

        virtual Real intersect(Ray& ray);
        virtual Vec3D getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);
        virtual Vec3D getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);

        virtual struct Enclosure getEnclosure();

    private:
        Vec3D mPosition_v;
        Vec3D mNormal_v;
};

/** A triangle object derived from the IObject3D base
 * Surface normal depends on the order of (A, B, C)
*/
class Triangle : public IObject3D {
    public:
        /** A colour and vertices A, B and C */
        Triangle(struct Material material, Vec3D A_v, Vec3D B_v, Vec3D C_v);
        virtual ~Triangle();

        virtual Real intersect(Ray& ray);
        virtual Vec3D getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);
        virtual Vec3D getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);

        virtual struct Enclosure getEnclosure();

    private:
        Vec3D mA_v, mB_v, mC_v;
        Vec3D mNormal_v;
};

/** A sphere object derived from the IObject3D base */
class Sphere : public IObject3D {
    public:
        Sphere(struct Material material, Vec3D center_v, Real radius);
        virtual ~Sphere();

        virtual Real intersect(Ray& ray);
        virtual Vec3D getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);
        virtual Vec3D getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);

        /// \todo Return the cubic enclosure of the sphere
        virtual struct Enclosure getEnclosure();

        Vec3D center() { return mCenter_v; }
        Real radius() { return mRadius; }

    private:
        Vec3D mCenter_v;
        Real mRadius;
};

/** A CompositeObject3D groups an arbitrary number of IObject3D entities
 * into a single one.
*/
class CompositeObject3D : public IObject3D {
public:
    CompositeObject3D();
    virtual ~CompositeObject3D() = 0;

    virtual Real intersect(Ray& ray);

    /* In the case of a CompositeObject3D we first need to calculate the
     * intersections to determine what object in the composite we hit
    */
    virtual Vec3D getHitNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);
    virtual Vec3D getSurfaceNormal(Vec3D& hitPoint_v, Vec3D& hitDirection_v);

    // Add object to vector
    virtual void addObject(IObject3D* object);

    // Get a reference to the children vector
    virtual std::vector<IObject3D*>& children();

    virtual struct Enclosure getEnclosure();

protected:
    /** \todo Define a Cube object to use as a container boundary.
     * Can this pointer leak?
    */
    IObject3D* mBoundary;
    struct Enclosure mEnclosure;
    std::vector<IObject3D*> mObjects;

    IObject3D* intersectedObject(Ray& ray);

    void updateBoundary();
};

#endif // _INCLUDE_PATHTRACER_OBJECTS_H_
