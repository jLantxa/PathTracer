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

#include "Surface.hpp"

#include "Common.hpp"
#include "debug.hpp"
#include "Light.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <cstdio>

static const char* TAG = "Surface";

Surface::Surface(unsigned width, unsigned height) : width(width), height(height) {
    Debug::Log::d(TAG, "Construct surface %dx%d", width, height);
    color = new Color*[width];
    for (int i = 0; i < width; i++) {
        color[i] = new Color[height];
    }
}

Surface::~Surface() {
    Debug::Log::d(TAG, "Delete surface");
    for (int i = 0; i < width; i++) {
        delete[] color[i];
    }
    delete[] color;
}

unsigned Surface::getWidth() {
    return width;
}

unsigned Surface::getHeight() {
    return height;
}

Color* Surface::operator[](unsigned i) {
    return color[i];
}

void Surface::toPPM(const char* filename) {
    FILE *f = fopen(filename, "w");
    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int j = 0; j < height; j++)  {
        for (int i = 0; i < width; i++) {
            Color c = color[i][j];
            fprintf(f,"%d %d %d ", toColorInt(c.x), toColorInt(c.y), toColorInt(c.z));
        }
    }
    fclose(f);
}

void Surface::applyGammaCorrection(Real gamma) {
    for (int i = 0; i < width; i++)  {
        for (int j = 0; j < height; j++) {
            color[i][j] = gammaFunc(color[i][j], gamma);
        }
    }
}

void Surface::clear() {
    for (int i = 0; i < width; i++)  {
        for (int j = 0; j < height; j++) {
            color[i][j].set(0, 0, 0);
        }
    }
}
