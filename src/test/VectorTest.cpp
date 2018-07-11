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
#include <assert.h>

#include "TestCommon.h"
#include "Common.h"
#include "Geometry.h"
#include "Light.h"

int main(int argc, char* args[]) {
    Vec3D v1(1, 0, 0);
    Vec3D v2(0, 1, 0);
    Vec3D v3(0, 0, 1);
    float k = 1.5;

    Vec3D vsum = v1 + v2 + v3;
    Vec3D vsub = v1 - v2 - v3;

    float dot = v1.dot(vsum);
    float orthDot = v1.dot(v2);
    Vec3D cross = v1.cross(v2);
    Vec3D scalar = k * v1;

    printVector("vsum", vsum);
    printVector("vsub", vsub);
    std::cout << "dot = " << dot << std::endl;
    std::cout << "orthDot = " << orthDot << std::endl;
    printVector("cross", cross);
    printVector("scalar", scalar);

    Ray ray(v1, v2);
    Vec3D point = ray.point(1.0);
    printVector("ray point", point);

    Vec3D assertVec;
    assertVec.set(1, 1, 1);
    assert(vsum == assertVec);
    assertVec.set(1, -1, -1);
    assert(vsub == assertVec);
    assertVec.set(0, 0, 1);
    assert(cross == assertVec);
    assertVec.set(k, 0, 0);
    assert(scalar == assertVec);
    assert(dot == 1);
    assert(orthDot == 0);
    assertVec.set(1, 1, 0);
    assert(point == assertVec);

    std::cout << "All test passed." << std::endl;

    return 0;
}