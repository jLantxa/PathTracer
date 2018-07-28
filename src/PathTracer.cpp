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

#include <stdio.h>

// TODO: This value is completely random
Real ACCURACY = 0.0001;

PathTracer::PathTracer(unsigned depth) : mMaxDepth(depth) { }

PathTracer::~PathTracer() { }

Canvas* PathTracer::renderScene(unsigned spp , struct Scene& scene, Camera& camera) {
    unsigned width = camera.getWidth();
    unsigned height = camera.getHeight();
    Real div = 1.0f/spp;

    Canvas* canvas = new Canvas(width, height);

    int cnt = 0;
    #pragma omp parallel for schedule(static) shared(cnt)
    for (int j = 0; j < height; j++) {
        printf("Rendering: %.2f\n", 100.0*(cnt)/(width*height));
        for (int i = 0; i < width; i++) {
            cnt++;
            Vec3D acc;
            for (int n = 0; n < spp; n++) {
                Ray ray = camera.getRayToPixel(i, j);
                acc = acc + traceRay(0, ray, scene);
            }
            (*canvas)[i][j] = div*acc;
        }
    }

    return canvas;
}

Object3D* PathTracer::intersect(Ray& ray, Scene& scene, Real& t) {
    Object3D* object_tmp = nullptr;
    t = std::numeric_limits<Real>::infinity();
    for (Object3D* object : scene.objects) {
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
    Object3D* iObject = intersect(ray, scene, t);
    if (iObject == nullptr) {
        return Vec3D();
    }

    Vec3D iPoint_v = ray.point(t);
    Vec3D iDirection_v = ray.getDirection();
    Vec3D iNormal_v = iObject->getHitNormal(iPoint_v, iDirection_v);
    iPoint_v.set(iPoint_v + ACCURACY*iNormal_v);

    Vec3D iColor = iObject->getColor();
    Vec3D emission = iObject->material.emission*iColor;

    Vec3D sample_v = sampleHemisphere(iNormal_v, Xi);
    Real cos_theta = sample_v.dot(iNormal_v);
    
    const Real p = 1.0f / (2*M_PI);
    Ray sampleRay(iPoint_v, sample_v);

    Vec3D incoming = iColor * traceRay(depth+1, sampleRay, scene);
    return emission + p*incoming;
}