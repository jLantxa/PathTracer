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

#include "PathTracer.hpp"

#include "Canvas.hpp"
#include "Common.hpp"
#include "Camera.hpp"
#include "Objects.hpp"
#include "Light.hpp"
#include "Utils.hpp"

#include <limits>
#include <vector>

#include <iostream>

// TODO: This value is completely random
Real ACCURACY = 0.0001;

PathTracer::PathTracer(unsigned depth) : mMaxDepth(depth) { }

PathTracer::~PathTracer() { }

Canvas* PathTracer::renderScene(struct Scene& scene, Camera& camera) {
    unsigned width = camera.getWidth();
    unsigned height = camera.getHeight();

    Canvas* canvas = new Canvas(width, height);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Ray ray = camera.getRayToPixel(i, j);
            Color color = traceRay(0, ray, scene);
            (*canvas)[i][j] = color;
        }
    }

    return canvas;
}

Color PathTracer::traceRay(unsigned depth, Ray& cameraRay, struct Scene& scene) {
    if (depth > mMaxDepth) {
        return Color(0, 0, 0);
    }

    Object3D* intersectObject = nullptr;

    Real t_intersect = std::numeric_limits<Real>::infinity();
    for (Object3D* object : scene.objects) {
        Real t = object->intersect(cameraRay);
        if (t > 0 && t < t_intersect) {
            intersectObject = object;
            t_intersect = t;
        }
    }

    if (intersectObject == nullptr) {
        return Color(0, 0, 0);
    }

#ifndef RAY_TRACING

    Color color;

    // TODO: Algorithm here :)

    return color;

#else // Ray tracer. Maintained only for testing
    Color color;
    Color objectColor = intersectObject->getColor();
    color.set(objectColor);

    Vec3D intersectPoint_v = cameraRay.point(t_intersect);
    Vec3D intersectDirection_v = cameraRay.getDirection();
    intersectPoint_v.set(intersectPoint_v + ACCURACY*intersectObject->getNormal(intersectPoint_v, intersectDirection_v));

    for (LightSource* lightSource : scene.lights) {
        bool inShadow = false;
        Color lightColor = lightSource->getColor();
        Vec3D toLight_v = lightSource->getPosition() - intersectPoint_v;
        Ray shadowRay(intersectPoint_v, toLight_v);

        for (Object3D* sObject : scene.objects) {
            Real t = sObject->intersect(shadowRay);
            Vec3D ori_v = shadowRay.getOrigin();
            Vec3D dir_v = shadowRay.getDirection();

            if (t > 0 && t < toLight_v.dist()) {
                inShadow = true;
                break;
            }
        }

        // TODO: This makes no sense, it is just a test to draw shadowed pixels
        if (inShadow) {
            color.R *= 0.2;
            color.B *= 0.2;
            color.G *= 0.2;
        }
    }

    return color;
#endif
}