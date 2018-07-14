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

#ifndef _INCLUDE_RAYTRACER_OBJECTS_H_
#define _INCLUDE_RAYTRACER_OBJECTS_H_

#include "Common.hpp"
#include "Geometry.hpp"
#include "Light.hpp"

/**
 * 3D Object base
 */
class Object3D {
    public:
        Object3D(Color color);
        virtual ~Object3D();

        virtual Color getColor() = 0;
        /**
         * Returns the intersection point of the ray and the object
         * as the distance from the ray's origin in the direction of the ray.
        */
        virtual Real intersect(Ray& ray) = 0;

        /**
         * Returns a vector normal to the object's surface in a given point
         * and hit direction.
        */
        virtual Vec3D getNormal(Vec3D& hitPoint, Vec3D& hitDirection) = 0;

    protected:
        Color color;
};


/** A sphere object derived from the Object3D base */
class Sphere : public Object3D {
    public:
        Sphere(Color color, Vec3D center, Real radius);
        virtual ~Sphere();

        virtual Color getColor();
        virtual Real intersect(Ray& ray);
        virtual Vec3D getNormal(Vec3D& hitPoint, Vec3D& hitDirection);

    private:
        Vec3D center;
        Real radius;
};

/** A plane object derived from the Object3D base */
class Plane : public Object3D {
    public:
        Plane(Color color, Vec3D position, Vec3D normal);
        virtual ~Plane();

        virtual Color getColor();
        virtual Real intersect(Ray& ray);
        virtual Vec3D getNormal(Vec3D& hitPoint, Vec3D& hitDirection);

    private:
        Vec3D position;
        Vec3D normal;
};

/** A triangle object derived from the Object3D base */
class Triangle : public Object3D {
    public:
        /** A colour and vertices A, B and C */
        Triangle(Color color, Vec3D A, Vec3D B, Vec3D C);
        virtual ~Triangle();

        virtual Color getColor();
        virtual Real intersect(Ray& ray);
        virtual Vec3D getNormal(Vec3D& hitPoint, Vec3D& hitDirection);

    private:
        Vec3D A, B, C;
        Vec3D normal;
};

#endif // _INCLUDE_RAYTRACER_OBJECTS_H_
