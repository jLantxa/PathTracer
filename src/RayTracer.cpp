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

#include "RayTracer.hpp"

#include "Common.hpp"
#include "Camera.hpp"
#include "Objects.hpp"
#include "Light.hpp"
#include "Utils.hpp"

#include <limits>

/* Canvas */
Canvas::Canvas() : width(0), height(0) {
    initRGB(0, 0);
}

Canvas::Canvas(unsigned width, unsigned height) : width(width), height(height) {
    initRGB(width, height);
}

void Canvas::freeRGB() {
    for (int i = 0; i < width; i++) {
        delete rgb[i];
    }
    delete rgb;
}

void Canvas::initRGB(unsigned w, unsigned h) {
    if (rgb != nullptr) {
        freeRGB();
    }

    rgb = new int*[w];
    for (int i = 0; i < w; i++) {
        rgb[i] = new int[h];
    }
}

void Canvas::resize(unsigned newWidth, unsigned newHeight) {
    initRGB(newWidth, newHeight);
}

int& Canvas::operator[](unsigned i) {
    return *rgb[i];
}

RayTracer::RayTracer() { }

RayTracer::~RayTracer() { }

void RayTracer::renderScene(struct Scene& scene, Camera& camera, struct Canvas* canvas) {
    unsigned width = camera.getWidth();
    unsigned height = camera.getHeight();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Ray ray = camera.getRayToPixel(i, j);
            Color color = castRayFromPixel(ray, scene);
            canvas[i][j] = colorToRGB(color);
        }
    }
}

Color RayTracer::castRayFromPixel(Ray& ray, struct Scene& scene) {
    Object3D* intersectObject = nullptr;
    Color color(0, 0, 0);   // Black background

    Real closestIntersect = std::numeric_limits<Real>::infinity();
    for (Object3D* object : scene.objects) {
        Real intersect = object->intersect(ray);
        if (intersect < closestIntersect) {
            intersectObject = object;
            closestIntersect = intersect;
        }
    }

    // An intersection with the primary ray occured
    if (intersectObject != nullptr) {
        color.set(intersectObject->getColor());
    }

    return color;
}