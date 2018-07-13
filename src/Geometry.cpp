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

#include "Geometry.hpp"

#include <cmath>

/*
 * Definitions of Vector3D
*/
template <typename T>
Vector3D<T>::Vector3D(T vx, T vy, T vz) : x(vx), y(vy), z(vz) { }

template <typename T>
Vector3D<T>::Vector3D(T vx, T vy) : x(vx), y(vy), z(0) { }

template <typename T>
Vector3D<T>::Vector3D() : x(0), y(0), z(0) { }

template <typename T>
Vector3D<T>::~Vector3D() { }

template <typename T>
void Vector3D<T>::set(const Vector3D<T>& v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename T>
void Vector3D<T>::set(T sx, T sy, T sz) {
    x = sx;
    y = sy;
    z = sz;
}

template <typename T>
void Vector3D<T>::set(T sx, T sy) {
    x = sx;
    y = sy;
    z = 0;
}

template <typename T>
T Vector3D<T>::dist() {
    return sqrt(x*x + y*y + z*z);
}

template <typename T>
Vector3D<T> Vector3D<T>::normalize() {
    T norm = dist();
    Vector3D normalized(x/norm, y/norm, z/norm);
    return normalized;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator+(const Vector3D& v) {
    Vector3D sum(x+v.x, y+v.y, z+v.z);
    return sum;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator-(const Vector3D& v) {
    Vector3D<T> sum(x-v.x, y-v.y, z-v.z);
    return sum;
}

template <typename T>
T Vector3D<T>::dot(const Vector3D<T>& v) {
    return x*v.x + y*v.y + z*v.z;
}

template <typename T>
Vector3D<T> Vector3D<T>::cross(const Vector3D<T>& v) {
    Vector3D<T> cross(
        y*v.z - z*v.y,
        z*v.x - x*v.z,
        x*v.y - y*v.x);
    return cross;
}

template <typename T>
bool Vector3D<T>::operator==(const Vector3D<T>& v) {
    return (x==v.x && y==v.y && z==v.z);
}

template <typename T>
bool Vector3D<T>::operator!=(const Vector3D<T>& v) {
    return (x!=v.x || y!=v.y || z!=v.z);
}

/* Specialisations of Vector3D */
template class Vector3D<float>;
template class Vector3D<double>;