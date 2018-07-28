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

#include "Canvas.hpp"
#include "Light.hpp"

#include <stdio.h>

Canvas::Canvas(unsigned width, unsigned height) : width(width), height(height) {
    color = new Vec3D*[width];
    for (int i = 0; i < width; i++) {
        color[i] = new Vec3D[height];
    }
}

Canvas::~Canvas() {
    for (int i = 0; i < width; i++) {
        delete color[i];
    }
    delete color;
}

unsigned Canvas::getWidth() {
    return width;
}

unsigned Canvas::getHeight() {
    return height;
}

Vec3D* Canvas::operator[](unsigned i) {
    return color[i];
}

void Canvas::toPPM(const char* filename) {
    FILE *f = fopen(filename, "w");
    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int i = 0; i < height; i++)  {
        for (int j = 0; j < width; j++) {
            Vec3D c = color[j][i];
            fprintf(f,"%d %d %d ", toColorInt(c.x/spp), toColorInt(c.y/spp), toColorInt(c.z/spp));
        }
    }
    fclose(f);
}