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
#include "Camera.h"

int main(int argc, char* args[]) {
    unsigned width = 640;
    unsigned height = 480;
    float fov = 90.0;

    Camera camera(width, height, fov);
    
    Vec3D dir;
    for (int i = 0; i < width; i+=width/4) {
        for (int j = 0; j < height; j+=height/4) {
            dir.set(camera.getRayToPixel(i, j).getDirection());
            std::cout << "(" << i << ", " << j << ") --> ";
            printVector(dir);
        }
    }

    return 0;
}