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

#ifndef _INCLUDE_RAYTRACER_GEOMETRY_H_
#define _INCLUDE_RAYTRACER_GEOMETRY_H_

#include <math.h>

/** A 3D vector with its typical operators */
template <typename T> class Vector3D {
    public:
        T x, y, z;

        /** 3D constructor */
        Vector3D(T x, T y, T z);
        /** 2D constructor (z = 0) */
        Vector3D(T x, T y);
        /** Creates a vector with zero components (0, 0, 0) */
        Vector3D();
        virtual ~Vector3D();

        /** Copy components from vector v */
        void set(const Vector3D& v);
        /** Set the 3D components */
        void set(T x, T y, T z);
        /** Set the 2D components (z = 0) */
        void set(T x, T y);

        /** Return the norm of the vector */
        T dist();
        /** Return this vector normalized */
        Vector3D normalize();

        // Addition and subtraction
        Vector3D operator+(const Vector3D& v);
        Vector3D operator-(const Vector3D& v);

        /** Pre-multiplication with a scalar */
        template <typename U>
        friend Vector3D<U> operator*(const U k, const Vector3D<U>& v);

        /** Dot product */
        T dot(const Vector3D& v);
        /** Cross product */
        Vector3D cross(const Vector3D& v);

        bool operator==(const Vector3D& v);
        bool operator!=(const Vector3D& v);
};

/** Pre-multiplication with a scalar */
template <typename U>
Vector3D<U> operator*(const U k, const Vector3D<U>& v) {
    Vector3D<U> mult(k*v.x, k*v.y, k*v.z);
    return mult;
}

inline float degToRad(float deg) {
    return M_PI * deg / 180.0;
}

inline float radToDeg(float rad) {
    return 180.0 * rad / M_PI;
}


#endif // _INCLUDE_RAYTRACER_GEOMETRY_H_