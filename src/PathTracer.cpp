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

#include "Common.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Objects.hpp"
#include "Surface.hpp"
#include "Utils.hpp"

#include <chrono>
#include <limits>
#include <vector>

#include "debug.hpp"

static const char* TAG = "PathTracer";

/// \todo ACCURACY value is completely random
Real ACCURACY = 0.0001;

PathTracer::PathTracer(unsigned spp, unsigned depth)
:   mSPP(spp), mMaxDepth(depth)
{ }

PathTracer::~PathTracer() { }

void PathTracer::render(struct Scene& scene, Camera& camera) {
    Surface& surface = camera.getSurface();
    const unsigned width = surface.getWidth();
    const unsigned height = surface.getHeight();
    Debug::Log::i(TAG, "Render scene: %dx%d", width, height);

    surface.clear();
    for (int n = 1; n <= mSPP; n++) {
        Debug::Log::i(TAG, "Rendering %d/%d: %.2f%%", n, mSPP, 100.0*(n-1)/(mSPP));

        #pragma omp parallel for schedule(static)
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                Ray ray = camera.getRayToPixel(i, j);
                surface[i][j] += traceRay(0, ray, scene);
            }
        }
    }

    const Real ispp = 1.0f/mSPP;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            surface[i][j] *= ispp;
        }
    }
}

Color PathTracer::traceRay(unsigned depth, Ray& ray, struct Scene& scene) {
    if (depth >= mMaxDepth) {
        return Color();
    }

    Real t;
    IObject3D* iObject = intersectObjects(ray, scene.objects, t);
    if (iObject == nullptr) {
            return Color();
    }

    Vec3D iPoint_v = ray.point(t);
    Vec3D iDirection_v = ray.getDirection();
    Vec3D iNormal_v = iObject->getHitNormal(iPoint_v, iDirection_v);
    iPoint_v.set(iPoint_v + ACCURACY*iNormal_v);

    struct Material iMaterial = iObject->material();
    Color emission = iMaterial.emission;

    Vec3D sample_v = sampleHemisphere(iNormal_v);
    Real cos_theta = sample_v.dot(iNormal_v);

    const Real p = 1.0/(2*M_PI);
    Ray sampleRay(iPoint_v, sample_v);

    Color incoming = iMaterial.color * traceRay(depth+1, sampleRay, scene);
    return emission + p*incoming;
}
