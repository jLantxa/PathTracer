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

#include "Canvas.hpp"
#include "Common.hpp"
#include "Camera.hpp"
#include "Objects.hpp"
#include "Light.hpp"
#include "Utils.hpp"

#include <limits>
#include <vector>

RayTracer::RayTracer() { }

RayTracer::~RayTracer() { }

Canvas* RayTracer::renderScene(struct Scene& scene, Camera& camera) {
    unsigned width = camera.getWidth();
    unsigned height = camera.getHeight();

    Canvas* canvas = new Canvas(width, height);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Ray ray = camera.getRayToPixel(i, j);
            Color color = castRayInScene(ray, scene);
            (*canvas)[i][j] = colorToRGB(color);
        }
    }

    return canvas;
}

Color RayTracer::castRayInScene(Ray& ray, struct Scene& scene) {
    Object3D* intersectObject = nullptr;
    Color color(0, 0, 0);   // Black background

    Real closestIntersect = std::numeric_limits<Real>::infinity();
    for (Object3D* object : scene.objects) {
        Real intersect = object->intersect(ray);
        if (intersect > 0 && intersect < closestIntersect) {
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