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

#include "Utils.hpp"
#include "Common.hpp"
#include "Vector3D.hpp"
#include "Light.hpp"

#include <stdlib.h>
#include <cstdint>
#include <cmath>
#include <limits>

#include <stdio.h>
#include "test/TestCommon.hpp"

/** Calculate discriminant b^2 - 4ac */
inline Real discriminant(Real a, Real b, Real c) {
    return (b*b - 4*a*c);
}

void solveDeg2(Real a, Real b, Real c, struct Deg2Solution& result) {
    Real discr = discriminant(a, b, c);
    if (discr < 0) {
        result.valid = false;
        return;
    }

    // Second degree
    if (a != 0) {
        if (discr == 0) {
            Real r = -b/(2*a);  // XXX: Float, double, etc operation
            result.x1 = r;
            result.x2 = r;
        } else {
            Real a2 = 2*a;
            Real sqrtDisc = sqrt(discr);
            result.x1 = (-b + sqrtDisc) / a2; // XXX: Float, double, etc operation
            result.x2 = (-b - sqrtDisc) / a2;
        }
    } else {
        if (b != 0) {
            Real r = c/b;   // XXX: Float, double, etc operation
            result.x1 = r;
            result.x2 = r;
        } else {
            result.valid = false;
            return;
        }
    }


    result.valid = true;
}

Real intersectPlane(Vec3D l0, Vec3D l, Vec3D p0, Vec3D n) {
    Real den = n.dot(l);
    if (den == 0) {
        return -std::numeric_limits<Real>::infinity();
    }
    Real num = n.dot(p0 - l0);
    Real t = num/den;
    return t;
}

Vec3D sampleHemisphere(Vec3D& normal, uint16_t* Xi) {
    double phi = 2 * M_PI * erand48(Xi);
    double r = erand48(Xi);
    double rs = sqrt(r);

    Vec3D w = normal.normalize();
    Vec3D rand_v = (w.cross(Vec3D(0, 0, 1)).dist() > 0)?
        Vec3D(0, 0, 1) : Vec3D(1, 0, 0);

    Vec3D u = w.cross(rand_v);
    Vec3D v = w.cross(u);
    Vec3D sample_v =
        static_cast<Real>(cos(phi)*rs)*u +
        static_cast<Real>(sin(phi)*rs)*v +
        static_cast<Real>(sqrt(1-r))*w;

    return sample_v;
}
