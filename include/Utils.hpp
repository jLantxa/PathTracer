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

#ifndef _INCLUDE_PATHTRACER_UTILS_H_
#define _INCLUDE_PATHTRACER_UTILS_H_

#include "Common.hpp"
#include "Vector3D.hpp"
#include "Objects.hpp"

#include <cmath>
#include <vector>

/** Calculate discriminant b^2 - 4ac */
Real discriminant(Real a, Real b, Real c);

struct Deg2Solution {
    bool valid = false;
    Real x1;
    Real x2;
};

void solveDeg2(Real a, Real b, Real c, struct Deg2Solution& result);

Real intersectPlane(Vec3D l0, Vec3D l, Vec3D p0, Vec3D n);

Vec3D sampleHemisphere(Vec3D& normal);

IObject3D* intersectObjects(Ray& ray, std::vector<IObject3D*>& objects, Real& t);

template <typename T>
inline T infinity() {
    return std::numeric_limits<T>::infinity();
}

Real gammaFunc(Real x, Real gamma);
Color gammaFunc(Color c, Real gamma);

template <typename T>
class CircularTimeArray {
public:
    CircularTimeArray(unsigned length) : mLength(length), mHead(0) {
        mValues = new T[length];
    }

    void add(T val) {
        mValues[mHead] = val;
        mHead = (mHead+1) % mLength;
    }

    Real mean() {
        T acc = 0;
        unsigned valid = 0;
        for (unsigned i = 0; i < mLength; i++) {
            T val = mValues[i];
            if (val <= 0) continue;
            acc += val;
            valid++;
        }
        return acc/valid;
    }

private:
    unsigned mLength;
    unsigned mHead;
    T* mValues;
};

#endif // _INCLUDE_PATHTRACER_UTILS_H_
