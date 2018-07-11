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

#ifndef _INCLUDE_GEOMETRY_H_
#define _INCLUDE_GEOMETRY_H_

template <typename T> class Vector3D {
    public:
        T x, y, z;

        Vector3D(T x, T y, T z);
        Vector3D(T x, T y);
        Vector3D();
        virtual ~Vector3D();

        T dist();
        Vector3D normalize();

        // Addition and subtraction
        Vector3D operator+(Vector3D& v);
        Vector3D operator-(Vector3D& v);

        T dot(Vector3D& v);
        Vector3D cross(Vector3D& v);
};

template <typename T>
T operator*(T k, Vector3D<T>& v);

template <typename T>
T operator*(Vector3D<T>& v, T k);


#endif // _INCLUDE_GEOMETRY_H_