/*
 * This source file is part of PathTracer
 *
 * Copyright 2018, 2019 Javier Lancha Vázquez
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

#include "debug.hpp"

#define TAG "PathTracer"

// TODO: This value is completely random
Real ACCURACY = 0.0001;

PathTracer::PathTracer(unsigned depth) : mMaxDepth(depth) { }

PathTracer::~PathTracer() { }

void PathTracer::setCallback(void (*callback)(Canvas*)) {
    partialResultCallback = callback;
}

void PathTracer::notifyCallback(Canvas* partialCanvas) {
    if (partialResultCallback != nullptr) {
        partialResultCallback(partialCanvas);
    }
}

Canvas* PathTracer::renderScene(unsigned spp , struct Scene& scene, Camera& camera) {
    const unsigned  width = camera.getWidth();
    const unsigned height = camera.getHeight();
    Debug::Log::i(TAG, "Render scene: %dx%d", width, height);

    Canvas* canvas = new Canvas(width, height);

    for (int n = 1; n <= spp; n++) {
        Debug::Log::i(TAG, "Rendering %d/%d: %.2f%%", n, spp, 100.0*(n)/(spp));
        #pragma omp parallel for schedule(static)
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                Ray ray = camera.getRayToPixel(i, j);
                (*canvas)[i][j] = (*canvas)[i][j] + traceRay(0, ray, scene);
            }
        }

        canvas->spp = n;
        notifyCallback(canvas);
    }

    return canvas;
}

IObject3D* PathTracer::intersect(Ray& ray, Scene& scene, Real& t) {
    IObject3D* object_tmp = nullptr;
    t = std::numeric_limits<Real>::infinity();
    for (IObject3D* object : scene.objects) {
        Real t_tmp = object->intersect(ray);
        if (t_tmp > 0 && t_tmp < t) {
            object_tmp = object;
            t = t_tmp;
        }
    }
    return object_tmp;
}

Vec3D PathTracer::traceRay(unsigned depth, Ray& ray, struct Scene& scene) {
    if (depth >= mMaxDepth) {
        return Vec3D();
    }

    Real t;
    IObject3D* iObject = intersect(ray, scene, t);
    if (iObject == nullptr) {
        return Vec3D();
    }

    Vec3D iPoint_v = ray.point(t);
    Vec3D iDirection_v = ray.getDirection();
    Vec3D iNormal_v = iObject->getHitNormal(iPoint_v, iDirection_v);
    iPoint_v.set(iPoint_v + ACCURACY*iNormal_v);

    Vec3D iColor = iObject->color();
    Vec3D emission = iObject->material().emission*iColor;

    Vec3D sample_v = sampleHemisphere(iNormal_v, Xi);
    Real cos_theta = sample_v.dot(iNormal_v);

    const Real p = 1.0f / (2*M_PI);
    Ray sampleRay(iPoint_v, sample_v);

    Vec3D incoming = iColor * traceRay(depth+1, sampleRay, scene);
    return emission + p*incoming;
}
