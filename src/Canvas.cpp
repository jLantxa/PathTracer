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

#include "Canvas.hpp"

Canvas::Canvas(unsigned width, unsigned height) : width(width), height(height) {
    rgb = new int*[width];
    for (int i = 0; i < width; i++) {
        rgb[i] = new int[height];
    }
}

Canvas::~Canvas() {
    for (int i = 0; i < width; i++) {
        delete rgb[i];
    }
    delete rgb;
}

unsigned Canvas::getWidth() {
    return width;
}

unsigned Canvas::getHeight() {
    return height;
}

int* Canvas::operator[](unsigned i) {
    return rgb[i];
}