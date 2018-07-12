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

#include "Utils.hpp"
#include "Common.hpp"

#include <cmath>
#include <limits>

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

    if (discr == 0) {
        Real r = -b/(2*a);
        result.x1 = result.x2 = -b/(2*a);
    } else {
        result.x1 = (-b + sqrt(discr)) / (2*a);
        result.x2 = (-b - sqrt(discr)) / (2*a);
    }
    
    result.valid = true;
}

Real intersectPlane(Vec3D l0, Vec3D l, Vec3D p0, Vec3D n) {
    Real den = n.dot(l);
    if (den == 0) {
        return std::numeric_limits<Real>::infinity();
    }
    Real num = n.dot(p0 - l0);
    Real t = num/den;
    return t;
}