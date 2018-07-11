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

#ifndef _INCLUDE_RAYTRACER_TEST_COMMON_H_
#define _INCLUDE_RAYTRACER_TEST_COMMON_H_

#include <iostream>
#include "Common.h"
#include "Geometry.h"

void printVector(Vec3D& v) {
    std::cout << "(" <<
        v.x << ", " <<
        v.y << ", " <<
        v.z << ")" << std::endl;
}

void printVector(std::string pfx, Vec3D& v) {
    std::cout << pfx;
    printVector(v);
}

#endif // _INCLUDE_RAYTRACER_TEST_COMMON_H_