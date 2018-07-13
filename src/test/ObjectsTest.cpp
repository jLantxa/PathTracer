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

#include <iostream>

#include "test/TestCommon.hpp"
#include "Common.hpp"
#include "Geometry.hpp"
#include "Objects.hpp"
#include "Light.hpp"
#include "Utils.hpp"

int main (int argc, char* argv[]) {
    Color sphereColor(0, 1, 0);
    Vec3D spherePos(0, 0, -2);
    Real sphereRadius = 1;
    Sphere sphere(sphereColor, spherePos, sphereRadius);

    Vec3D rayOrigin(0, 0, 0);
    Vec3D rayDirection(0, 0, -1);
    Ray ray(rayOrigin, rayDirection);

    Real t = sphere.intersect(ray);
    std::cout << t << std::endl;

    return 0;
}