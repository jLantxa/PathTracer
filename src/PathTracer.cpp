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

#include <algorithm>
#include <chrono>
#include <limits>
#include <vector>

#include "debug.hpp"

static const char* TAG = "PathTracer";

/// \todo ACCURACY value is completely random
Real ACCURACY = 0.0001;

void PathTracer::calculateBlocks(std::vector<Block>& blocks, unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < width; i += mBlockWidth) {
        unsigned int right = i + mBlockWidth;
        if (right > width) right = width;

        for (unsigned int j = 0; j < height; j += mBlockWidth) {
            unsigned int down = j + mBlockWidth;
            if (down > height) down = height;

            Block block = {
                .left = i,
                .up = j,
                .right = right,
                .down = down
            };
            blocks.push_back(block);
        }
    }
}

void PathTracer::reorderBlocks(std::vector<Block>& blocks, unsigned width, unsigned height) {
    int cx = width / 2;
    int cy = height / 2;
    std::sort(blocks.begin(), blocks.end(),
        [&](const Block& lhs, const Block& rhs)-> bool {
            int lx = (lhs.left + lhs.right) / 2;
            int ly = (lhs.up + lhs.down) / 2;
            int rx = (rhs.left + rhs.right) / 2;
            int ry = (rhs.up + rhs.down) / 2;

            float dl = pow(lx - cx, 2) + pow(ly - cy, 2);
            float dr = pow(rx - cx, 2) + pow(ry - cy, 2);

            return dl < dr;
        }
    );
}

PathTracer::PathTracer(unsigned spp, unsigned depth)
:   mMaxDepth(depth), mSPP(spp)
{ }

PathTracer::~PathTracer() { }

void PathTracer::addCallback(IResultsListener* listener) {
    mPartialResultListeners.push_back(listener);
}

void PathTracer::notifyPartialResult(struct Scene& scene, Camera& camera) {
    for(IResultsListener* listener : mPartialResultListeners) {
        listener->onPartialResult(scene, camera);
    }
}

void PathTracer::notifyRenderFinished(struct Scene& scene, Camera& camera) {
    for(IResultsListener* listener : mPartialResultListeners) {
        listener->onRenderFinished(scene, camera);
    }
}

void PathTracer::setBlockSize(unsigned int width, unsigned int height) {
    mBlockWidth = width;
    mBlockHeight = height;
}

void PathTracer::render(struct Scene& scene, Camera& camera) {
    Surface& surface = camera.getSurface();
    const unsigned width = surface.getWidth();
    const unsigned height = surface.getHeight();
    surface.clear();
    Debug::Log::i(TAG, "Render scene: %dx%d", width, height);

    std::vector<Block> blocks;
    calculateBlocks(blocks, width, height);
    reorderBlocks(blocks, width, height);

    for (unsigned int b = 0; b < blocks.size(); b++) {
        #pragma omp parallel for
        for (unsigned int i = blocks[b].left; i < blocks[b].right; i++) {
            for (unsigned int j = blocks[b].up; j < blocks[b].down; j++) {
                Ray ray = camera.getRayToPixel(i, j);
                for (unsigned int n = 0; n < mSPP; n++) {
                    surface[i][j] += traceRay(0, ray, scene);
                }
                surface[i][j] *= 1.0f/mSPP;
            }
        }

        notifyPartialResult(scene, camera);
    }

    notifyRenderFinished(scene, camera);
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

    const Real p = 1.0/(2*M_PI);
    Ray sampleRay(iPoint_v, sample_v);

    Color incoming = iMaterial.color * traceRay(depth+1, sampleRay, scene);
    return emission + p*incoming;
}
