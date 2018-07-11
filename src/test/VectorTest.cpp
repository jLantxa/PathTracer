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

#include "Geometry.h"

void printVector(Vector3D<float>& v) {
    std::cout << "(" <<
        v.x << ", " <<
        v.y << ", " <<
        v.z << ")" << std::endl;
}

int main(int argc, char* args[]) {
    Vector3D<float> v1(1, 0, 0);
    Vector3D<float> v2(0, 1, 0);
    Vector3D<float> v3(0, 0, 1);

    Vector3D<float> vsum = v1 + v2 + v3;
    Vector3D<float> vsub = v1 - v2 - v3;

    float dot = v1.dot(vsum);
    float orthDot = v1.dot(v2);
    Vector3D<float> cross = v1.cross(v2);

    printVector(vsum);
    printVector(vsub);
    std::cout << dot << std::endl;
    std::cout << orthDot << std::endl;
    printVector(cross);

    return 0;
}