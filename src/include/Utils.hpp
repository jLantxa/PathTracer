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

#ifndef _INCLUDE_RAYTRACER_UTILS_H_
#define _INCLUDE_RAYTRACER_UTILS_H_

#include "Common.hpp"
#include "Geometry.hpp"

/** Calculate discriminant b^2 - 4ac */
Real discriminant(Real a, Real b, Real c);

struct Deg2Solution {
    bool valid = false;
    Real x1;
    Real x2;
};

void solveDeg2(Real a, Real b, Real c, struct Deg2Solution& result);

Real intersectPlane(Vec3D l0, Vec3D l, Vec3D p0, Vec3D n);

#endif // _INCLUDE_RAYTRACER_UTILS_H_